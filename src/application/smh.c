#include "smh.h"
#include "simple_logger.h"

#include "gf2d_input.h"
#include "gf2d_scene.h"
#include "gf2d_physics_entity.h"
#include "gf2d_tilemap.h"

void somethink(Entity *self)
{
    if( gf2d_input_is_key_pressed(SDL_SCANCODE_D) )
        self->velocity.x = 3;
    else if( gf2d_input_is_key_pressed(SDL_SCANCODE_A) )
        self->velocity.x = -3;
    else
        self->velocity.x = 0;

    if( gf2d_input_is_key_pressed(SDL_SCANCODE_S) )
        self->velocity.y = 3;
    else if( gf2d_input_is_key_pressed(SDL_SCANCODE_W) )
        self->velocity.y = -3;
    else
        self->velocity.y = 0;
}

void smh_awake()
{
    PhysicsEntity *p = NULL;

    p = gf2d_physics_entity_new();
    // slog("--== %.2f %.2f ==--", p->entity->anim->rend->position.x, p->entity->anim->rend->position.y);
    p->entity->anim->rend->sprite = gf2d_sprite_load_all("images/space_bug.png", 128, 128, 16);
    p->entity->position.x = (4)*1200/8;
    p->entity->position.y = 3.0f*700.0f/4.0f;
    gf2d_animation_play(p->entity->anim, 7, 16);
    if( gf2d_scene_add_to_drawables(p, DET_PHYS) < 0 )
        slog("no hay espacion para phys");
}