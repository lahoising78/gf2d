#include "smh.h"
#include "simple_logger.h"

#include "gf2d_draw.h"
#include "gf2d_input.h"
#include "gf2d_scene.h"
#include "gf2d_physics_entity.h"
#include "gf2d_tilemap.h"
#include "gf2d_camera.h"
#include "gf2d_ui.h"

#include "gf2d_trie.h"

UIComponent *ui = NULL;

float hor = 0.0f;
float ver = 0.0f;

void somethink(Entity *self)
{
    if ( gf2d_input_key_released(SDL_SCANCODE_A) || gf2d_input_key_released(SDL_SCANCODE_D) )
        hor = 0.0f;
    
    if( gf2d_input_key_just_pressed(SDL_SCANCODE_D) )
        hor = 1.0f;
    else if ( gf2d_input_key_just_pressed(SDL_SCANCODE_A) )
        hor = -1.0f;

    if ( gf2d_input_key_released(SDL_SCANCODE_W) || gf2d_input_key_released(SDL_SCANCODE_S) )
        ver = 0.0f;
    
    if( gf2d_input_key_just_pressed(SDL_SCANCODE_S) )
        ver = 1.0f;
    else if ( gf2d_input_key_just_pressed(SDL_SCANCODE_W) )
        ver = -1.0f;

    self->velocity.x = 3.0f * hor;
    self->velocity.y = 3.0f * ver;
}

void smh_awake()
{
    SJson *punti = NULL;
    PhysicsEntity *phys = NULL;
    UIComponent *ui = NULL;
    Trie trie = {0};

    punti = sj_load("application/entities/punti_jordan_phys.json");
    phys = gf2d_physics_entity_load( punti );
    sj_free(punti);

    phys->entity->update = somethink;
    gf2d_scene_add_to_drawables(phys, DET_PHYS);

    ui = gf2d_ui_label_new("Esto es un texto", NULL, 10, vector2d(128.0f, 128.0f));
    gf2d_label_set_text_color(ui->component.label, vector4d(0.0f, 0.0f, 0.0f, 255.0f));
    gf2d_scene_add_to_drawables(ui, DET_UI);

    trie = gf2d_trie_new();
    gf2d_trie_insert(&trie, "punti", punti);
    gf2d_trie_insert(&trie, "phys", phys);
    gf2d_trie_insert(&trie, "ui", ui);  

    slog("hola contains %u", gf2d_trie_remove(&trie, "hola"));
    slog("phys contrains %u", gf2d_trie_remove(&trie, "phys"));

    slog("can i get punti? %u == %u", punti, gf2d_trie_get(&trie, "punti"));

    gf2d_trie_free(&trie);
}