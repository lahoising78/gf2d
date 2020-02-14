#include "gf2d_collision.h"
#include "simple_logger.h"
#include "gf2d_draw.h"

uint8_t gf2d_collision_check( CollisionShape *a, CollisionShape *b, Vector2D *poc, Vector2D *normal )
{
    if(!a || !b) return 0;
    if( a->shapeType == CST_NONE || b->shapeType == CST_NONE ) return 0;

    if( a->shapeType == CST_BOX && b->shapeType == CST_BOX )
    {
        if ( a->position.x <= b->position.x + b->dimensions.wh.x &&  
            a->position.x + a->dimensions.wh.x >= b->position.x &&
            a->position.y <= b->position.y + b->dimensions.wh.y &&
            a->position.y + a->dimensions.wh.y >= b->position.y ) 
        {
            if (poc)
            {
                poc->y = poc->x = 0;
                if (normal)normal->x = normal->y = 0;
                if (a->position.x + 1 >= b->position.x + b->dimensions.wh.x)
                {
                    poc->x = a->position.x;
                    if (normal)normal->x = -1;
                }
                else if (b->position.x + 1 >= a->position.x + a->dimensions.wh.x)
                {
                    poc->x = b->position.x;
                    if (normal)normal->x = 1;
                }
                if (a->position.y + 1 >= b->position.y + b->dimensions.wh.y)
                {
                    poc->y = a->position.y;
                    if (normal)normal->y = -1;
                }
                if (b->position.y + 1 >= a->position.y + a->dimensions.wh.y)
                {
                    if (normal)normal->y = 1;
                    poc->y = b->position.y;
                }
            }

            return 1;
        }
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