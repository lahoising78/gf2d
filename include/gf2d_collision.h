#ifndef _GF2D_COLLISION_H_
#define _GF2D_COLLISION_H_

#include "gfc_vector.h"

typedef enum
{
    CST_BOX = 0,
    CST_CIRCLE = 1
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

uint8_t gf2d_collision_check( CollisionShape *a, CollisionShape *b );

#endif