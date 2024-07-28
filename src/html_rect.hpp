#ifndef HTMLRECT_H
#define HTMLRECT_H

#include "view_rect.hpp"

namespace godot {

    class HtmlRect : public ViewRect {
        GDCLASS(HtmlRect, ViewRect)

        private:
            void CreateView();

        protected:
            static void _bind_methods();
            
            virtual void SetupJS(JSContextRef context) {};

        public:
            String index_path = "";
            HtmlRect();
            ~HtmlRect();

            // void _process(double delta) override;
            // void _gui_input(const Ref<InputEvent> &event) override;

            bool LoadIndex(RefPtr<View> view);

            void set_index(const String p_index);
            String get_index() const;

            void OnDOMReady(ultralight::View *caller, uint64_t frame_id, bool is_main_frame, const ultralight::String &url) override;
    };

}

#endif