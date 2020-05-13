#ifndef _GF2D_LABEL_H_
#define _GF2D_LABEL_H_

#include "gfc_types.h"
#include "gfc_vector.h"
#include "gf2d_render_ent.h"
#include "gf2d_font.h"

typedef struct
{
    char                    _text[GFCLINELEN];
    uint32_t                fontSize;
    RenderEntity            *_display;
    RenderEntity            *_bg;
    TTF_Font                *font;

    uint8_t                 _inuse;
} Label;

void gf2d_label_manager_init( uint32_t count );

Label *gf2d_label_new(const char *text, TTF_Font *font, uint32_t fontSize, Vector2D position);
void gf2d_label_render( Label *label );
void gf2d_label_free(Label *label);

void gf2d_label_set_text_color(Label *label, Vector4D newColor);
void gf2d_label_set_background_color(Label *label, Vector4D color);
void gf2d_label_set_display(Label *label);

#endif