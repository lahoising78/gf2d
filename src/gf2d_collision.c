#include "gf2d_collision.h"
#include "simple_logger.h"
#include "gf2d_draw.h"

uint8_t gf2d_collision_check( CollisionShape *a, CollisionShape *b )
{
    if(!a || !b) return 0;
    if( a->shapeType == CST_NONE || b->shapeType == CST_NONE ) return 0;

    if( a->shapeType == CST_BOX && b->shapeType == CST_BOX )
    {
        return (a->position.x <= b->position.x + b->dimensions.wh.x &&
                a->position.x + a->dimensions.wh.x >= b->position.x &&
                a->position.y <= b->position.y + b->dimensions.wh.y &&
                a->position.y + a->dimensions.wh.y >= b->position.y);
    }

    return 0;
}

void gf2d_collision_draw( CollisionShape *shape )
{
    SDL_Rect rect = {0};
    if(!shape) return;
    switch ( shape->shapeType )
    {
    case CST_BOX:
        gfc_rect_set(rect, shape->position.x, shape->position.y, shape->dimensions.wh.x, shape->dimensions.wh.y);
        gf2d_draw_rect(rect, vector4d(255, 0, 255, 255));
        break;

    case CST_CIRCLE:
        gf2d_draw_circle(shape->position, shape->dimensions.radius, vector4d(0.0f, 255.0f, 0.0f, 255.0f));
        break;
    
    default:
        break;
    }
}