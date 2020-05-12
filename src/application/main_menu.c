#include <dirent.h>
#include "simple_logger.h"
#include "main_menu.h"
#include "gf2d_ui.h"
#include "gf2d_scene.h"
#include "gf2d_application.h"

List *main_menu_ui_list = NULL;

UIComponent *main_menu_ui[3];
List *pre_editor_ui = NULL;

void start_game(Button *self);
void open_editor(Button *self);
void quit_game(Button *self);
void open_tilemap_file(Button *self);

void foreach_button(void *self, void *context)
{
    if(!self) return;
    gf2d_ui_update((UIComponent*)self);
}

void update_ui_event_manager(Entity *self)
{
    gfc_list_foreach(main_menu_ui_list, foreach_button, NULL);
}

void main_menu_awake()
{
    UIComponent *startBtn = NULL;
    UIComponent *editorBtn = NULL;
    UIComponent *quitBtn = NULL;
    Entity *ui_event_manager = NULL;
    DIR *dir = NULL;
    struct dirent *de = NULL;
    UIComponent *editorBtnBuffer = NULL;
    float fileCount = 0.0f;

    /* Create buttons for main menu */
    startBtn = gf2d_ui_button_new(
        gf2d_label_new("Start Game", NULL, 32, vector2d(550.0f, 275.0f)),
        NULL
    );
    gf2d_label_set_text_color(startBtn->component.btn->text, vector4d(0.0f, 0.0f, 0.0f, 255.0f));
    gf2d_scene_add_to_drawables(startBtn, DET_UI);
    startBtn->component.btn->onClick = start_game;

    editorBtn = gf2d_ui_button_new(
        gf2d_label_new("Editor", NULL, 32, vector2d(575.0f, 339.0f)),
        NULL
    );
    gf2d_label_set_text_color(editorBtn->component.btn->text, vector4d(0.0f, 0.0f, 0.0f, 255.0f));
    gf2d_scene_add_to_drawables(editorBtn, DET_UI);
    editorBtn->component.btn->onClick = open_editor;

    quitBtn = gf2d_ui_button_new(
        gf2d_label_new("Quit", NULL, 32, vector2d(580.0f, 403.0f)),
        NULL
    );
    gf2d_label_set_text_color(quitBtn->component.btn->text, vector4d(0.0f, 0.0f, 0.0f, 255.0f));
    gf2d_scene_add_to_drawables(quitBtn, DET_UI);
    quitBtn->component.btn->onClick = quit_game;

    /* create event manager to handle ui update */
    ui_event_manager = gf2d_entity_new("ui eve");
    ui_event_manager->update = update_ui_event_manager;
    
    /* create ui list and append all created ui components */
    main_menu_ui_list = gfc_list_new_size(16);
    gfc_list_append(main_menu_ui_list, startBtn);
    gfc_list_append(main_menu_ui_list, editorBtn);
    gfc_list_append(main_menu_ui_list, quitBtn);

    dir = opendir("application/scenes/");
    if(!dir)
    {
        slog("failed to open directory");
        return;
    }

    while( (de = readdir(dir)) != NULL )
    {
        if( strstr(de->d_name, "tilemap.json") != NULL )
        {
            slog("%s", de->d_name);
            editorBtnBuffer = gf2d_ui_button_new(
                gf2d_label_new(de->d_name, NULL, 32, vector2d(10.0f, 100.0f + 64.0f * fileCount)),
                open_tilemap_file
            );
            gf2d_label_set_text_color(editorBtnBuffer->component.btn->text, vector4d(0.0f, 0.0f, 0.0f, 255.0f));
            gf2d_scene_add_to_drawables(editorBtnBuffer, DET_UI);
            gfc_list_append(main_menu_ui_list, editorBtnBuffer);

            fileCount++;
        }
    }

    closedir(dir);
}

void start_game(Button *self)
{
    if(!self) return;   

    gf2d_scene_load_from_file("application/scenes/first_scene.json");
}

void open_editor(Button *self)
{
    if(!self) return;   
    slog("open editor");
}

void quit_game(Button *self)
{
    if(!self) return;

    slog("quit game");
    gf2d_close_application();
}

void open_tilemap_file(Button *self)
{
    if(!self) return;

    slog("%s", self->text->_text);
}