#include "html_rect.hpp"

#include <functional>

#include "godot/ultralight_singleton/ultralight_singleton.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <JavaScriptCore/JavaScript.h>
#include <JavaScriptCore/JSRetainPtr.h>
#include <AppCore/JSHelpers.h>

using namespace godot;

void HtmlRect::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("get_index"), &HtmlRect::get_index);
	ClassDB::bind_method(D_METHOD("set_index", "p_index"), &HtmlRect::set_index);
	ClassDB::add_property(get_class_static(), PropertyInfo(Variant::STRING, "index_path"), "set_index", "get_index");

	GDVIRTUAL_BIND(_on_dom_ready, "url");
}

HtmlRect::HtmlRect()
{
    CreateView();
}

HtmlRect::~HtmlRect() {}

void HtmlRect::CreateView()
{
    Vector2 size = get_size();

    ViewConfig view_config;
    view_config.is_accelerated = false;
    view_config.is_transparent = true;

    RefPtr<View> view = UltralightSingleton::get_singleton()->CreateView((int)size.x, (int)size.y, view_config, nullptr);

    if(view)
    { 
        SetView(view);
        if(!LoadIndex(view)) view->LoadHTML("<h1>Placeholder Text</h1>");
    }
    
}

bool HtmlRect::LoadIndex(RefPtr<View> view)
{
    if(index_path.is_empty()) return false;
    auto path_parts = index_path.split("://");
    if(path_parts.size() == 1) 
    {
        view->LoadURL(("file:///"+index_path).utf8().get_data());
    }
    else
    {
        view->LoadURL(index_path.utf8().get_data());
    }
    return true;
}

void HtmlRect::set_index(const String p_index)
{
    String currentPath = index_path;
	index_path = p_index;
    if(!LoadIndex(GetView())) index_path = currentPath;
}

godot::String HtmlRect::get_index() const
{
	return index_path;
}

Dictionary HtmlRect::call_on_dom_ready(const String &url)
{
    Dictionary obj;
    if(GDVIRTUAL_CALL(_on_dom_ready, url, obj))
    {
        return obj;
    }
    return _on_dom_ready(url);
}

void HtmlRect::OnDOMReady(ultralight::View *caller, uint64_t frame_id, bool is_main_frame, const ultralight::String &url)
{
    // Acquire the JS execution context for the current page.
    auto scoped_context = caller->LockJSContext();
    
    // Typecast to the underlying JSContextRef.
    JSContextRef context = (*scoped_context);
    
    String godot_url = godot::String(url.utf8().data());
    Dictionary godot_obj_dict = call_on_dom_ready(godot_url);
    godot_obj = ConvertDictionaryToMap(godot_obj_dict);

    StoreGlobalObject(context, godot_obj);
}

#pragma region Static Functions

JSValueRef HtmlRect::CallableCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    Variant* private_data = (Variant*)JSObjectGetPrivate(function);
    if(private_data == nullptr) UtilityFunctions::printerr("No function private data.");

    Callable callable = *private_data;
    int64_t callable_argument_count = callable.get_argument_count();

    if(argumentCount != callable_argument_count)
    {
        UtilityFunctions::printerr("Argument count mismatch.");
        JSRetainPtr<JSStringRef> errorMessage = adopt(JSStringCreateWithUTF8CString("Argument count mismatch."));
        *exception = JSValueMakeString(ctx, errorMessage.get());
        return JSValueMakeUndefined(ctx);
    }
    // Convert the argument array to a Variant array.
    Array call_args = Array();
    for(int i = 0; i < argumentCount; i++)
    {
        call_args.append(JSValueToVariant(ctx, arguments[i]));
    }
    Variant return_value = callable.callv(call_args);

    return VariantToJSValue(ctx, &return_value);
}

void HtmlRect::StoreGlobalObject(JSContextRef context, GodotObj obj)
{
    JSObjectRef godot_obj = JSObjectMake(context, NULL, NULL);

    for(auto iter = obj.begin(); iter != obj.end(); iter++)
    {
        auto key = iter->first;
        auto value = iter->second;

        JSRetainPtr<JSStringRef> js_key = adopt(JSStringCreateWithUTF8CString(key.utf8().get_data()));
        JSValueRef js_value = VariantToJSValue(context, value.get());
        JSObjectSetProperty(context, godot_obj, js_key.get(), js_value, 0, 0);
    }
    
    // Get the global JavaScript object (aka 'window')
    JSObjectRef globalObj = JSContextGetGlobalObject(context);

    // Create a JavaScript String containing the name of our callback.
    JSRetainPtr<JSStringRef> propertyName = adopt(JSStringCreateWithUTF8CString("godot"));

    // Store our function in the page's global JavaScript object.
    JSObjectSetProperty(context, globalObj, propertyName.get(), godot_obj, 0, 0);

}

Variant HtmlRect::JSValueToVariant(JSContextRef context, JSValueRef value)
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

JSValueRef HtmlRect::VariantToJSValue(JSContextRef context, Variant* variant)
{
    switch(variant->get_type())
    {
        case Variant::BOOL:
            return JSValueMakeBoolean(context, *variant);
        case Variant::INT:
        case Variant::FLOAT:
            return JSValueMakeNumber(context, *variant);
        case Variant::STRING: {
            JSRetainPtr<JSStringRef> js_string = adopt(
                JSStringCreateWithUTF8CString(
                    ((godot::String)*variant).utf8().get_data()
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

            //Create a function callback entry that references the class we created for the private data
            JSObjectRef nativeFunc = JSObjectMake(context, classReference.get(), variant);

            return nativeFunc;
        }
        case Variant::DICTIONARY:
        case Variant::ARRAY:
        case Variant::OBJECT:
            return JSValueMakeUndefined(context);
        default: {
            JSRetainPtr<JSStringRef> js_string = adopt(
                JSStringCreateWithUTF8CString(
                    variant->stringify().utf8().get_data()
                )
            );
            return JSValueMakeString(context, js_string.get());
        }
    }

}

GodotObj HtmlRect::ConvertDictionaryToMap(Dictionary dict)
{
    GodotObj map;
    Array keys = dict.keys();
    for(int i = 0; i < keys.size(); i++)
    {
        std::shared_ptr<Variant> variant_ptr = std::make_shared<Variant>(dict[keys[i]]);
        map[keys[i]] = variant_ptr;
    }
    return map;
}

#pragma endregion
