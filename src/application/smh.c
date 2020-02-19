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
    Vector4D bg = {0.0f, 0.0f, 255.0f, 255.0f};

    ui = gf2d_ui_progress_bar_new(&bg, vector2d(100, 100), NULL, vector2d(50, 50));
    gf2d_scene_add_to_drawables(ui, DET_UI);
}