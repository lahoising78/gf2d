#include "simple_logger.h"
#include "gf2d_input.h"
#include "player.h"

#define VELOCITY_CONST 3.0f

typedef enum
{
    DIR_NONE =                  0x00,
    DIR_UP =                    0x01,
    DIR_RIGHT =                 0x02,
    DIR_DOWN =                  0x04,
    DIR_LEFT =                  0x08
} WalkDirection;

void player_update(Entity *self);

void player_walking();

float walkDir = 0.0f;
float ydir = 0.0f;
uint8_t left, right;

void player_create(PhysicsEntity *self)
{
    if(!self || !self->entity) return;
    
    self->entity->update = player_update;
}

void player_update(Entity *self)
{
    if(!self) return;

    player_walking();
    self->velocity.x = walkDir * VELOCITY_CONST;
    self->velocity.y = ydir * VELOCITY_CONST;
}

void player_walking()
{
    float dir = 0.0f;

    dir = gf2d_input_joystick_get_axis(0, 0);
    ydir = gf2d_input_joystick_get_axis(0, 1);
    if( dir != 0.0f ) 
    {
        walkDir = dir;
        return;
    }

    if( gf2d_input_key_just_pressed(SDL_SCANCODE_RIGHT) )
        right = 1;
    else if ( gf2d_input_key_released(SDL_SCANCODE_RIGHT) )
        right = 0;
    
    if ( gf2d_input_key_just_pressed(SDL_SCANCODE_LEFT) )
        left = 1;
    else if ( gf2d_input_key_released(SDL_SCANCODE_LEFT) )
        left = 0;

    walkDir = (float)(right - left);
}