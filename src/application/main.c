#include "gf2d_main.h"
#include "simple_logger.h"

#include "gf2d_scene.h"
#include "gf2d_entity.h"
#include "gf2d_physics_entity.h"

#include "gf2d_input.h"

void gf2d_config()
{
    slog("CONFIG");

    gf2d_main_set_window_name( "Punti Jordan - El Sayayin de la Patria" );
}

void hola()
{
    Entity *pe = NULL;
    pe = gf2d_entity_new();
    pe->render_ent->sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    gf2d_scene_add_entity(pe);
}

void gf2d_main()
{
    slog("MAIN");
    gf2d_scene_load(8, 8, hola );
}