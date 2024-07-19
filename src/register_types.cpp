#include "register_types.hpp"

#include "ultralight_manager.hpp"

#include "gdexample.hpp"
#include "html_rect.hpp"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "windows.h"

using namespace godot;

// Debug console.
FILE* f;

GodotHTML::UManager* manager;

void initialize_html_module(ModuleInitializationLevel p_level) {
    std::cout << "(" << p_level << ") Initializing Godot HTML." << std::endl;
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

    manager = new GodotHTML::UManager();

    ClassDB::register_class<HtmlRect>();
}

void uninitialize_html_module(ModuleInitializationLevel p_level) {
    std::cout << "(" << p_level << ") Uninitializing Godot HTML." << std::endl;
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

    delete manager;
}

extern "C" {
    // Initialization.
    GDExtensionBool GDE_EXPORT html_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
        // #region Initialize console.
            AllocConsole();
            freopen_s(&f, "CONOUT$", "w", stdout);
        // #endregion


        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_html_module);
        init_obj.register_terminator(uninitialize_html_module);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
        return init_obj.init();
    }
}