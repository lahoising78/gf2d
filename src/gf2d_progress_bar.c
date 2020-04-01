#include "simple_logger.h"
#include "gf2d_progress_bar.h"
#include "gf2d_sprite_manipulation.h"
#include "gf2d_camera.h"

typedef struct
{
    ProgressBar *progress_bar_list;
    uint32_t count;
} ProgressBarManager;

static ProgressBarManager gf2d_pb_manager = {0};

void gf2d_progress_bar_manager_close();
void gf2d_progress_bar_manager_init_all();

ProgressBar *gf2d_progress_bar_get_new();

void gf2d_progress_bar_manager_init(uint32_t count)
{
    gf2d_pb_manager.progress_bar_list = (ProgressBar*)gfc_allocate_array(sizeof(ProgressBar), count);
    if( !gf2d_pb_manager.progress_bar_list )
    {
        slog("Unable to allocate enough progress bar");
        return;
    }
    gf2d_pb_manager.count = count;

    atexit(gf2d_progress_bar_manager_close);
    gf2d_progress_bar_manager_init_all();
}

void gf2d_progress_bar_manager_init_all()
{
    int i;
    ProgressBar *pb = NULL;

    for(i = 0; i < gf2d_pb_manager.count; i++)
    {
        pb = &gf2d_pb_manager.progress_bar_list[i];   
        pb->_background = gf2d_render_ent_new(NULL);
        pb->_foreground = gf2d_render_ent_new(NULL);
    }
}

void gf2d_progress_bar_manager_close()
{
    int i;
    ProgressBar *pbar = NULL;

    for(i = 0; i < gf2d_pb_manager.count; i++)
    {
        pbar = &gf2d_pb_manager.progress_bar_list[i];
        gf2d_progress_bar_free(pbar);
        if( pbar->_background ) free(pbar->_background);
        if( pbar->_foreground ) free(pbar->_foreground);
    }

    if( gf2d_pb_manager.progress_bar_list ) free(gf2d_pb_manager.progress_bar_list);
}

ProgressBar *gf2d_progress_bar_get_new()
{
    int i;
    ProgressBar *pb = NULL;

    for(i = 0; i < gf2d_pb_manager.count; i++)
    {
        pb = &gf2d_pb_manager.progress_bar_list[i];
        if( pb->_inuse ) continue;

        pb->_inuse = 1;

        return pb;
    }

    return NULL;
}

ProgressBar *gf2d_progress_bar_new(Vector4D *backColor, Vector2D backSize, Vector4D *foreColor, Vector2D foreSize)
{
    ProgressBar *pb = NULL;

    pb = gf2d_progress_bar_get_new();
    if(!pb)
    {
        slog("No progress bars available");
        return NULL;
    }

    pb->_background->sprite = gf2d_sprite_manipulation_get_default_solid();
    if(backColor) vector4d_copy(pb->_background->colorShift, (*backColor));
    pb->_foreground->sprite = gf2d_sprite_manipulation_get_default_solid();
    if(foreColor) vector4d_copy(pb->_foreground->colorShift, (*foreColor));

    vector2d_copy(pb->_background->scale, backSize);
    vector2d_copy(pb->_foreground->scale, foreSize);
    vector2d_copy(pb->backSize, backSize);
    vector2d_copy(pb->foreSize, foreSize);

    return pb;
}

void gf2d_progress_bar_render(ProgressBar *pbar)
{
    Vector2D back = {0};
    Vector2D fore = {0};
    if(!pbar) return;

    if(pbar->_background) 
    {
        back = pbar->_background->position;
        vector2d_sub(pbar->_background->position, pbar->_background->position, gf2d_camera_get_displaced_position(pbar->_background->position));
        vector2d_add(pbar->_background->position, pbar->_background->position, back);
    }
    if(pbar->_foreground) 
    {
        fore = pbar->_foreground->position;
        vector2d_sub(pbar->_foreground->position, pbar->_foreground->position, gf2d_camera_get_displaced_position(pbar->_foreground->position));
        vector2d_add(pbar->_foreground->position, pbar->_foreground->position, fore);
    }
    gf2d_render_ent_draw(pbar->_background);
    gf2d_render_ent_draw(pbar->_foreground);
    if(pbar->_background) pbar->_background->position = back;
    if(pbar->_foreground) pbar->_foreground->position = fore;
}

void gf2d_progress_bar_free(ProgressBar *pbar)
{
    RenderEntity *bg = NULL;
    RenderEntity *fore = NULL;

    if(!pbar) return;

    bg = pbar->_background;
    fore = pbar->_foreground;

    gf2d_render_ent_free(bg);
    gf2d_render_ent_free(fore);

    memset(pbar, 0, sizeof(ProgressBar));

    pbar->_background = bg;
    pbar->_foreground = fore;
}

void gf2d_progress_bar_set_position_and_offset(ProgressBar *pbar, Vector2D position, Vector2D offset)
{
    if(!pbar) return;

    if(pbar->_background) vector2d_copy(pbar->_background->position, position);
    if(pbar->_foreground) vector2d_add(pbar->_foreground->position, position, offset);
}

void gf2d_progress_bar_set_max_value(ProgressBar *pbar, float value)
{
    if(!pbar) return;
    pbar->_maxValue = value;
    gf2d_progress_bar_set_value(pbar, pbar->_value);
}

void gf2d_progress_bar_set_value(ProgressBar *pbar, float value)
{
    if(!pbar) return;

    pbar->_value = value;

    if(pbar->_foreground)
    {
        pbar->_foreground->scale.x = pbar->foreSize.x * value / pbar->_maxValue;
    }
}