#include "register_types.hpp"

#include "ultralight_manager.hpp"

#include "view_rect.hpp"
#include "html_rect.hpp"
#include "inspector_rect.hpp"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "windows.h"

using namespace godot;

// Debug console.
FILE* f;

GodotHTML::UManager* manager;

void initialize_html_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
    // #region Initialize console.
        AllocConsole();
        freopen_s(&f, "CONOUT$", "w", stdout);
    // #endregion

    std::cout << "Initializing Godot HTML." << std::endl;

    manager = new GodotHTML::UManager();

    ClassDB::register_abstract_class<ViewRect>();
    ClassDB::register_class<HtmlRect>();
    ClassDB::register_class<InspectorRect>();
}

void uninitialize_html_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

    std::cout << "Uninitializing Godot HTML." << std::endl;
    delete manager;

    // #region Cleanup console.
        if(f) fclose(f);
        FreeConsole();
    // #endregion
}

extern "C" {
    // Initialization.
    GDExtensionBool GDE_EXPORT html_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address, const GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_html_module);
        init_obj.register_terminator(uninitialize_html_module);
        init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);
        return init_obj.init();
    }
}