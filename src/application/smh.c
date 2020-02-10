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
    Tilemap *tm = NULL;
    PhysicsEntity *pe = NULL;
    uint32_t w = 8, h = 8;
    uint32_t map[8 * 8] = {
         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 1, 1, 0, 0, 0,
         0, 0, 1, 1, 1, 1, 0, 0,
         0, 0, 1, 1, 1, 1, 0, 0,
         0, 0, 0, 1, 1, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
         0, 0, 0, 0, 0, 0, 0, 0,
    };

    tm = gf2d_tilemap_load( gf2d_sprite_load_all("images/pointer.png", 32, 32, 16), map, w, h );
    gf2d_scene_add_to_drawables(tm, DET_TMAP);

    pe = gf2d_physics_entity_new();
    pe->entity->anim->rend->sprite = gf2d_sprite_load_all("images/space_bug.png", 128, 128, 16);
    pe->type = PET_KINETIC;
    pe->canCollide = 1;

    pe->modelBox.shapeType = CST_BOX;
    pe->modelBox.dimensions.wh = vector2d(32, 32);

    gf2d_animation_play(pe->entity->anim, 0, 16);
    pe->entity->update = somethink;
    gf2d_scene_add_to_drawables(pe, DET_PHYS);
}