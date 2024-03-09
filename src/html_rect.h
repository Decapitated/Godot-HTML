#ifndef HTMLRECT_H
#define HTMLRECT_H

#include <Ultralight/Ultralight.h>
#include <AppCore/Platform.h>
#include <JavaScriptCore/JavaScript.h>

#include <godot_cpp/classes/texture_rect.hpp>
#include <godot_cpp/classes/image.hpp>
#include <godot_cpp/classes/image_texture.hpp>

#include "godot_file_system.h"

using namespace ultralight;

namespace godot {

class HtmlRect : public TextureRect {
	GDCLASS(HtmlRect, TextureRect)

	private:
        bool is_setup = false;

        Ref<ImageTexture> image_texture;
        Ref<Image> image;
        
        RefPtr<View> view; // Ultralight view

        GodotFileSystem* file_system = nullptr;

        void Setup();

        void InitPlatform();
		void CreateRenderer();
        void CreateView();

        void UpdateLogic();
        void RenderFrame();
        void SizeChanged();

        void CopyBitmapToTexture(RefPtr<Bitmap> bitmap);

	protected:
        static RefPtr<Renderer> renderer; // Ultralight renderer
		static void _bind_methods();

	public:
        String index_path = "";
		HtmlRect();
		~HtmlRect();

		void _process(double delta) override;

        bool LoadIndex();

        void set_index(const String p_index);
		String get_index() const;
};

}

#endif