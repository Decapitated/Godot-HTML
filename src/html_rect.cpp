#include "html_rect.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

using namespace godot;
// using namespace ultralight;

RefPtr<Renderer> HtmlRect::renderer = RefPtr<Renderer>();

void HtmlRect::_bind_methods() {
    ClassDB::bind_method(D_METHOD("render_frame"), &HtmlRect::RenderFrame);
    ClassDB::bind_method(D_METHOD("size_changed"), &HtmlRect::SizeChanged);

	ClassDB::bind_method(D_METHOD("get_index"), &HtmlRect::get_index);
	ClassDB::bind_method(D_METHOD("set_index", "p_amplitude"), &HtmlRect::set_index);
	ClassDB::add_property("HtmlRect", PropertyInfo(Variant::STRING, "index_path"), "set_index", "get_index");
}

HtmlRect::HtmlRect() {}

HtmlRect::~HtmlRect() {
    if(file_system != nullptr)
    {
	    delete file_system;
    }
}

void HtmlRect::_process(double delta)
{
    if(!is_setup)
    {
        Setup();
        is_setup = true;
    }
    else UpdateLogic();
}

void HtmlRect::Setup()
{
    image = Ref<Image>();

    InitPlatform();
    if(!renderer) CreateRenderer();
    CreateView();

    RenderingServer::get_singleton()->connect("frame_post_draw", Callable(this, "render_frame"));
    connect("resized", Callable(this, "size_changed"));
}

void HtmlRect::UpdateLogic()
{
    ///
    /// Give the library a chance to handle any pending tasks and timers.
    ///
    ///
    renderer->Update();
}

void HtmlRect::RenderFrame()
{
    ///
    /// Render all active Views (this updates the Surface for each View).
    ///
    renderer->Render();
    ///
    /// Get the Surface as a BitmapSurface (the default implementation).
    ///
    BitmapSurface* surface = (BitmapSurface*)(view->surface());
    ///
    /// Check if our Surface is dirty (pixels have changed).
    ///
    if (!surface->dirty_bounds().IsEmpty()) {
        ///
        /// Psuedo-code to upload Surface's bitmap to GPU texture.
        ///
        CopyBitmapToTexture(surface->bitmap());
        
        ///
        /// Clear the dirty bounds.
        ///
        surface->ClearDirtyBounds();
    }
}

void HtmlRect::SizeChanged()
{
    Vector2 size = get_size();
    view->Resize((int)size.x, (int)size.y);
    image = Ref<Image>();
}

void HtmlRect::InitPlatform()
{
    ultralight::Config config;
    config.resource_path_prefix = "gdexample/resources/";
    Platform::instance().set_config(config);

    ///
    /// Use the OS's native font loader
    ///
    Platform::instance().set_font_loader(GetPlatformFontLoader());

    file_system = new GodotFileSystem();
    Platform::instance().set_file_system(file_system);

    ///
    /// Use the default logger (writes to a log file)
    ///
    Platform::instance().set_logger(GetDefaultLogger("ultralight.log"));
}

void HtmlRect::CreateRenderer()
{
    ///
    /// Create our Renderer (call this only once per application).
    /// 
    /// The Renderer singleton maintains the lifetime of the library
    /// and is required before creating any Views.
    ///
    /// You should set up the Platform handlers before this.
    ///
    renderer = Renderer::Create();
}

void HtmlRect::CreateView()
{
    Vector2 size = get_size();
    ///
    /// Configure our View, make sure it uses the CPU renderer by
    /// disabling acceleration.
    ///
    ViewConfig view_config;
    view_config.is_accelerated = false;
    view_config.is_transparent = true;

    ///
    /// Create an HTML view, 500 by 500 pixels large.
    ///
    view = renderer->CreateView((int)size.x, (int)size.y, view_config, nullptr);
    
    if(!LoadIndex()) view->LoadHTML("<h1>Placeholder Text</h1>");
}

bool HtmlRect::LoadIndex()
{
    if(index_path.is_empty())
        return false;
    view->LoadURL(("file:///"+index_path).utf8().get_data());
    return true;
}

void HtmlRect::CopyBitmapToTexture(RefPtr<Bitmap> bitmap)
{
    bitmap->SwapRedBlueChannels();
    auto pixels = bitmap->LockPixelsSafe();
    PackedByteArray arr = PackedByteArray();
    arr.resize(pixels.size());
    memcpy(arr.ptrw(), pixels.data(), pixels.size());
    if(image.is_null())
    {
        image = Image::create_from_data(bitmap->width(), bitmap->height(), false, Image::FORMAT_RGBA8, arr);
        image_texture = ImageTexture::create_from_image(image);
        set_texture(image_texture);
    }
    else
    {
        image->set_data(bitmap->width(), bitmap->height(), false, Image::Format::FORMAT_RGBA8, arr); 
        image_texture->update(image);
    }
}

void HtmlRect::set_index(const String p_index) {
	index_path = p_index;
}

godot::String HtmlRect::get_index() const {
	return index_path;
}