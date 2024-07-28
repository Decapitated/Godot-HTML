#ifndef APPEXAMPLE_H
#define APPEXAMPLE_H

#include "html_rect.hpp"

namespace godot {

    class AppExample : public HtmlRect {
        GDCLASS(AppExample, HtmlRect)

        private:
            static JSValueRef ExampleCallback(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);

        protected:
            static void _bind_methods();

            void SetupJS(JSContextRef context) override;
    };

}

#endif