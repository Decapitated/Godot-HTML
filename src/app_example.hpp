#ifndef APPEXAMPLE_H
#define APPEXAMPLE_H

#include "godot/html_rect/html_rect.hpp"

namespace godot {

    class AppExample : public HtmlRect {
        GDCLASS(AppExample, HtmlRect)

        protected:
            static void _bind_methods();
            
            Dictionary _on_window_ready(const String &url) override;
    };

}

#endif