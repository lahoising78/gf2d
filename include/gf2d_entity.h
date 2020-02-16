#ifndef _GF2D_ENTITY_H_
#define _GF2D_ENTITY_H_

#include "gf2d_animation.h"
#include "gfc_vector.h"

typedef struct entity_s
{
    Animation           *anim;
    Vector2D            position;
    Vector2D            velocity;
    Vector2D            acceleration;
    Vector2D            rotation;

    void (*think)       (struct entity_s *ent);
    void (*update)      (struct entity_s *ent);
    void (*touch)       (struct entity_s *ent, struct entity_s *other);

    uint8_t _inuse;
} Entity;

/** 
 * @brief initialize entity manager
 * @param count how many entities to allocate
 */
void gf2d_entity_manager_init(uint32_t count);

/** 
 * @brief initialize each entity in entity manager
 * @note this is where each entity is assigned an animation
 */
void gf2d_entity_manager_initialize_all_entities();

/** 
 * @brief reset all data from all animations
 * @param del 1 to also forget the animation. 0 to keep it
 */
void gf2d_entity_manager_clean( uint8_t del );

/** 
 * @brief update all entities
 */
void gf2d_entity_manager_update();

/** 
 * @brief draw all entities
 */
void gf2d_entity_manager_draw();

/** 
 * @brief set entity initial values
 * @note it assumes all data is set to 0
 * @param ent the entity to init
 */
void gf2d_entity_init(Entity *ent);

/** 
 * @brief get new entity
 * @return an entity pointer
 */
Entity *gf2d_entity_new();

/** 
 * 
 */
Entity *gf2d_entity_load(SJson *json);

void gf2d_entity_load_to_entity(Entity *ent, SJson *json);

/** 
 * @brief clear data from entity
 * @param ent the entity to free
 */
void gf2d_entity_free(Entity *ent);

#endif