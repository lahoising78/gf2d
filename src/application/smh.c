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
    PhysicsEntity *phys = NULL;

    phys = gf2d_physics_entity_get_by_name("punti");
    if(phys)
        phys->entity->update = somethink;
}