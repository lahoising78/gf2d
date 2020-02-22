#include "gf2d_collision.h"
#include "simple_logger.h"
#include "gf2d_draw.h"
#include "gf2d_camera.h"

CollisionShapeType gf2d_collision_type_from_string(const char *str)
{
    if( strcmp(str, "CST_BOX") == 0 ) return CST_BOX;
    if( strcmp(str, "CST_CIRCLE") == 0 ) return CST_CIRCLE;
    return CST_NONE;
}

CollisionShape gf2d_collision_shape(Vector2D position, Vector2D dimensions, CollisionShapeType type)
{
    CollisionShape shape = {0};

    shape.position = position;
    shape.shapeType = type;

    switch (type)
    {
    case CST_BOX:
        shape.dimensions.wh = dimensions;
        break;
    
    default:
        break;
    }

    return shape;
}

CollisionShape gf2d_collision_shape_load(SJson *json)
{
    Vector2D position = {0};
    Vector2D dimensions = {0};
    CollisionShapeType shapeType = CST_NONE;

    if(!json) return gf2d_collision_shape(position, dimensions, shapeType);

    position = gf2d_json_vector2d( sj_object_get_value(json, "position") );
    dimensions = gf2d_json_vector2d( sj_object_get_value(json, "dimensions") );
    shapeType = gf2d_collision_type_from_string( sj_get_string_value( sj_object_get_value(json, "shapeType") ) );

    return gf2d_collision_shape(position, dimensions, shapeType);
}

uint8_t gf2d_collision_check( CollisionShape *a, CollisionShape *b, CollisionInfo *info )
{
    Vector2D aMax = {0};
    Vector2D bMax = {0};
    int i;

    if(!a || !b) return 0;
    if( a->shapeType == CST_NONE || b->shapeType == CST_NONE ) return 0;

    if( a->shapeType == CST_BOX && b->shapeType == CST_BOX )
    {
        vector2d_set(aMax, a->position.x + a->dimensions.wh.x, a->position.y + a->dimensions.wh.y);
        vector2d_set(bMax, b->position.x + b->dimensions.wh.x, b->position.y + b->dimensions.wh.y);
        // if ( a->position.x <= b->position.x + b->dimensions.wh.x &&  
        //     a->position.x + a->dimensions.wh.x >= b->position.x &&
        //     a->position.y <= b->position.y + b->dimensions.wh.y &&
        //     a->position.y + a->dimensions.wh.y >= b->position.y ) 
        if( bMax.x > a->position.x && aMax.x > b->position.x &&
            bMax.y > a->position.y && aMax.y > b->position.y )
        {
            slog("colliding");
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
                    else if (b->position.y + i >= a->position.y + a->dimensions.wh.y)
                    {
                        info->normal.y = 1;
                        info->poc.y = b->position.y;
                    }
                }
                info->a = *a;
                info->b = *b;
            }

            info->overlap = MIN( 
                MIN( aMax.x, bMax.x ) - MAX( a->position.x, b->position.x ),
                MIN( aMax.y, bMax.y ) - MAX( a->position.y, b->position.y )
            );

            return 1;
        }
    }
    return 0;
}

void gf2d_collision_draw( CollisionShape *shape )
{
    SDL_Rect rect = {0};
    Vector2D finalPosition = {0};
    if(!shape) return;

    finalPosition = gf2d_camera_get_displaced_position(shape->position);
    switch ( shape->shapeType )
    {
    case CST_BOX:
        gfc_rect_set(rect, finalPosition.x, finalPosition.y, shape->dimensions.wh.x, shape->dimensions.wh.y);
        gf2d_draw_rect(rect, vector4d(255, 0, 255, 255));
        break;

    case CST_CIRCLE:
        gf2d_draw_circle(finalPosition, shape->dimensions.radius, vector4d(0.0f, 255.0f, 0.0f, 255.0f));
        break;
    
    default:
        break;
    }
}