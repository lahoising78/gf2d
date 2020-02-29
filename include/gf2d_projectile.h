#ifndef _GF2D_PROJECTILE_H_
#define _GF2D_PROJECTILE_H_

#include "gf2d_physics_entity.h"

typedef struct
{
    PhysicsEntity *self;
    Entity *owner;
    Vector2D start;
    Vector2D dir;
    Vector2D dst;
    float speed;
    uint32_t _inuse;
} Projectile;

void gf2d_projectile_manager_init(uint32_t count);

PhysicsEntity *gf2d_projectile_create( Vector2D start, Vector2D dir, float speed, float distance, Entity *owner );

void gf2d_projectile_general_update( Entity *proj );

void gf2d_projectile_free( Entity *proj );

#endif