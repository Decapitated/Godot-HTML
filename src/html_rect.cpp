#include "html_rect.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void HtmlRect::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("get_index"), &HtmlRect::get_index);
	ClassDB::bind_method(D_METHOD("set_index", "p_index"), &HtmlRect::set_index);
	ClassDB::add_property("HtmlRect", PropertyInfo(Variant::STRING, "index_path"), "set_index", "get_index");
}

HtmlRect::HtmlRect()
{
    CreateView();
}

HtmlRect::~HtmlRect() { }

void HtmlRect::CreateView()
{
    Vector2 size = get_size();

    ViewConfig view_config;
    view_config.is_accelerated = false;
    view_config.is_transparent = true;

    RefPtr<View> view = UltralightSingleton::get_singleton()->CreateView((int)size.x, (int)size.y, view_config, nullptr);

    if(view)
    { 
        SetView(view);
        if(!LoadIndex(view)) view->LoadHTML("<h1>Placeholder Text</h1>");
    }
    
}

bool HtmlRect::LoadIndex(RefPtr<View> view)
{
    if(index_path.is_empty()) return false;
    auto path_parts = index_path.split("://");
    if(path_parts.size() == 1) 
    {
        view->LoadURL(("file:///"+index_path).utf8().get_data());
    }
    else
    {
        view->LoadURL(index_path.utf8().get_data());
    }
    return true;
}

void HtmlRect::set_index(const String p_index)
{
    String currentPath = index_path;
	index_path = p_index;
    if(!LoadIndex(GetView())) index_path = currentPath;
}

godot::String HtmlRect::get_index() const
{
	return index_path;
}

void HtmlRect::OnDOMReady(ultralight::View *caller, uint64_t frame_id, bool is_main_frame, const ultralight::String &url)
{
    // Acquire the JS execution context for the current page.
    auto scoped_context = caller->LockJSContext();
    
    // Typecast to the underlying JSContextRef.
    JSContextRef context = (*scoped_context);
    
    SetupJS(context);
}
