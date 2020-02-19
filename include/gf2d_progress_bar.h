#ifndef _GF2D_PROGRESS_BAR_H_
#define _GF2D_PROGRESS_BAR_H_

#include "gfc_vector.h"
#include "gf2d_render_ent.h"

typedef struct
{
    RenderEntity *_background;
    RenderEntity *_foreground;
    float maxValue;
    float *_value;
    uint8_t _inuse;
} ProgressBar;

void gf2d_progress_bar_manager_init(uint32_t count);

ProgressBar *gf2d_progress_bar_new(Vector4D *backColor, Vector4D *foreColor);
void gf2d_progress_bar_free(ProgressBar *pbar);

#endif