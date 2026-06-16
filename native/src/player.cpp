#include "player.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/input.hpp>

namespace godot {

    Player::Player() {}
    Player::~Player() {}

    void Player::_bind_methods() {}

    void Player::_ready() {
        UtilityFunctions::print("PARAGON: Player ready!");
        anim_player = get_node<AnimationPlayer>("AnimationPlayer");
    }

    void Player::_physics_process(double delta) {
        if (Engine::get_singleton()->is_editor_hint()) return;

        Input* input = Input::get_singleton();
        Vector2 velocity = get_velocity();

        // Gravity
        if (!is_on_floor()) {
            velocity.y += gravity * (float)delta;
        }

        // Roll logic
        if (is_rolling) {
            roll_timer -= (float)delta;
            velocity.x = roll_speed * roll_direction;
            if (roll_timer <= 0.0f) {
                is_rolling = false;
            }
            set_velocity(velocity);
            move_and_slide();
            return;
        }

        // Horizontal movement
        float direction = 0.0f;
        if (input->is_action_pressed("move_right")) direction += 1.0f;
        if (input->is_action_pressed("move_left"))  direction -= 1.0f;
        velocity.x = direction * speed;

        // Jump
        if (input->is_action_just_pressed("jump") && is_on_floor()) {
            velocity.y = jump_velocity;
        }

        // Roll (tap shift)
        if (input->is_action_just_pressed("roll") && is_on_floor() && direction != 0.0f) {
            is_rolling = true;
            roll_timer = roll_duration;
            roll_direction = direction;
            UtilityFunctions::print("PARAGON: Roll!");
        }

        if (anim_player != nullptr) {
            if (Math::abs(velocity.x) > 1.0f && is_on_floor()) {
                if (anim_player->get_current_animation() != "walk") {
                    anim_player->play("walk");
                }
            }
            //else if (is_on_floor()) {
            //    if (anim_player->get_current_animation() != "idle") {
            //        UtilityFunctions::print("PARAGON: Playing idle");
            //        anim_player->play("idle");
            //    }
            //}
            else if (is_on_floor()) {
                if (anim_player->get_current_animation() != "RESET") {
                    anim_player->play("RESET");

                }
            }
        }

        set_velocity(velocity);
        move_and_slide();
    }

} // namespace godot