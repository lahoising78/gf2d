#ifndef _GF2D_SCENE_H_
#define _GF2D_SCENE_H_

#include "gfc_list.h"
#include "gf2d_physics_entity.h"

typedef struct
{
    char                name[GFCLINELEN];
    PhysicsEntity       **physics_entities;
    uint32_t            physics_entities_count;
    Entity              **entities;
    uint32_t            entities_count;

    List                *render_list;
} Scene;

void gf2d_scene_load( uint32_t physics_entities_count, uint32_t entities, void (*scene_awake)() );
void gf2d_scene_close();

void gf2d_scene_render();

int gf2d_scene_add_entity( Entity *ent );

#endif