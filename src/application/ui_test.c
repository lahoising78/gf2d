#include "ui_test.h"
#include "gf2d_scene.h"
#include "gf2d_ui.h"

void ui_test_awake()
{
    UIComponent *label = NULL;
    UIComponent *pbar = NULL;

    label = gf2d_ui_label_new("this is label", NULL, 32, vector2d(10.0f, 10.0f));
    gf2d_label_set_text_color(label->component.label, vector4d(100.0f, 100.0f, 100.0f, 255.0f));
    gf2d_scene_add_to_drawables(label, DET_UI);

    Vector4D bgColor = vector4d(192.0f, 192.0f, 192.0f, 255.0f);
    Vector4D frColor = vector4d(100.0f, 100.0f, 100.0f, 255.0f);
    pbar = gf2d_ui_progress_bar_new( &bgColor, vector2d(200.0f, 30.0f), &frColor, vector2d(190.0f, 20.0f) );
    gf2d_progress_bar_set_position_and_offset(pbar->component.pbar, vector2d(10.0f, 200.0f), vector2d(5.0f, 5.0f));
    gf2d_scene_add_to_drawables(pbar, DET_UI);
}