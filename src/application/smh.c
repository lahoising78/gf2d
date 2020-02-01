#include "smh.h"
#include "simple_logger.h"

#include "gf2d_scene.h"
#include "gf2d_physics_entity.h"

extern float frameTime;
void do_the_thing( Entity *self )
{
    self->position.y = 350.0f + 100.0f * cosf( 0.025f * self->position.x );
}

void smh_awake()
{
    int i;
    Entity *e = NULL;

    for(i = 0; i < 8+1; i++)
    {
        e = gf2d_entity_new();
        e->anim->rend->sprite = gf2d_sprite_load_all("images/space_bug.png", 128, 128, 16);
        e->position.x = i*1200/8;
        gf2d_animation_play(e->anim, i, 16);
        if( gf2d_scene_add_entity(e) < 0 )
            slog("no hay espacio suficiente");
        if(i%2 == 0) gf2d_scene_remove_entity(e);
    }
}