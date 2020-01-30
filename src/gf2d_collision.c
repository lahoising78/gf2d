#include "gf2d_collision.h"

uint8_t gf2d_collision_check( CollisionShape *a, CollisionShape *b )
{
    if( a->shapeType == CST_BOX && b->shapeType == CST_BOX )
    {
        return (a->position.x < b->position.x + b->dimensions.wh.x &&
                a->position.x + a->dimensions.wh.x > b->position.x &&
                a->position.y < b->position.y + b->dimensions.wh.y &&
                a->position.y + a->dimensions.wh.y > b->position.y);
    }

    return 0;
}