#ifndef _GF2D_SCENE_H_
#define _GF2D_SCENE_H_

#include "gfc_list.h"
#include "gf2d_physics_entity.h"
#include "gf2d_animation.h"

void gf2d_scene_load( uint32_t physics_entities_count, uint32_t entities, uint32_t animCount, void (*scene_awake)() );
void gf2d_scene_close();

void gf2d_scene_render();

int gf2d_scene_add_entity( Entity *ent );
void gf2d_scene_remove_entity( Entity *e );

int gf2d_scene_add_animation(Animation *anim);

#endif