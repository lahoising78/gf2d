#ifndef _GF2D_BUTTON_H_
#define _GF2D_BUTTON_H_

#include "gf2d_label.h"

typedef struct button_s
{
    Label *text;

    void (*onClick)(struct button_s *btn);

    uint8_t _inuse;
} Button;

Button *gf2d_button_new(Label *text, void (*onClick)(Button *btn));
void gf2d_button_update(Button *btn);
void gf2d_button_render(Button *btn);
void gf2d_button_free(Button *btn);

#endif