#ifndef HTMLRECT_H
#define HTMLRECT_H

#include "godot/view_rect/view_rect.hpp"
#include "convert/convert.hpp"

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/gdvirtual.gen.inc>

namespace godot {
    class HtmlRect : public ViewRect {
        GDCLASS(HtmlRect, ViewRect);

        private:
            void CreateView();
            void StoreGlobalObject(JSContextRef context, Dictionary obj);

        protected:
            static void _bind_methods();
            
            Dictionary call_on_dom_ready(const String &url);
            GDVIRTUAL1R(Dictionary, _on_dom_ready, String);
            virtual Dictionary _on_dom_ready(const String &url) { return Dictionary(); };

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