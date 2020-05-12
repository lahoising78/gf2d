#ifndef _GF2D_UI_H_
#define _GF2D_UI_H_

#include "gf2d_label.h"
#include "gf2d_progress_bar.h"
#include "gf2d_button.h"

typedef enum
{
    UIT_NONE        = 0,
    UIT_LABEL       = 1,
    UIT_PBAR        = 2,
    UIT_BUTTON
} UIComponentType;

typedef struct
{
    union
    {
        Label *label;
        ProgressBar *pbar;
        Button *btn;
    }                           component;
    UIComponentType             _uiType;
    uint8_t                     _inuse;
} UIComponent;

/** 
 * @note TO UPDATE BUTTONS AND OTHER UI, YOU MUST CALL THEIR UPDATES MANUALLY FROM AN ENTITY UPDATE
 */

void gf2d_ui_manager_init( uint32_t count );

void gf2d_ui_manager_render();

UIComponent *gf2d_ui_load(SJson *json);
void gf2d_ui_update( UIComponent *ui );
void gf2d_ui_render( UIComponent *ui );

UIComponent *gf2d_ui_label_new(const char *text, TTF_Font *font, uint32_t fontSize, Vector2D position);
UIComponent *gf2d_ui_progress_bar_new(Vector4D *backColor, Vector2D backScale, Vector4D *foreColor, Vector2D foreScale);
UIComponent *gf2d_ui_button_new(Label *label, void (*onClick)(Button *btn));

#endif