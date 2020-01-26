#ifndef _GF2D_ACTOR_H_
#define _GF2D_ACTOR_H_

#include "gf2d_entity.h"

typedef enum
{
    PET_STATIC = 0,
    PET_KINETIC = 1
} PhysicsEntityType;

/* A structure that can interact with the physics in the game */
typedef struct physics_entity_s
{
    Entity                  *entity;
    PhysicsEntityType       type;
    uint8_t                 useGravity;

    /** 
     * TODO: 
     * Add shapes for collision and triggers
     */

    uint8_t                 _inuse;
} PhysicsEntity;

/** 
 * @note count must be less or equal to the amount of entities allocated in entity manager
 */
void gf2d_physics_entity_manager_init(uint32_t count);

PhysicsEntity *gf2d_physics_entity_new();
void gf2d_physics_entity_free( PhysicsEntity *ent );

#endif