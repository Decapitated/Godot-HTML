#include "convert.hpp"

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/object.hpp>

#include <JavaScriptCore/JSRetainPtr.h>

Variant Convert::ToVariant(JSContextRef context, JSValueRef value)
{
    JSType value_type = JSValueGetType(context, value);
    switch(value_type)
    {
        case kJSTypeBoolean:
            return JSValueToBoolean(context, value);
        case kJSTypeNumber:
            return JSValueToNumber(context, value, NULL);
        case kJSTypeString: {
            JSStringRef js_string = JSValueToStringCopy(context, value, NULL);
            ultralight::String string = JSString(js_string);
            return godot::String(string.utf8().data());
        }
        default:
            return Variant();
    }
}

JSValueRef Convert::ToJSValue(JSContextRef context, Variant variant)
{
    Variant::Type variant_type = variant.get_type();
    switch(variant_type)
    {
        case Variant::NIL:
            return JSValueMakeNull(context);
        case Variant::BOOL:
            return JSValueMakeBoolean(context, variant);
        case Variant::INT:
        case Variant::FLOAT:
            return JSValueMakeNumber(context, variant);
        case Variant::STRING: {
            JSRetainPtr<JSStringRef> js_string = adopt(
                JSStringCreateWithUTF8CString(
                    ((godot::String)variant).utf8().get_data()
                )
            );
            return JSValueMakeString(context, js_string.get());
        }
        case Variant::CALLABLE: {
            // Create class function definition for our callback.
            JSClassDefinition classFunctionDefinition = kJSClassDefinitionEmpty;
            classFunctionDefinition.callAsFunction = CallableCallback;
            classFunctionDefinition.finalize = FinalizeCallable;
            // Create a class reference from the class function definition.
            JSRetainPtr<JSClassRef> classReference = adopt(JSClassCreate(&classFunctionDefinition));
            // Create a function callback entry that references the class we created for the private data.
            JSObjectRef nativeFunc = JSObjectMake(context, classReference.get(), memnew(Variant(variant)));

            return nativeFunc;
        }
        case Variant::DICTIONARY: {
            Dictionary dict = variant;
            JSObjectRef godot_obj = JSObjectMake(context, NULL, NULL);
            auto keys = dict.keys();
            for(int i = 0; i < keys.size(); i++)
            {
                auto key = keys[i];
                auto value = dict[key];

                JSRetainPtr<JSStringRef> js_key = adopt(JSStringCreateWithUTF8CString(key.stringify().utf8().get_data()));
                JSValueRef js_value = Convert::ToJSValue(context, value);
                JSObjectSetProperty(context, godot_obj, js_key.get(), js_value, 0, 0);
            }
            return godot_obj;
        }
        case Variant::ARRAY: {
            Array array = variant;
            auto array_size = array.size();
            shared_ptr<JSValueRef[]> array_data(new JSValueRef[array_size]);
            for(int i = 0; i < array_size; i++)
            {
                JSValueRef js_value = Convert::ToJSValue(context, array[i]);
                array_data[i] = js_value;
            }
            JSObjectRef js_array = JSObjectMakeArray(context, array_size, array_data.get(), NULL);
            return js_array;
        }
        // Leave these here to be implemented later.
        case Variant::OBJECT: {
            Object* object = variant;
            JSObjectRef js_obj = JSObjectMake(context, NULL, NULL);

            auto properties = object->get_property_list();
            auto num_properties = properties.size();
            for(int i = 0; i < num_properties; i++)
            {
                Dictionary property_info = properties[i];

                godot::String name = property_info["name"];
                Variant::Type type = static_cast<Variant::Type>((int)property_info["type"]);
                PropertyUsageFlags usage = static_cast<PropertyUsageFlags>((uint64_t)property_info["usage"]);
            
                if(type != Variant::NIL &&  !(usage & PROPERTY_USAGE_INTERNAL))
                {
                    Variant property = object->get(name);
                    JSValueRef js_value;
                    if(type == Variant::OBJECT && !property.booleanize())
                    {
                        js_value = JSValueMakeNull(context);
                    } else {
                        js_value = Convert::ToJSValue(context, property);
                    }
                    JSRetainPtr<JSStringRef> js_name = adopt(JSStringCreateWithUTF8CString(name.utf8().get_data()));
                    JSObjectSetProperty(context, js_obj, js_name.get(), js_value, 0, 0);
                }
            }

            auto methods = object->get_method_list();
            auto num_methods = methods.size();
            for(int i = 0; i < num_methods; i++)
            {
                Dictionary method_info = methods[i];
                godot::String name = method_info["name"];
                MethodFlags flags = static_cast<MethodFlags>((uint64_t)method_info["flags"]);
                bool is_invalid = flags & METHOD_FLAG_EDITOR || flags & METHOD_FLAG_VIRTUAL || flags & METHOD_FLAG_OBJECT_CORE;
                if(!is_invalid)
                {
                    Callable callable = object->get(name);
                    JSRetainPtr<JSStringRef> js_name = adopt(JSStringCreateWithUTF8CString(name.utf8().get_data()));
                    JSValueRef js_value = Convert::ToJSValue(context, callable);
                    JSObjectSetProperty(context, js_obj, js_name.get(), js_value, 0, 0);
                }
            }

            return js_obj;
        }
        default: {
            JSRetainPtr<JSStringRef> js_string = adopt(
                JSStringCreateWithUTF8CString(
                    (Variant::get_type_name(variant_type) + variant.stringify()).utf8().get_data()
                )
            );
            return JSValueMakeString(context, js_string.get());
        }
    }
}

JSValueRef JSThrowError(JSContextRef context, const char* message, JSValueRef* exception)
{
    JSRetainPtr<JSStringRef> errorMessage = adopt(JSStringCreateWithUTF8CString(message));
    *exception = JSValueMakeString(context, errorMessage.get());
    return JSValueMakeUndefined(context);
}

JSValueRef Convert::CallableCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject,
                                     size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    // Get Callable variant from private data.
    Variant* private_data = (Variant*)JSObjectGetPrivate(function);
    if(private_data == nullptr)
    {
        UtilityFunctions::printerr("No function private data.");
        return JSThrowError(ctx, "No function private data.", exception);
    }
    // Check if callable is valid.
    Callable callable = *private_data;
    if(!callable.is_valid())
    {
        UtilityFunctions::printerr("Invalid callable.");
        return JSThrowError(ctx, "Invalid callable.", exception);
    }
    // Ensure the number of arguments is correct.
    if(argumentCount != callable.get_argument_count())
    {
        UtilityFunctions::printerr("Invalid number of arguments.");
        return JSThrowError(ctx, "Invalid number of arguments.", exception);
    }
    // Convert the argument array to a Variant array.
    Array call_args = Array();
    for(int i = 0; i < argumentCount; i++)
    {
        call_args.append(Convert::ToVariant(ctx, arguments[i]));
    }
    // Call the function.
    Variant return_value = callable.callv(call_args);
    // Convert the return value and return.
    JSValueRef return_value_ref = Convert::ToJSValue(ctx, return_value);
    return Convert::ToJSValue(ctx, return_value);
}

void Convert::FinalizeCallable(JSObjectRef function)
{
    Variant* private_data = (Variant*)JSObjectGetPrivate(function);
    if(private_data != nullptr)
    {
        memdelete(private_data);
    }
}
