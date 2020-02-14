#ifndef _GF2D_ACTOR_H_
#define _GF2D_ACTOR_H_

#include "gf2d_entity.h"
#include "gf2d_collision.h"

// #define PE_MAX_MODEL_BOXES 3

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

    CollisionShape          modelBox;
    uint8_t                 canCollide;                             /* 1 if we want to use modelBoxes to collide, 0 if modelBoxes should only be triggers */

    uint8_t                 _onFloor;
    uint8_t                 _inuse;
} PhysicsEntity;

/** 
 * @brief initialize physics entity manager. This is where physics entities are allocated and initialized
 * @note count must be less or equal to the amount of entities allocated in entity manager
 * @param count how many physics entities to allocate
 */
void gf2d_physics_entity_manager_init(uint32_t count);

/** 
 * @brief reset all data for all animations
 * @param freeEntity 1 to free entities and 0 to keep it
 */
void gf2d_physics_entity_manager_clean( uint32_t freeEntity );

/** 
 * @brief set initial data for all entities
 */
void gf2d_physics_entity_manager_initialize_entities();

/** 
 * @brief update all entities
 */
void gf2d_physics_entity_manager_update();

/** 
 * @brief get a physics entity that is not being used
 * @return a pointer to a physics entity
 */
PhysicsEntity *gf2d_physics_entity_new();

/** 
 * @brief free the physics entity and clear values
 */
void gf2d_physics_entity_free( PhysicsEntity *ent );

#endif