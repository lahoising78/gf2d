#ifndef _GF2D_COLLISION_H_
#define _GF2D_COLLISION_H_

#include "gfc_vector.h"

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
} CollisionInfo;

uint8_t gf2d_collision_check( CollisionShape *a, CollisionShape *b, CollisionInfo *info );

void gf2d_collision_draw( CollisionShape *shape );

#endif