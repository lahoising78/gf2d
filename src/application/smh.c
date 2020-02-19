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

    ui = gf2d_ui_progress_bar_new(NULL, NULL);
    // gf2d_progress_bar_free(ui->component.pb);
    gf2d_scene_add_to_drawables(ui, DET_UI);
}