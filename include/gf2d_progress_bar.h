#ifndef _GF2D_PROGRESS_BAR_H_
#define _GF2D_PROGRESS_BAR_H_

#include "gfc_vector.h"
#include "gf2d_render_ent.h"

typedef struct
{
    RenderEntity *_background;
    Vector2D backSize;
    RenderEntity *_foreground;
    Vector2D foreSize;
    float _maxValue;
    float _value;
    uint8_t _inuse;
} ProgressBar;

void gf2d_progress_bar_manager_init(uint32_t count);

ProgressBar *gf2d_progress_bar_new(Vector4D *backColor, Vector2D backSize, Vector4D *foreColor, Vector2D foreSize);
void gf2d_progress_bar_render(ProgressBar *pbar);
void gf2d_progress_bar_free(ProgressBar *pbar);

void gf2d_progress_bar_set_position_and_offset(ProgressBar *pbar, Vector2D position, Vector2D offset);
void gf2d_progress_bar_set_max_value(ProgressBar *pbar, float value);
void gf2d_progress_bar_set_value(ProgressBar *pbar, float value);

#endif