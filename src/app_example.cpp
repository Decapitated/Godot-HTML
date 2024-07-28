#include "app_example.hpp"

#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void AppExample::_bind_methods() {}

void AppExample::SetupJS(JSContextRef context)
{
    // Create a JavaScript String containing the name of our callback.
    JSStringRef name = JSStringCreateWithUTF8CString("ExampleCallback");

    // Create a garbage-collected JavaScript function that is bound to our
    // native C callback 'ExampleCallback()'.
    JSObjectRef func = JSObjectMakeFunctionWithCallback(context, name, ExampleCallback);

    // Get the global JavaScript object (aka 'window')
    JSObjectRef globalObj = JSContextGetGlobalObject(context);

    // Store our function in the page's global JavaScript object so that it
    // accessible from the page as 'ExampleCallback()'.
    JSObjectSetProperty(context, globalObj, name, func, 0, 0);

    // Release the JavaScript String we created earlier.
    JSStringRelease(name);
}

JSValueRef AppExample::ExampleCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
{
    UtilityFunctions::print("ExampleCallback called from JS!");
    return JSValueMakeNull(ctx);
}
