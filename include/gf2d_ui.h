#ifndef _GF2D_UI_H_
#define _GF2D_UI_H_

#include "gf2d_label.h"
#include "gf2d_progress_bar.h"

typedef enum
{
    UIT_NONE        = 0,
    UIT_LABEL       = 1,
    UIT_PBAR        = 2
} UIComponentType;

typedef struct
{
    union
    {
        Label *label;
        ProgressBar *pbar;
    }                           component;
    UIComponentType             _uiType;
    uint8_t                     _inuse;
} UIComponent;

void gf2d_ui_manager_init( uint32_t count );

void gf2d_ui_manager_render();

void gf2d_ui_render( UIComponent *ui );

UIComponent *gf2d_ui_label_new(const char *text, TTF_Font *font, uint32_t fontSize, Vector2D position);
UIComponent *gf2d_ui_progress_bar_new(Vector4D *backColor, Vector4D *foreColor);

#endif