#include "smh.h"
#include "simple_logger.h"

#include "gf2d_draw.h"
#include "gf2d_input.h"
#include "gf2d_scene.h"
#include "gf2d_physics_entity.h"
#include "gf2d_tilemap.h"
#include "gf2d_camera.h"
#include "gf2d_ui.h"

void smh_awake()
{
    UIComponent *ui = NULL;

    ui = gf2d_ui_label_new("hola como estas?", NULL, 32, vector2d(0.0f, 0.0f));
    gf2d_scene_add_to_drawables(ui, DET_UI);
}