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

        SubViewport* viewport = get_node<SubViewport>(
            "SubViewportContainer/SubViewport"
        );
        if (viewport == nullptr) {
            UtilityFunctions::print("PARAGON: ERROR - SubViewport not found!");
            return;
        }
        UtilityFunctions::print("PARAGON: SubViewport found!");

        model = viewport->get_node<Node3D>("CharacterBody3D");
        if (model == nullptr) {
            UtilityFunctions::print("PARAGON: ERROR - Model not found!");
        }
        else {
            UtilityFunctions::print("PARAGON: Model found!");
        }

        anim_player = viewport->get_node<AnimationPlayer>(
            "CharacterBody3D/UAL1_Standard/AnimationPlayer"
        );
        if (anim_player == nullptr) {
            UtilityFunctions::print("PARAGON: ERROR - AnimationPlayer not found!");
        }
        else {
            UtilityFunctions::print("PARAGON: AnimationPlayer found!");
        }
    }

    void Player::_physics_process(double delta) {
        //if (Engine::get_singleton()->is_editor_hint()) return;
        //UtilityFunctions::print("PARAGON: physics running");

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

        if (model != nullptr) {
            if (direction > 0.0f) {
                model->set_rotation(Vector3(0, Math::deg_to_rad(90.0f), 0));
            }
            else if (direction < 0.0f) {
                model->set_rotation(Vector3(0, Math::deg_to_rad(-90.0f), 0));
            }
        }

        if (anim_player != nullptr) {
            if (Math::abs(velocity.x) > 1.0f && is_on_floor()) {
                if (anim_player->get_current_animation() != "Walk") {
                    anim_player->play("Walk");
                }
            }
            else if (is_on_floor()) {
                if (anim_player->get_current_animation() != "Idle") {
                    UtilityFunctions::print("PARAGON: Playing idle");
                    anim_player->play("Idle");
                    UtilityFunctions::print(String("PARAGON: Current anim: ") + anim_player->get_current_animation());

                }
            }
            //else if (is_on_floor()) {
            //    if (anim_player->get_current_animation() != "RESET") {
            //        anim_player->play("RESET");

            //    }
            //}
        }

        set_velocity(velocity);
        move_and_slide();
    }

} // namespace godot