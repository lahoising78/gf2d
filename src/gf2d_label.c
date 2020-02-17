#include "gf2d_label.h"
#include <SDL_ttf.h>

void gf2d_label_set_display(Label *label);

Label *gf2d_label_new(const char *text, uint32_t fontSize, Vector2D position)
{
    Label *label = NULL;
    Sprite *sprite = NULL;

    label = (Label*) gfc_allocate_array(sizeof(Label), 1);
    if(!label) return NULL;

    gfc_line_cpy(label->_text, text);

    label->fontSize = fontSize;
    label->display = gf2d_render_ent_new(NULL);

    return label;
}

void gf2d_label_set_display(Label *label)
{

}

void gf2d_label_free(Label *label)
{
    if(!label) return;

    if(label->display) gf2d_render_ent_free(label->display);
}