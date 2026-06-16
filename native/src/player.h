#ifndef PLAYER_H
#define PLAYER_H

#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/animation_player.hpp>
#include <godot_cpp/classes/input.hpp>

namespace godot {

    class Player : public CharacterBody2D {
        GDCLASS(Player, CharacterBody2D)

    protected:
        static void _bind_methods();
    
    private:
        float speed = 200.0f;
        float jump_velocity = -400.0f;
        float gravity = 980.0f;

        bool is_rolling = false;
        float roll_speed = 450.0f;
        float roll_duration = 0.3f;
        float roll_timer = 0.0f;
        float roll_direction = 1.0f;

        AnimationPlayer* anim_player = nullptr;
    
    public:
        Player();
        ~Player();

        void _ready() override;
		void _physics_process(double delta) override;
    };

} // namespace godot

#endif