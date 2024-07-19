#ifndef INSPECTRECT_H
#define INSPECTRECT_H

#include <Ultralight/Ultralight.h>

#include <godot_cpp/classes/texture_rect.hpp>

using namespace ultralight;

namespace godot {

    class InspectorRect : public TextureRect {
        GDCLASS(InspectorRect, TextureRect)

        private:
            // void Setup();

        protected:
            static void _bind_methods();

        public:
            String index_path = "";
            InspectorRect();
            ~InspectorRect();

            void _process(double delta) override;

            void _gui_input(const Ref<InputEvent> &event) override;
    };

}

#endif