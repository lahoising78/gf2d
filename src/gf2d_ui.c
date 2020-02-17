#include "simple_logger.h"
#include "gf2d_ui.h"

typedef struct
{
    UIComponent                 *component_list;
    uint32_t                    count;
} UIManager;

static UIManager gf2d_ui_manager = {0};

UIComponent *gf2d_ui_new();
void gf2d_ui_render( UIComponent *ui );
void gf2d_ui_free( UIComponent *ui );

void gf2d_ui_manager_close()
{
    UIComponent *ui = NULL;
    int i;
    for(i = 0; i < gf2d_ui_manager.count; i++)
    {
        ui = &gf2d_ui_manager.component_list[i];
        gf2d_ui_free(ui);
    }
}

void gf2d_ui_manager_init( uint32_t count )
{
    gf2d_ui_manager.component_list = (UIComponent*)gfc_allocate_array(sizeof(UIComponent), count);
    if(!gf2d_ui_manager.component_list)
    {
        slog("Unable to initialize UI Manager");
        return;
    }
    gf2d_ui_manager.count = count;
    atexit(gf2d_ui_manager_close);
}

void gf2d_ui_manager_render()
{
    UIComponent *ui = NULL;
    int i;

    for(i = 0; i < gf2d_ui_manager.count; i++)
    {
        ui = &gf2d_ui_manager.component_list[i];
        if(!ui->_inuse) continue;

        gf2d_ui_render(ui);
    }
}

UIComponent *gf2d_ui_new()
{
    UIComponent *ui = NULL;
    int i;

    for(i = 0; i < gf2d_ui_manager.count; i++)
    {
        ui = &gf2d_ui_manager.component_list[i];
        if(ui->_inuse) continue;
        
        ui->_inuse = 1;

        return ui;
    }

    return NULL;
}

void gf2d_ui_render( UIComponent *ui )
{
    if(!ui) return;

    switch (ui->_uiType)
    {
    case UIT_LABEL:
        gf2d_label_render(ui->component.label);
        break;
    
    default:
        break;
    }
}

void gf2d_ui_free( UIComponent *ui )
{
    if(!ui) return;

    switch (ui->_uiType)
    {
    case UIT_LABEL:
        if(!ui->component.label) break;
        gf2d_label_free(ui->component.label);
        break;
    
    default:
        break;
    }

    ui->_inuse = 0;
}

UIComponent *gf2d_ui_label_new(const char *text, TTF_Font *font, uint32_t fontSize, Vector2D position)
{
    UIComponent *ui = NULL;

    ui = gf2d_ui_new();
    ui->_uiType = UIT_LABEL;
    ui->component.label = gf2d_label_new(text, font, fontSize, position);

    return ui;
}