#include "simple_logger.h"
#include "gf2d_button.h"
#include "gf2d_input.h"

Button *gf2d_button_new(Label *text, void (*onClick)(Button *btn))
{
    Button *btn = NULL;

    btn = (Button*)gfc_allocate_array(sizeof(Button), 1);
    if(!btn) return NULL;

    btn->text = text;
    btn->onClick = onClick;
    return btn;
}

void gf2d_button_update(Button *btn)
{
    int x, y;
    uint8_t released;
    SDL_Rect rect = {0};
    SDL_Point point = {0};
    if(!btn || !btn->onClick) return;

    released = gf2d_input_mouse_released(SDL_BUTTON_LEFT);
    gf2d_input_mouse_position(&x, &y);

    vector2d_copy(rect, btn->text->_bg->position);
    rect.w = (int)btn->text->_bg->scale.x;
    rect.h = (int)btn->text->_bg->scale.y;

    point.x = x;
    point.y = y;

    if(released && SDL_PointInRect(&point, &rect))
    {
        btn->onClick(btn);
    }
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