#include "gf2d_camera.h"

typedef struct
{
    Vector2D position;
    Vector2D view;
} Camera;

static Camera gf2d_camera = {0};

void gf2d_camera_set_view(Vector2D view)
{
    vector2d_copy(gf2d_camera.view, view);
}

Vector2D gf2d_camera_get_displaced_position(Vector2D position)
{
    Vector2D finalPos = {0};

    vector2d_sub(finalPos, position, gf2d_camera.position);

    return finalPos;
}

void gf2d_camera_move(Vector2D displacement)
{
    vector2d_add(gf2d_camera.position, gf2d_camera.position, displacement);
}

void gf2d_camera_set_position(Vector2D position)
{
    // vector2d_copy(gf2d_camera.position, position);
    vector2d_scale(gf2d_camera.position, gf2d_camera.view, 0.5f);
    vector2d_sub(gf2d_camera.position, position, gf2d_camera.position);
}

CollisionShape gf2d_camera_collision_shape()
{
    CollisionShape ret;
    ret.shapeType = CST_BOX;
    ret.position = gf2d_camera.position;
    ret.dimensions.wh.x = 1200;
    ret.dimensions.wh.y = 700;
    return ret;
}