#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

#include "player.h"
#include "boss.h"

using namespace godot;

void initialize_paragon(ModuleInitializationLevel level) {
    if (level != MODULE_INITIALIZATION_LEVEL_SCENE) return;

    ClassDB::register_class<Player>();
    ClassDB::register_class<Boss>();
}

void uninitialize_paragon(ModuleInitializationLevel level) {}

extern "C" {
GDExtensionBool GDE_EXPORT paragon_library_init(
    GDExtensionInterfaceGetProcAddress get_proc_address,
    GDExtensionClassLibraryPtr library,
    GDExtensionInitialization *initialization
) {
    godot::GDExtensionBinding::InitObject init_obj(
        get_proc_address, library, initialization
    );
    init_obj.register_initializer(initialize_paragon);
    init_obj.register_terminator(uninitialize_paragon);
    init_obj.set_minimum_library_initialization_level(
        MODULE_INITIALIZATION_LEVEL_SCENE
    );
    return init_obj.init();
}
}
