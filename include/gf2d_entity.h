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
 * @brief get new entity
 * @return an entity pointer
 */
Entity *gf2d_entity_new();

/** 
 * @brief clear data from entity
 * @param ent the entity to free
 */
void gf2d_entity_free(Entity *ent);

#endif