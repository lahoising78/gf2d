#include "gf2d_collision.h"
#include "simple_logger.h"
#include "gf2d_draw.h"

uint8_t gf2d_collision_check( CollisionShape *a, CollisionShape *b, CollisionInfo *info )
{
    int i;

    if(!a || !b) return 0;
    if( a->shapeType == CST_NONE || b->shapeType == CST_NONE ) return 0;

    if( a->shapeType == CST_BOX && b->shapeType == CST_BOX )
    {
        if ( a->position.x <= b->position.x + b->dimensions.wh.x &&  
            a->position.x + a->dimensions.wh.x >= b->position.x &&
            a->position.y <= b->position.y + b->dimensions.wh.y &&
            a->position.y + a->dimensions.wh.y >= b->position.y ) 
        {
            if (info)
            {
                for(i = 0; info->poc.x == 0.0f && info->poc.y == 0.0f && info->normal.x == 0.0f && info->normal.y == 0.0f; i++)
                {
                    info->poc.y = info->poc.x = 0;
                    info->normal.x = info->normal.y = 0;
                    if (a->position.x + i >= b->position.x + b->dimensions.wh.x)
                    {
                        info->poc.x = a->position.x;
                        info->normal.x = 1;
                    }
                    else if (b->position.x + i >= a->position.x + a->dimensions.wh.x)
                    {
                        info->poc.x = b->position.x;
                        info->normal.x = -1;
                    }
                    if (a->position.y + i >= b->position.y + b->dimensions.wh.y)
                    {
                        info->poc.y = a->position.y;
                        info->normal.y = -1;
                    }
                    if (b->position.y + i >= a->position.y + a->dimensions.wh.y)
                    {
                        info->normal.y = 1;
                        info->poc.y = b->position.y;
                    }
                }
                info->a = *a;
                info->b = *b;
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