#ifndef ULTRALIGHT_SINGLETON_H
#define ULTRALIGHT_SINGLETON_H

#include <godot_cpp/classes/object.hpp>

#include <godot_cpp/core/class_db.hpp>

#include "ultralight_manager.hpp"

namespace godot
{

class UltralightSingleton : public Object
{
	GDCLASS(UltralightSingleton, Object);

    private:
	    static UltralightSingleton *singleton;

        bool is_setup = false;

        GodotHTML::UltralightManager* manager = nullptr;

    protected:
        static void _bind_methods();

    public:
        static UltralightSingleton *get_singleton();

        UltralightSingleton();
        ~UltralightSingleton();

        void RenderFrame();

        RefPtr<View> CreateView(uint32_t width, uint32_t height, const ViewConfig& config, RefPtr<Session> session);
};

}

#endif