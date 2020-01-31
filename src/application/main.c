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

extern float frameTime;
void update_that_thing( Entity *e )
{
    if(!e) return;
    if( gf2d_input_is_key_pressed( SDL_SCANCODE_D ) )
    {
        e->position.x += 1;
    }
}

void un_awake_ahi()
{
    Entity *e = NULL;
    e = gf2d_entity_new();
    e->render_ent->sprite = gf2d_sprite_load_all("images/space_bug.png", 128, 128, 16);
    e->update = update_that_thing;
    gf2d_scene_add_entity(e);
    gf2d_scene_remove_entity(e);
}

void gf2d_main()
{
    slog("MAIN");
    gf2d_scene_load(8, 8, un_awake_ahi );
}