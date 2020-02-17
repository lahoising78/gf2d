#include "smh.h"
#include "simple_logger.h"

#include "gf2d_draw.h"
#include "gf2d_input.h"
#include "gf2d_scene.h"
#include "gf2d_physics_entity.h"
#include "gf2d_tilemap.h"
#include "gf2d_camera.h"
#include "gf2d_label.h"

Label *label = NULL;

void clear_ui()
{
    slog("CLEAR UI");
    gf2d_label_free(label);
    if(label) free(label);
}

void draw_label(Entity *self)
{
    SDL_Rect rect = {0};
    // slog("draw label");
    gf2d_label_render(label);

    vector2d_copy(rect, label->_display->position);
    rect.w = 100.0f;
    rect.h = 100.0f;

    gf2d_draw_rect(rect, label->_textColor);
}

void smh_awake()
{
    slog("hello from smh");

    label = gf2d_label_new("Habia una vez que un tipo dijo \"hola como esta?\" y yo dije bien", NULL, 14, vector2d(100.0f, 100.0f));
    atexit(clear_ui);

    gf2d_scene_add_to_drawables(label, DET_UI);
    // slog("label %s %.2f %.2f %u %u", label->_text, label->_display->position.x, label->_display->position.y, label->fontSize, label->font);
}