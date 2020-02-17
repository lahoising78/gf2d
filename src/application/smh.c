#include "smh.h"
#include "simple_logger.h"

#include "gf2d_input.h"
#include "gf2d_scene.h"
#include "gf2d_physics_entity.h"
#include "gf2d_tilemap.h"
#include "gf2d_camera.h"
#include "gf2d_label.h"

void smh_awake()
{
    Label *label = NULL;
    slog("hello from smh");

    label = gf2d_label_new("hola como est'a?", 14, vector2d(0.0f, 0.0f));

    gf2d_label_free(label);
}