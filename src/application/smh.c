#include "smh.h"
#include "simple_logger.h"

#include "gf2d_input.h"
#include "gf2d_scene.h"
#include "gf2d_physics_entity.h"
#include "gf2d_tilemap.h"

void somethink(Entity *self)
{
    float state = gf2d_input_joystick_get_axis(0, 0);
    self->velocity.x = state * 3;
    self->velocity.y = gf2d_input_joystick_get_axis(0, 1) * 3;

    if( gf2d_input_joystick_button_pressed(0, 0) )
        slog("pressing button 0");

    if( gf2d_input_joystick_button_released(0, 1) )
        slog("released button 1");

    if( gf2d_input_is_key_pressed(SDL_SCANCODE_A) )
        slog("pressing A");

    if( gf2d_input_joystick_get_hat(0, 0, SDL_HAT_CENTERED) )
        slog("got a hat");
}

void smh_awake()
{
    PhysicsEntity *p = NULL;

    p = gf2d_physics_entity_new();
    // slog("--== %.2f %.2f ==--", p->entity->anim->rend->position.x, p->entity->anim->rend->position.y);
    p->entity->anim->rend->sprite = gf2d_sprite_load_all("images/space_bug.png", 128, 128, 16);
    p->entity->position.x = (4)*1200/8;
    p->entity->position.y = 3.0f*700.0f/4.0f;
    p->entity->update = somethink;
    p->type = PET_KINETIC;
    gf2d_animation_play(p->entity->anim, 7, 16);
    if( gf2d_scene_add_to_drawables(p, DET_PHYS) < 0 )
        slog("no hay espacion para phys");
}