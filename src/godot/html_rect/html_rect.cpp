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

HtmlRect::~HtmlRect() { }

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

Dictionary HtmlRect::_on_dom_ready_call(const String &url)
{
    Dictionary obj;
    if(GDVIRTUAL_CALL(_on_dom_ready, url, obj))
    {
        return obj;
    }
    return _on_dom_ready(url);
}

JSValueRef CallableCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
{
    UtilityFunctions::print("Callback");
    Callable callable = (Variant)(GDExtensionVariantPtr)JSObjectGetPrivate(function);
    UtilityFunctions::print(callable);
    return JSValueMakeNull(ctx);
}

JSValueRef VariantToJSValue(JSContextRef context, Variant var)
{
    UtilityFunctions::print(var.get_type());
    switch(var.get_type())
    {
        case Variant::BOOL:
            return JSValueMakeBoolean(context, var);
        case Variant::INT:
        case Variant::FLOAT:
            return JSValueMakeNumber(context, var);
        case Variant::STRING: {
            JSRetainPtr<JSStringRef> js_string = adopt(
                JSStringCreateWithUTF8CString(
                    ((godot::String)var).utf8().get_data()
                )
            );
            return JSValueMakeString(context, js_string.get());
        }
        case Variant::CALLABLE: {
            Callable callable = var;
            JSRetainPtr<JSStringRef> method_name = adopt(
                JSStringCreateWithUTF8CString(
                    ((godot::String)callable.get_method()).utf8().get_data()
                )
            );
            JSObjectRef func = JSObjectMakeFunctionWithCallback(context, method_name.get(), CallableCallback);
            JSObjectSetPrivate(func, callable._native_ptr());
            return func;
        }
        default:
            return JSValueMakeNull(context);
    }

}

void StoreGlobalObject(JSContextRef context, Dictionary obj)
{
    JSObjectRef godot_obj = JSObjectMake(context, NULL, NULL);

    Array keys = obj.keys();
    for(int i = 0; i < keys.size(); i++)
    {
        Variant key = keys[i];
        Variant value = obj[keys[i]];

        JSRetainPtr<JSStringRef> js_key = adopt(JSStringCreateWithUTF8CString(key.stringify().utf8().get_data()));
        JSValueRef js_value = VariantToJSValue(context, value);
        JSObjectSetProperty(context, godot_obj, js_key.get(), js_value, 0, 0);
    }
    
    // Get the global JavaScript object (aka 'window')
    JSObjectRef globalObj = JSContextGetGlobalObject(context);

    // Create a JavaScript String containing the name of our callback.
    JSRetainPtr<JSStringRef> propertyName = adopt(JSStringCreateWithUTF8CString("godot"));

    // Store our function in the page's global JavaScript object.
    JSObjectSetProperty(context, globalObj, propertyName.get(), godot_obj, 0, 0);

}

void HtmlRect::OnDOMReady(ultralight::View *caller, uint64_t frame_id, bool is_main_frame, const ultralight::String &url)
{
    // Acquire the JS execution context for the current page.
    auto scoped_context = caller->LockJSContext();
    
    // Typecast to the underlying JSContextRef.
    JSContextRef context = (*scoped_context);
    
    String godot_url = godot::String(url.utf8().data());
    Dictionary obj = _on_dom_ready_call(godot_url);

    StoreGlobalObject(context, obj);
}
