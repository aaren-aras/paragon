#include "boss.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

    Boss::Boss() {}
    Boss::~Boss() {}

    void Boss::_bind_methods() {}

    void Boss::_ready() {
        UtilityFunctions::print("PARAGON: Boss C++ node ready.");
    }

} // namespace godot