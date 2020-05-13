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
    Vector2D min = {0};

    if(!a || !b) return 0;
    if( a->shapeType == CST_NONE || b->shapeType == CST_NONE ) return 0;

    if( a->shapeType == CST_BOX && b->shapeType == CST_BOX )
    {
        vector2d_set(aMax, a->position.x + a->dimensions.wh.x, a->position.y + a->dimensions.wh.y);
        vector2d_set(bMax, b->position.x + b->dimensions.wh.x, b->position.y + b->dimensions.wh.y);
        
        if( bMax.x >= a->position.x && aMax.x >= b->position.x &&
            bMax.y >= a->position.y && aMax.y >= b->position.y )
        {
            // slog("colliding");
            if (info)
            {
                min.x = MIN( aMax.x, bMax.x ) - MAX( a->position.x, b->position.x );
                min.y = MIN( aMax.y, bMax.y ) - MAX( a->position.y, b->position.y );

                // slog("min %.2f %.2f", min.x, min.y);

                if(min.x < min.y)
                {
                    info->overlap = min.x;
                    info->normal.x = a->position.x > b->position.x ? 1.0f : -1.0f;
                }
                else if (min.x > min.y)
                {
                    info->overlap = min.y;
                    info->normal.y = a->position.y > b->position.y ? 1.0f : -1.0f;
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

void gf2d_collision_shape_type_to_string(char *dst, CollisionShapeType cst)
{
    if(!dst) return;

    switch (cst)
    {
    case CST_BOX:
        strcpy(dst, "CST_BOX");
        break;

    case CST_CIRCLE:
        strcpy(dst, "CST_CIRCLE");
        break;
    
    default:
        strcpy(dst, "CST_NONE");
        break;
    }
}