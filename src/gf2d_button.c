#include "gf2d_button.h"

Button *gf2d_button_new(Label *text, void (*onClick)(Button *btn))
{
    Button *btn = NULL;

    btn = (Button*)gfc_allocate_array(sizeof(Button), 1);
    if(!btn) return NULL;

    btn->text = text;
    btn->onClick = onClick;
    return btn;
}

void gf2d_button_render(Button *btn)
{
    if(!btn) return;

    gf2d_label_render(btn->text);
}

void gf2d_button_free(Button *btn)
{
    if(!btn) return;

    gf2d_label_free(btn->text);
}