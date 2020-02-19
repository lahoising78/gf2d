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
    Vector4D bg = {255.0f, 255.0f, 255.0f, 255.0f};

    ui = gf2d_ui_progress_bar_new(&bg, NULL);
    // vector2d_set(ui->component.pbar->_background->scale, 0.2, 0.2);
    vector2d_set(ui->component.pbar->_foreground->scale, 600, 360);
    // vector2d_set(ui->component.pbar->_background->position, 600.0f, 360.0f);
    // gf2d_progress_bar_free(ui->component.pb);
    gf2d_scene_add_to_drawables(ui, DET_UI);
}