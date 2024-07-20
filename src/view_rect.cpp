#include "view_rect.hpp"

using namespace godot;

void ViewRect::_bind_methods() {
    ClassDB::bind_method(D_METHOD("render_frame"), &ViewRect::RenderFrame);
    ClassDB::bind_method(D_METHOD("size_changed"), &ViewRect::SizeChanged);
}