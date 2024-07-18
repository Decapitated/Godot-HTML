#include "html_rect.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

#include <godot_cpp/classes/input_event_mouse.hpp>
#include <godot_cpp/classes/input_event_mouse.hpp>
#include <godot_cpp/classes/input_event_mouse_motion.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>
#include <godot_cpp/classes/input_event_key.hpp>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

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
        file_system = nullptr;
    }
}

void HtmlRect::_process(double delta)
{
    if(!is_setup)
    {
        Setup();
        is_setup = true;
    }
    else
    {
        UpdateLogic();
        for (; !events.empty(); events.pop())
        {
            switch(events.front())
            {
                case Event::mouse:
                    view->FireMouseEvent(mouse_events.front());
                    mouse_events.pop();
                    break;
                case Event::key:
                    view->FireKeyEvent(key_events.front());
                    key_events.pop();
                    break;
                case Event::scroll:
                    view->FireScrollEvent(scroll_events.front());
                    scroll_events.pop();
                    break;
            }
        }
    }
}

void HtmlRect::_gui_input(const Ref<InputEvent> &event)
{
    if(auto e = dynamic_cast<InputEventMouseButton*>(*event))
    {
        MouseButton index = e->get_button_index();
        // Check if scroll.
        if(index == MouseButton::MOUSE_BUTTON_WHEEL_UP || index == MouseButton::MOUSE_BUTTON_WHEEL_DOWN)
        {
            ScrollEvent evt;
            evt.type = ScrollEvent::kType_ScrollByPixel;
            int delta = index == MouseButton::MOUSE_BUTTON_WHEEL_UP ? -1 : 1;
            evt.delta_x = delta * 10;
            evt.delta_y = delta * 10;
            scroll_events.push(evt);
            events.push(Event::scroll);
        }
        else if(index < 4)
        {
            auto pos = e->get_position();
            MouseEvent evt;
            evt.x = (int)pos.x;
            evt.y = (int)pos.y;
            evt.type = e->is_pressed() ? MouseEvent::kType_MouseDown : MouseEvent::kType_MouseUp;
            switch(index)
            {
                case MouseButton::MOUSE_BUTTON_LEFT:
                    evt.button = MouseEvent::Button::kButton_Left;
                    break;
                case MouseButton::MOUSE_BUTTON_MIDDLE:
                    evt.button = MouseEvent::Button::kButton_Middle;
                    break;
                case MouseButton::MOUSE_BUTTON_RIGHT:
                    evt.button = MouseEvent::Button::kButton_Right;
                    break;
            };
            mouse_events.push(evt);
            events.push(Event::mouse);
        }
    }
    else if(auto e = dynamic_cast<InputEventMouseMotion*>(*event))
    {
        auto pos = e->get_position();
        MouseEvent evt;
        evt.type = MouseEvent::kType_MouseMoved;
        evt.x = (int)pos.x;
        evt.y = (int)pos.y;
        evt.button = MouseEvent::kButton_None;
        mouse_events.push(evt);
        events.push(Event::mouse);
    }
    else if(auto e = dynamic_cast<InputEventKey*>(*event))
    {
        UtilityFunctions::print("Key");
        // UtilityFunctions::print(e->as_text_physical_keycode());
        // KeyEvent evt;
        // evt.type = KeyEvent::kType_RawKeyDown;
        // evt.virtual_key_code = KeyCodes::GK_RIGHT;
        // evt.native_key_code = 0;
        // evt.modifiers = 0;

        // // You'll need to generate a key identifier from the virtual key code
        // // when synthesizing events. This function is provided in KeyEvent.h
        // GetKeyIdentifierFromVirtualKeyCode(evt.virtual_key_code, evt.key_identifier);

        // view->FireKeyEvent(evt);
    }
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
    config.resource_path_prefix = "gdhtml/resources/";
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

    ViewConfig view_config;
    view_config.is_accelerated = false;
    view_config.is_transparent = true;

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
