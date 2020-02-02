#ifndef _GF2D_SCENE_H_
#define _GF2D_SCENE_H_

#include "gfc_list.h"
#include "gf2d_physics_entity.h"
#include "gf2d_animation.h"

typedef enum
{
    DET_NONE = 0,
    DET_REND = 1,
    DET_ANIM = 2,
    DET_ENT  = 3,
    DET_PHYS = 4,
    DET_TMAP = 5
} DrawableEntityType;

void gf2d_scene_load( uint32_t drawablesCount, void (*scene_awake)() );
void gf2d_scene_close();
void gf2d_scene_render();

// int gf2d_scene_add_entity( Entity *ent );
// void gf2d_scene_remove_entity( Entity *e );

// int gf2d_scene_add_animation(Animation *anim);
// void gf2d_scene_remove_animation(Animation *anim);

int gf2d_scene_add_to_drawables( void *de, DrawableEntityType type );
void gf2d_scene_remove_from_drawables( void *e );

#endif