#ifndef BOSS_H
#define BOSS_H

#include <godot_cpp/classes/node2d.hpp>

namespace godot {

    class Boss : public Node2D {
        GDCLASS(Boss, Node2D)

    protected:
        static void _bind_methods();

    public:
        Boss();
        ~Boss();

        void _ready() override;
    };

} // namespace godot

#endif