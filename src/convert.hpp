#ifndef CONVERT_H
#define CONVERT_H

#include "godot/html_rect/html_rect.hpp"

#include <AppCore/JSHelpers.h>
#include <JavaScriptCore/JSRetainPtr.h>

using namespace godot;
using namespace ultralight;

class Convert {
public:
    static Variant ToVariant(JSContextRef context, JSValueRef value)
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

    static JSValueRef ToJSValue(JSContextRef context, Variant* variant)
    {
        switch(variant->get_type())
        {
            case Variant::NIL:
                return JSValueMakeNull(context);
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
                classFunctionDefinition.callAsFunction = HtmlRect::CallableCallback;
                // Create a class reference from the class function definition.
                JSRetainPtr<JSClassRef> classReference = adopt(JSClassCreate(&classFunctionDefinition));

                //Create a function callback entry that references the class we created for the private data
                JSObjectRef nativeFunc = JSObjectMake(context, classReference.get(), variant);

                return nativeFunc;
            }
            // Leave these here to be implemented later.
            case Variant::DICTIONARY:
            case Variant::ARRAY:
            case Variant::OBJECT:
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
};

#endif