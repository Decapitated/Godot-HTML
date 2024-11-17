#include "html_rect.hpp"

#include "godot/ultralight_singleton/ultralight_singleton.hpp"
#include "convert.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <AppCore/JSHelpers.h>
#include <JavaScriptCore/JSRetainPtr.h>

using namespace godot;

#pragma region Utility Declarations

GodotObj ConvertDictionaryToMap(Dictionary dict);
void StoreGlobalObject(JSContextRef context, GodotObj obj);
JSValueRef JSThrowError(JSContextRef context, const char* message, JSValueRef* exception);

#pragma endregion

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

JSValueRef HtmlRect::CallableCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
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
    return Convert::ToJSValue(ctx, &return_value);
}

#pragma region Utilities

void StoreGlobalObject(JSContextRef context, GodotObj obj)
{
    JSObjectRef godot_obj = JSObjectMake(context, NULL, NULL);

    for(auto iter = obj.begin(); iter != obj.end(); iter++)
    {
        auto key = iter->first;
        auto value = iter->second;

        JSRetainPtr<JSStringRef> js_key = adopt(JSStringCreateWithUTF8CString(key.utf8().get_data()));
        JSValueRef js_value = Convert::ToJSValue(context, value.get());
        JSObjectSetProperty(context, godot_obj, js_key.get(), js_value, 0, 0);
    }
    
    // Get the global JavaScript object (aka 'window')
    JSObjectRef globalObj = JSContextGetGlobalObject(context);

    // Create a JavaScript String containing the name of our callback.
    JSRetainPtr<JSStringRef> propertyName = adopt(JSStringCreateWithUTF8CString("godot"));

    // Store our function in the page's global JavaScript object.
    JSObjectSetProperty(context, globalObj, propertyName.get(), godot_obj, 0, 0);
}

GodotObj ConvertDictionaryToMap(Dictionary dict)
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

JSValueRef JSThrowError(JSContextRef context, const char* message, JSValueRef* exception)
{
    JSRetainPtr<JSStringRef> errorMessage = adopt(JSStringCreateWithUTF8CString(message));
    *exception = JSValueMakeString(context, errorMessage.get());
    return JSValueMakeUndefined(context);
}

#pragma endregion