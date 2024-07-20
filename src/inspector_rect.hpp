#ifndef INSPECTRECT_H
#define INSPECTRECT_H

#include "view_rect.hpp"

#include "html_rect.hpp"

#include <godot_cpp/variant/node_path.hpp>

namespace godot {

    class InspectorRect : public ViewRect {
        GDCLASS(InspectorRect, ViewRect)

        protected:
            static void _bind_methods();

        public:
            NodePath html_rect;

            // InspectorRect();
            // ~InspectorRect();

            void _process(double delta) override;
            // void _gui_input(const Ref<InputEvent> &event) override;

            void init(RefPtr<View> p_view);

            void set_html_rect(NodePath p_html_rect);
            NodePath get_html_rect();
    };

}

#endif