#include "simple_logger.h"
#include "gf2d_progress_bar.h"
#include "gf2d_sprite_manipulation.h"

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

ProgressBar *gf2d_progress_bar_new(Vector4D *backColor, Vector4D *foreColor)
{
    ProgressBar *pb = NULL;

    pb = gf2d_progress_bar_get_new();
    if(!pb)
    {
        slog("No progress bars available");
        return NULL;
    }

    pb->_background->sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    // pb->_background->sprite = gf2d_sprite_manipulation_get_default_solid();
    if(backColor) vector4d_copy(pb->_background->colorShift, (*backColor));
    pb->_foreground->sprite = gf2d_sprite_manipulation_get_default_solid();
    if(foreColor) vector4d_copy(pb->_foreground->colorShift, (*foreColor));

    return pb;
}

void gf2d_progress_bar_render(ProgressBar *pbar)
{
    if(!pbar) return;

    gf2d_render_ent_draw(pbar->_background);
    gf2d_render_ent_draw(pbar->_foreground);
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