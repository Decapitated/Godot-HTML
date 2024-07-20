#include "inspector_rect.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void InspectorRect::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("get_html_rect"), &InspectorRect::get_html_rect);
	ClassDB::bind_method(D_METHOD("set_html_rect", "p_html_rect"), &InspectorRect::set_html_rect);
	ClassDB::add_property("InspectorRect", PropertyInfo(Variant::OBJECT, "html_rect", PROPERTY_HINT_NODE_TYPE), "set_html_rect", "get_html_rect");
}

InspectorRect::InspectorRect() { }

InspectorRect::~InspectorRect() { }

void InspectorRect::set_html_rect(HtmlRect* p_html_rect)
{
    html_rect = p_html_rect;
}

HtmlRect* InspectorRect::get_html_rect()
{
    return html_rect;
}
