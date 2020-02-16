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

typedef void (*AwakeFunction)();

void gf2d_scene_load_from_file(const char *filename);
void gf2d_scene_load( uint32_t drawablesCount, void (*scene_awake)() );
void gf2d_scene_close();
void gf2d_scene_render();

void gf2d_scene_awake_list_start(uint32_t count);
void gf2d_scene_submit_awake( void (*scene_awake)() );

int gf2d_scene_add_to_drawables( void *de, DrawableEntityType type );
void gf2d_scene_remove_from_drawables( void *e );

#endif