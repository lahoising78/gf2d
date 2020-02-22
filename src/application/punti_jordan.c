#include "simple_logger.h"
#include "gf2d_input.h"
#include "punti_jordan.h"

void punti_jordan_think();

float hor = 0.0f;
float ver = 0.0f;

void punti_jordan(PhysicsEntity *punti)
{
    if(!punti || !punti->entity) return;

    slog("Punti Jordan, sayayin de la patria, reportandose");
    punti->entity->update = punti_jordan_think;
}

void punti_jordan_think(Entity *self)
{
    if(!self) return;

    if( gf2d_input_key_just_pressed(SDL_SCANCODE_A) )
        hor = -1.0f;
    else if( gf2d_input_key_just_pressed(SDL_SCANCODE_D) )
        hor = 1.0f;
    else if( gf2d_input_key_released(SDL_SCANCODE_A) || gf2d_input_key_released(SDL_SCANCODE_D) )
        hor = 0.0f;

    if( gf2d_input_key_just_pressed(SDL_SCANCODE_W) )
        ver = -1.0f;
    else if( gf2d_input_key_just_pressed(SDL_SCANCODE_S) )
        ver = 1.0f;
    else if( gf2d_input_key_released(SDL_SCANCODE_W) || gf2d_input_key_released(SDL_SCANCODE_S) )
        ver = 0.0f;

    self->velocity.x = 3.0f * hor;
    self->velocity.y = 3.0f * ver;
}