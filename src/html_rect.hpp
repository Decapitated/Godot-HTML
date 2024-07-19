#ifndef HTMLRECT_H
#define HTMLRECT_H

#include "ultralight_manager.hpp"

#include <queue>

#include <Ultralight/Ultralight.h>
#include <AppCore/Platform.h>
#include <JavaScriptCore/JavaScript.h>

#include <Ultralight/KeyEvent.h>
#include <Ultralight/MouseEvent.h>
#include <Ultralight/ScrollEvent.h>

#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>
#include <godot_cpp/classes/input_event.hpp>

using namespace ultralight;

namespace godot {

    class HtmlRect : public TextureRect {
        GDCLASS(HtmlRect, TextureRect)

        private:
            enum Event { mouse, key, scroll };
            std::queue<Event> events;
            std::queue<MouseEvent> mouse_events;
            std::queue<KeyEvent> key_events;
            std::queue<ScrollEvent> scroll_events;

            Ref<ImageTexture> image_texture;
            Ref<Image> image;
            
            RefPtr<View> view; // Ultralight view

            void CreateView();

            void UpdateLogic();
            void RenderFrame();
            void SizeChanged();

            void CopyBitmapToTexture(RefPtr<Bitmap> bitmap);

        protected:
            static void _bind_methods();

        public:
            String index_path = "";
            HtmlRect();
            ~HtmlRect();

            void _process(double delta) override;

            void _gui_input(const Ref<InputEvent> &event) override;

            bool LoadIndex();

            void set_index(const String p_index);
            String get_index() const;
    };

}

#endif