#include "gf2d_camera.h"

typedef struct
{
    Vector2D position;
} Camera;

static Camera gf2d_camera = {0};

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