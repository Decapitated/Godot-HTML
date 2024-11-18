#include "convert.hpp"

#include <godot_cpp/variant/utility_functions.hpp>

#include <JavaScriptCore/JSRetainPtr.h>

shared_ptr<VariantRefrences> Convert::variant_refrences = nullptr;

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
    if(variant_refrences == nullptr)
    {
        variant_refrences = shared_ptr<VariantRefrences>(new VariantRefrences());
    }
    switch(variant.get_type())
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
            // Create a class reference from the class function definition.
            JSRetainPtr<JSClassRef> classReference = adopt(JSClassCreate(&classFunctionDefinition));

            shared_ptr<Variant> private_data = shared_ptr<Variant>(new Variant(variant));
            variant_refrences->push_back(private_data);

            // Create a function callback entry that references the class we created for the private data.
            JSObjectRef nativeFunc = JSObjectMake(context, classReference.get(), private_data.get());

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
        // Leave these here to be implemented later.
        case Variant::ARRAY:
        case Variant::OBJECT:
        default: {
            JSRetainPtr<JSStringRef> js_string = adopt(
                JSStringCreateWithUTF8CString(
                    variant.stringify().utf8().get_data()
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


JSValueRef Convert::CallableCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    // Get Callable variant from private data.
    Variant* private_data = (Variant*)JSObjectGetPrivate(function);
    if(private_data == nullptr)
    {
        return JSThrowError(ctx, "No function private data.", exception);
    }
    Callable callable = *private_data;
    // Ensure the number of arguments is correct.
    if(argumentCount != callable.get_argument_count())
    {
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
