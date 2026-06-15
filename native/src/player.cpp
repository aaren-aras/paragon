#include "player.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

namespace godot {

    Player::Player() {}
    Player::~Player() {}

    void Player::_bind_methods() {}

    void Player::_ready() {
        UtilityFunctions::print("PARAGON: Player C++ node ready.");
    }

} // namespace godot