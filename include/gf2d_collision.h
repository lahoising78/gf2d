#ifndef _GF2D_COLLISION_H_
#define _GF2D_COLLISION_H_

#include "gfc_vector.h"
#include "gf2d_json.h"

typedef enum
{
    CST_NONE = 0,
    CST_BOX = 1,
    CST_CIRCLE = 2
} CollisionShapeType;

typedef struct
{
    Vector2D position;
    union
    {
        Vector2D wh;
        float radius;
    } dimensions;
    CollisionShapeType shapeType;
} CollisionShape;

typedef struct
{
    uint8_t collided;
    CollisionShape a;
    CollisionShape b;
    Vector2D poc;
    Vector2D normal;
    Vector2D originalPosition;
    float overlap;
} CollisionInfo;

/** 
 * @brief create a collision shape
 * @param position the position of the collision shape to create
 * @note entities use position as an offset relative to them
 * @param dimensions x hold radius if it is for a circle. otherwise it's width and height
 * @param type the collision shape type (box or circle)
 * @return a collision shape
 */
CollisionShape gf2d_collision_shape(Vector2D position, Vector2D dimensions, CollisionShapeType type);

CollisionShape gf2d_collision_shape_load(SJson *json);

/** 
 * @brief check if two bodies have collided and, if so, get information about the collision
 * @param a the first body to check
 * @param b the second body to check
 * @param info (optional) where to store the information about collision
 * @return 1 if bodies are colliding, 0 if not
 */
uint8_t gf2d_collision_check( CollisionShape *a, CollisionShape *b, CollisionInfo *info );

/** 
 * @brief draw a collision shape
 * @param shape the shape to draw
 */
void gf2d_collision_draw( CollisionShape *shape );

#endif