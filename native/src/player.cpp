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

        //anim_player = viewport->get_node<AnimationPlayer>(
        //    "CharacterBody3D/UAL1_Standard/AnimationPlayer"
        //);
        //if (anim_player == nullptr) {
        //    UtilityFunctions::print("PARAGON: ERROR - AnimationPlayer not found!");
        //}
        //else {
        //    UtilityFunctions::print("PARAGON: AnimationPlayer found!");
        //}
        anim_tree = viewport->get_node<AnimationTree>("CharacterBody3D/AnimationTree");
        if (anim_tree == nullptr) {
            UtilityFunctions::print("PARAGON: ERROR - AnimationTree not found!");
        }
        else {
            UtilityFunctions::print("PARAGON: AnimationTree found!");
        }
        //anim_tree->set("parameters/playback/current_node", "Idle");

    }

    void Player::_physics_process(double delta) {
        //if (Engine::get_singleton()->is_editor_hint()) return;
        //UtilityFunctions::print("PARAGON: physics running");

        if (Engine::get_singleton()->is_editor_hint()) return; // exit if in editor

        Input* input = Input::get_singleton(); // only runs during actual gameplay
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

        String current_anim = "";
        if (anim_tree != nullptr) {
            Variant pv = anim_tree->get("parameters/playback");
            AnimationNodeStateMachinePlayback* pb =
                Object::cast_to<AnimationNodeStateMachinePlayback>(pv);
            if (pb != nullptr) current_anim = pb->get_current_node();
        }

        bool recovering = (current_anim == "quaternius_Jump_Land" || current_anim == "quaternius_Roll");

        if (!recovering) {
            if (input->is_action_pressed("move_right")) direction += 1.0f;
            if (input->is_action_pressed("move_left"))  direction -= 1.0f;
        }
        velocity.x = recovering ? 0.0f : direction * speed;

        UtilityFunctions::print(String("state: ") + current_anim);

        //float direction = 0.0f;
        //if (input->is_action_pressed("move_right")) direction += 1.0f;
        //if (input->is_action_pressed("move_left"))  direction -= 1.0f;
        //velocity.x = direction * speed;

        // Sprint
        //if (input->is_action_just_pressed("jump") && is_on_floor()) {
        //    velocity.y = jump_velocity;
        //}


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

        if (anim_tree != nullptr) {
            Variant playback_variant = anim_tree->get("parameters/playback");
            AnimationNodeStateMachinePlayback* playback =
                Object::cast_to<AnimationNodeStateMachinePlayback>(playback_variant);

            if (playback == nullptr) {
                UtilityFunctions::print("PARAGON: playback became null mid-game!");
                return;
            }
            //if (playback != nullptr) {
            //    //UtilityFunctions::print(String("PARAGON: current node: ") + playback->get_current_node());
            //    
            //    // Initialize once
            //    if (!initialized) {
            //        playback->travel("Idle");
            //        initialized = true;
            //    }
 
            //}
            //else {
            //    UtilityFunctions::print("PARAGON: playback is null!");
            //}

            if (playback != nullptr) {
                
                //if (!initialized) {
                //    playback->start("quaternius_Idle");
                //    initialized = true;
                //}
                String current = playback->get_current_node();


                // Roll - uninterruptible
                if (is_rolling) {
                    if (current != "quaternius_Roll") playback->travel("quaternius_Roll");
                }
                // Airborne
                else if (!is_on_floor()) {
                    if (current == "quaternius_Jump_Start" || current == "quaternius_Jump") {
                        if (velocity.y >= 0 && current != "quaternius_Jump") {
                            playback->travel("quaternius_Jump");
                        }
                    }
                    else {
                        playback->travel("quaternius_Jump_Start");
                    }
                }
                // Just landed
                else if (is_on_floor() && (current == "quaternius_Jump" || current == "quaternius_Jump_Start")) {
                    playback->travel("quaternius_Jump_Land");
                }
                // Moving
                else if (is_on_floor() && Math::abs(velocity.x) > 1.0f) {
                    if (current != "quaternius_Roll" && current != "quaternius_Jump_Land") {
                        bool sprinting = input->is_action_pressed("sprint");
                        playback->travel(sprinting ? "quaternius_Sprint" : "quaternius_Walk");
                    }
                }
                // Idle
                else if (is_on_floor()) {
                    if (current != "quaternius_Roll" && current != "quaternius_Jump_Land") {
                        playback->travel("quaternius_Idle");
                    }
                }
            }
        }

        //if (anim_player != nullptr) {
        //    String current = anim_player->get_current_animation();

        //    // If a must-finish animation is still playing, don't interrupt it (unless player is walking or sprinting)
        //    //if (must_finish_anim) {
        //    //    bool actively_moving = ((Math::abs(velocity.x) > 1.0f) || velocity.y = jump_velocity);
        //    //    if (!anim_player->is_playing() || actively_moving) {
        //    //        must_finish_anim = false;
        //    //    }
        //    //    else {
        //    //        // Skip all other animation logic this frame
        //    //        set_velocity(velocity);
        //    //        move_and_slide();
        //    //        return;
        //    //    }
        //    //}
       

        //    if (must_finish_anim) {
        //        // Only movement can interrupt Jump_Land, never Roll
        //        bool can_interrupt = (current != "Roll") && Math::abs(velocity.x) > 1.0f && is_on_floor();
        //        if (!anim_player->is_playing() || can_interrupt) {
        //            must_finish_anim = false;
        //        }
        //        else {
        //            set_velocity(velocity);
        //            move_and_slide();
        //            return;
        //        }
        //    }

        //    // Rolling
        //    if (is_rolling) {
        //        if (current != "Roll") {
        //            anim_player->play("Roll");
        //            must_finish_anim = true;
        //        }
        //    }
        //    // In the air
        //    else if (!is_on_floor()) {
        //        if (velocity.y < 0) {
        //            // Rising
        //            if (current != "Jump_Start") anim_player->play("Jump_Start");
        //        }
        //        else {
        //            // Falling
        //            if (current != "Jump") anim_player->play("Jump");
        //        }
        //    }
        //    // Just landed
        //    else if (is_on_floor() && (current == "Jump" || current == "Jump_Start")) {
        //        anim_player->play("Jump_Land");
        //        must_finish_anim = true;
        //    }
        //    // Moving on floor
        //    else if (is_on_floor() && Math::abs(velocity.x) > 1.0f) {
        //        bool sprinting = input->is_action_pressed("sprint");
        //        if (sprinting) {
        //            if (current != "Sprint") anim_player->play("Sprint");
        //        }
        //        else {
        //            if (current != "Walk") anim_player->play("Walk");
        //        }
        //    }
        //    // Idle
        //    else if (is_on_floor()) {
        //        if (current != "Idle") anim_player->play("Idle");
        //    }

        //    //if (Math::abs(velocity.x) > 1.0f && is_on_floor()) {
        //    //    if (anim_player->get_current_animation() != "Walk") {
        //    //        anim_player->play("Walk");
        //    //    }
        //    //}
        //    //else if (is_on_floor()) {
        //    //    if (anim_player->get_current_animation() != "Idle") {
        //    //        UtilityFunctions::print("PARAGON: Playing idle");
        //    //        anim_player->play("Idle");
        //    //        UtilityFunctions::print(String("PARAGON: Current anim: ") + anim_player->get_current_animation());

        //    //    }
        //    //}
        //    //else if (is_on_floor()) {
        //    //    if (anim_player->get_current_animation() != "RESET") {
        //    //        anim_player->play("RESET");

        //    //    }
        //    //}
        //}

        set_velocity(velocity);
        move_and_slide();
    }

} // namespace godot