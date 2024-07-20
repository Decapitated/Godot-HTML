#ifndef VIEWRECT_H
#define VIEWRECT_H

#include "ultralight_manager.hpp"

#include <queue>

#include <Ultralight/KeyEvent.h>
#include <Ultralight/MouseEvent.h>
#include <Ultralight/ScrollEvent.h>

#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/input_event.hpp>

using namespace ultralight;

namespace godot {

    class ViewRect : public TextureRect {
        GDCLASS(ViewRect, TextureRect);

        private:
            enum Event { mouse, key, scroll };
            std::queue<Event> events;
            std::queue<MouseEvent> mouse_events;
            std::queue<KeyEvent> key_events;
            std::queue<ScrollEvent> scroll_events;

            RefPtr<View> view; // Ultralight view
            Ref<ImageTexture> image_texture;
            Ref<Image> image;

            void UpdateLogic();
            void RenderFrame();
            void SizeChanged();

            void CopyBitmapToTexture(RefPtr<Bitmap> bitmap);

        protected:
            static void _bind_methods();

        public:
            ViewRect();
            ~ViewRect();

            void SetView(RefPtr<View> p_view);
            RefPtr<View> GetView();

            void _process(double delta) override;
            void _gui_input(const Ref<InputEvent> &event) override;
    };

}

#endif