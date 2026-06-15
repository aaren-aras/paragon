#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/v_box_container.hpp>
#include <godot_cpp/classes/label.hpp>

namespace godot {

    class MainMenu : public Control {
        GDCLASS(MainMenu, Control)

    protected:
        static void _bind_methods();

    private:
        Button* btn_new_game;
        Button* btn_continue;
        Button* btn_quit;

        void _on_new_game_pressed();
        void _on_quit_pressed();

    public:
        MainMenu();
        ~MainMenu();

        void _ready() override;
    };

} // namespace godot

#endif