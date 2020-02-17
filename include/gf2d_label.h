#ifndef _GF2D_LABEL_H_
#define _GF2D_LABEL_H_

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_render_ent.h"

typedef struct
{
    char _text[GFCLINELEN];
    uint32_t fontSize;
    RenderEntity *display;
} Label;

Label *gf2d_label_new(const char *text, uint32_t fontSize, Vector2D position);

void gf2d_label_free(Label *label);

#endif