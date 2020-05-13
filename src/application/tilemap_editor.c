#include "simple_logger.h"
#include "gf2d_tilemap.h"
#include "tilemap_editor.h"
#include "gf2d_sprite_manipulation.h"
#include "gf2d_scene.h"
#include "gf2d_ui.h"

typedef struct
{
    UIComponent *less;
    UIComponent *disp;
    UIComponent *more;
} TmapController;

typedef struct 
{
    char filename[4096];
    Tilemap *tilemap;

    RenderEntity *panel;
    List *ui_list;

    TmapController width_control;
    TmapController height_control;
    
    TmapController tile_control;
    RenderEntity *tile_sprite;

    Entity *ui_event_updater;

    uint8_t initialized;
} TilemapEditor;

static TilemapEditor tilemap_editor_controller = {0};

void tmap_controller_update(TmapController *controller)
{
    if(!controller) return;

    gf2d_ui_update(controller->less);
    gf2d_ui_update(controller->more);
    gf2d_ui_update(controller->disp);
}

void tile_editor_width_less(Button *btn);
void tile_editor_width_more(Button *btn);
void tile_editor_height_less(Button *btn);
void tile_editor_height_more(Button *btn);
void tile_editor_tile_less(Button *btn);
void tile_editor_tile_more(Button *btn);

void tile_editor_ui_update(Entity *self)
{
    tmap_controller_update(&tilemap_editor_controller.width_control);
    tmap_controller_update(&tilemap_editor_controller.height_control);
    tmap_controller_update(&tilemap_editor_controller.tile_control);
}

void tmap_controller_free(TmapController *controller)
{
    if(!controller) return;

    if(controller->disp) gf2d_ui_free(controller->disp);
    if(controller->less) gf2d_ui_free(controller->less);
    if(controller->more) gf2d_ui_free(controller->more);

    memset(controller, 0, sizeof(TmapController));
}

void tilemap_editor_close()
{
    if(tilemap_editor_controller.panel)
    {
        gf2d_render_ent_free(tilemap_editor_controller.panel);
    }

    if(tilemap_editor_controller.tilemap)
    {
        gf2d_tilemap_free(tilemap_editor_controller.tilemap);
    }

    tmap_controller_free(&tilemap_editor_controller.width_control);
    tmap_controller_free(&tilemap_editor_controller.height_control);
    tmap_controller_free(&tilemap_editor_controller.tile_control);
    if(tilemap_editor_controller.tile_sprite) 
    {
        gf2d_render_ent_free(tilemap_editor_controller.tile_sprite);
        free(tilemap_editor_controller.tile_sprite);
    }

    if(tilemap_editor_controller.ui_event_updater)
    {
        gf2d_entity_free(tilemap_editor_controller.ui_event_updater);
    }

    memset(&tilemap_editor_controller, 0, sizeof(TilemapEditor));
}

void tilemap_editor_set_filename(const char *filename)
{
    if(!filename) return;
    snprintf(tilemap_editor_controller.filename, 4096, "application/scenes/%s", filename);
}

void tilemap_editor_awake()
{
    UIComponent *label = NULL;
    UIComponent *btn = NULL;
    const uint32_t buffer_size = 16;
    char buf[ buffer_size ];

    slog("%s", tilemap_editor_controller.filename);

    tilemap_editor_controller.tilemap = gf2d_tilemap_load_from_file(tilemap_editor_controller.filename);
    if(!tilemap_editor_controller.tilemap) 
    {
        slog("no filemap found with name %s", tilemap_editor_controller.filename);
        return;
    }
    gf2d_scene_add_to_drawables(tilemap_editor_controller.tilemap, DET_TMAP);

    /* CREATE PANEL IF NOT CREATED YET */
    if(!tilemap_editor_controller.panel)
    {
        tilemap_editor_controller.panel = gf2d_render_ent_new( gf2d_sprite_manipulation_get_default_solid() );
        tilemap_editor_controller.panel->colorShift = vector4d(100.0f, 100.0f, 100.0f, 255.0f);
        tilemap_editor_controller.panel->position = vector2d(1000.0f, 0.0f);
        tilemap_editor_controller.panel->scale = vector2d(200.0f, 720.0f);
        slog("created panel");
    }
    gf2d_scene_add_to_drawables(tilemap_editor_controller.panel, DET_REND);

    #pragma region WIDTH DISPLAY
    /* WIDTH DISPLAY */
    snprintf(buf, buffer_size, "%u", tilemap_editor_controller.tilemap->w);
    label = gf2d_ui_label_new(buf, NULL, 32, vector2d(1075.0f, 50.0f));
    gf2d_scene_add_to_drawables(label, DET_UI);
    tilemap_editor_controller.width_control.disp = label;

    btn = gf2d_ui_button_new(
        gf2d_label_new( "<", NULL, 32, vector2d(1025.0f, 50.0f) ),
        tile_editor_width_less
    );
    gf2d_scene_add_to_drawables(btn, DET_UI);
    tilemap_editor_controller.width_control.less = btn;

    btn = gf2d_ui_button_new(
        gf2d_label_new( ">", NULL, 32, vector2d(1169.0f, 50.0f) ),
        tile_editor_width_more
    );
    gf2d_scene_add_to_drawables(btn, DET_UI);
    tilemap_editor_controller.width_control.more = btn;
    #pragma endregion

    #pragma region HEIGHT DISPLAY
    /* HEIGHT DISPLAY */
    snprintf(buf, buffer_size, "%u", tilemap_editor_controller.tilemap->h);
    label = gf2d_ui_label_new(buf, NULL, 32, vector2d(1075.0f, 150.0f));
    gf2d_scene_add_to_drawables(label, DET_UI);
    tilemap_editor_controller.height_control.disp = label;
    
    btn = gf2d_ui_button_new(
        gf2d_label_new( "<", NULL, 32, vector2d(1025.0f, 150.0f) ),
        tile_editor_height_less
    );
    gf2d_scene_add_to_drawables(btn, DET_UI);
    tilemap_editor_controller.height_control.less = btn;

    btn = gf2d_ui_button_new(
        gf2d_label_new( ">", NULL, 32, vector2d(1169.0f, 150.0f) ),
        tile_editor_height_more
    );
    gf2d_scene_add_to_drawables(btn, DET_UI);
    tilemap_editor_controller.height_control.more = btn;
    #pragma endregion

    #pragma region TILE SPRITE
    /* TILE SPRITE DISPLAY */
    if( tilemap_editor_controller.tile_sprite )
    {
        gf2d_render_ent_free(tilemap_editor_controller.tile_sprite);
        free(tilemap_editor_controller.tile_sprite);
        tilemap_editor_controller.tile_sprite = NULL;
    }
    tilemap_editor_controller.tile_sprite = gf2d_render_ent_new( tilemap_editor_controller.tilemap->spriteSheet );
    tilemap_editor_controller.tile_sprite->position = vector2d(1075.0f, 350.0f);
    gf2d_scene_add_to_drawables(tilemap_editor_controller.tile_sprite, DET_REND);
    
    btn = gf2d_ui_button_new(
        gf2d_label_new( "<", NULL, 32, vector2d(1025.0f, 350.0f) ),
        tile_editor_tile_less
    );
    gf2d_scene_add_to_drawables(btn, DET_UI);
    tilemap_editor_controller.tile_control.less = btn;

    btn = gf2d_ui_button_new(
        gf2d_label_new( ">", NULL, 32, vector2d(1169.0f, 350.0f) ),
        tile_editor_tile_more
    );
    gf2d_scene_add_to_drawables(btn, DET_UI);
    tilemap_editor_controller.tile_control.more = btn;
    #pragma endregion

    if(!tilemap_editor_controller.ui_event_updater)
    {
        tilemap_editor_controller.ui_event_updater = gf2d_entity_new(NULL);
        tilemap_editor_controller.ui_event_updater->update = tile_editor_ui_update;
    }

    /* FINALIZE INITIALIZATION */
    if(!tilemap_editor_controller.initialized)
    {
        atexit(tilemap_editor_close);
    }

    tilemap_editor_controller.initialized = 1;
}

void tile_editor_width_less(Button *btn)
{
    if(!btn) return;


}

void tile_editor_width_more(Button *btn)
{
    if(!btn) return;
}

void tile_editor_height_less(Button *btn)
{
    if(!btn) return;
}

void tile_editor_height_more(Button *btn)
{
    if(!btn) return;
}

void tile_editor_tile_less(Button *btn)
{
    RenderEntity *rend = NULL;
    if(!btn) return;
    if(!tilemap_editor_controller.tile_sprite) return;

    rend = tilemap_editor_controller.tile_sprite;
    
    rend->frame--;
    if(rend->frame < 0) rend->frame = 0;
}

void tile_editor_tile_more(Button *btn)
{
    RenderEntity *rend = NULL;
    if(!btn) return;
    if(!tilemap_editor_controller.tile_sprite) return;

    rend = tilemap_editor_controller.tile_sprite;
    rend->frame++;
}