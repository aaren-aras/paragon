#include "main_menu.h"
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

namespace godot {

    MainMenu::MainMenu() {}
    MainMenu::~MainMenu() {}

    void MainMenu::_bind_methods() {
        ClassDB::bind_method(D_METHOD("_on_new_game_pressed"), &MainMenu::_on_new_game_pressed);
        ClassDB::bind_method(D_METHOD("_on_quit_pressed"), &MainMenu::_on_quit_pressed);
    }

    void MainMenu::_ready() {
        // Title label
        Label* title = memnew(Label);
        title->set_text("PARAGON");
        title->set_horizontal_alignment(HORIZONTAL_ALIGNMENT_CENTER);

        // Buttons
        btn_new_game = memnew(Button);
        btn_new_game->set_text("New Game");

        btn_continue = memnew(Button);
        btn_continue->set_text("Continue");
        btn_continue->set_disabled(true);

        btn_quit = memnew(Button);
        btn_quit->set_text("Quit");

        // Layout
        VBoxContainer* vbox = memnew(VBoxContainer);
        vbox->set_anchors_preset(Control::PRESET_CENTER);
        vbox->add_child(title);
        vbox->add_child(btn_new_game);
        vbox->add_child(btn_continue);
        vbox->add_child(btn_quit);

        add_child(vbox);

        // Connect signals
        btn_new_game->connect("pressed", Callable(this, "_on_new_game_pressed"));
        btn_quit->connect("pressed", Callable(this, "_on_quit_pressed"));

        UtilityFunctions::print("PARAGON: MainMenu ready.");
    }

    void MainMenu::_on_new_game_pressed() {
        UtilityFunctions::print("PARAGON: Starting new game...");
        get_tree()->change_scene_to_file("res://scenes/test_level.tscn");
    }

    void MainMenu::_on_quit_pressed() {
        get_tree()->quit();
    }

} // namespace godot