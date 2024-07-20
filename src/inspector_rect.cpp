#include "inspector_rect.hpp"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/rendering_server.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

class InspectorListener : public ViewListener
{
    private:
        InspectorRect *inspector;
    public:
        InspectorListener(InspectorRect *p_inspector)
        {
            inspector = p_inspector;
        }

        RefPtr<View> OnCreateInspectorView(ultralight::View* caller, bool is_local, const ultralight::String& inspected_url) override
        {
            Vector2 size = inspector->get_size();

            ViewConfig view_config;
            view_config.is_accelerated = false;
            view_config.is_transparent = true;

            RefPtr<View> view = GodotHTML::UManager::GetRenderer()->CreateView((int)size.x, (int)size.y, view_config, nullptr);
            if(view)
            {
                inspector->SetView(view);
                return view;
            }
            return nullptr;
        }
};

void InspectorRect::_bind_methods()
{
	ClassDB::bind_method(D_METHOD("get_html_rect"), &InspectorRect::get_html_rect);
	ClassDB::bind_method(D_METHOD("set_html_rect", "p_html_rect"), &InspectorRect::set_html_rect);
	ClassDB::add_property("InspectorRect", PropertyInfo(Variant::OBJECT, "html_rect", PROPERTY_HINT_NODE_TYPE), "set_html_rect", "get_html_rect");
}

InspectorRect::InspectorRect() { }

InspectorRect::~InspectorRect() { }

void InspectorRect::_process(double delta)
{
    ViewRect::_process(delta);
    if(!GetView())
    {
        if(html_rect != nullptr)
        {
            init(html_rect->GetView());
        }
    }
}

void InspectorRect::init(RefPtr<View> p_view)
{
    if(p_view)
    {
        InspectorListener* listener = new InspectorListener(this);
        p_view->set_view_listener(listener);
        p_view->CreateLocalInspectorView();
    }
}


void InspectorRect::set_html_rect(HtmlRect* p_html_rect)
{
    html_rect = p_html_rect;
}

HtmlRect* InspectorRect::get_html_rect()
{
    return html_rect;
}
