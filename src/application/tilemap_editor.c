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

    uint8_t initialized;
} TilemapEditor;

static TilemapEditor tilemap_editor = {0};

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
    if(tilemap_editor.panel)
    {
        gf2d_render_ent_free(tilemap_editor.panel);
        tilemap_editor.panel = NULL;
    }

    if(tilemap_editor.tilemap)
    {
        gf2d_tilemap_free(tilemap_editor.tilemap);
        tilemap_editor.tilemap = NULL;
    }

    tmap_controller_free(&tilemap_editor.width_control);
    tmap_controller_free(&tilemap_editor.height_control);
    tmap_controller_free(&tilemap_editor.tile_control);
    if(tilemap_editor.tile_sprite) 
    {
        gf2d_render_ent_free(tilemap_editor.tile_sprite);
        free(tilemap_editor.tile_sprite);
        tilemap_editor.tile_sprite = NULL;
    }
}

void tilemap_editor_set_filename(const char *filename)
{
    if(!filename) return;
    snprintf(tilemap_editor.filename, 4096, "application/scenes/%s", filename);
}

void tilemap_editor_awake()
{
    UIComponent *label = NULL;
    UIComponent *btn = NULL;
    const uint32_t buffer_size = 16;
    char buf[ buffer_size ];

    slog("%s", tilemap_editor.filename);

    tilemap_editor.tilemap = gf2d_tilemap_load_from_file(tilemap_editor.filename);
    if(!tilemap_editor.tilemap) 
    {
        slog("no filemap found with name %s", tilemap_editor.filename);
        return;
    }
    gf2d_scene_add_to_drawables(tilemap_editor.tilemap, DET_TMAP);

    /* CREATE PANEL IF NOT CREATED YET */
    if(!tilemap_editor.panel)
    {
        tilemap_editor.panel = gf2d_render_ent_new( gf2d_sprite_manipulation_get_default_solid() );
        tilemap_editor.panel->colorShift = vector4d(100.0f, 100.0f, 100.0f, 255.0f);
        tilemap_editor.panel->position = vector2d(1000.0f, 0.0f);
        tilemap_editor.panel->scale = vector2d(200.0f, 720.0f);
        slog("created panel");
    }
    gf2d_scene_add_to_drawables(tilemap_editor.panel, DET_REND);

    #pragma region WIDTH DISPLAY
    /* WIDTH DISPLAY */
    snprintf(buf, buffer_size, "%u", tilemap_editor.tilemap->w);
    label = gf2d_ui_label_new(buf, NULL, 32, vector2d(1075.0f, 50.0f));
    gf2d_scene_add_to_drawables(label, DET_UI);
    tilemap_editor.width_control.disp = label;

    btn = gf2d_ui_button_new(
        gf2d_label_new( "<", NULL, 32, vector2d(1025.0f, 50.0f) ),
        NULL
    );
    gf2d_scene_add_to_drawables(btn, DET_UI);
    tilemap_editor.width_control.less = btn;

    btn = gf2d_ui_button_new(
        gf2d_label_new( ">", NULL, 32, vector2d(1169.0f, 50.0f) ),
        NULL
    );
    gf2d_scene_add_to_drawables(btn, DET_UI);
    tilemap_editor.width_control.more = btn;
    #pragma endregion

    #pragma region HEIGHT DISPLAY
    /* HEIGHT DISPLAY */
    snprintf(buf, buffer_size, "%u", tilemap_editor.tilemap->h);
    label = gf2d_ui_label_new(buf, NULL, 32, vector2d(1075.0f, 150.0f));
    gf2d_scene_add_to_drawables(label, DET_UI);
    tilemap_editor.height_control.disp = label;
    
    btn = gf2d_ui_button_new(
        gf2d_label_new( "<", NULL, 32, vector2d(1025.0f, 150.0f) ),
        NULL
    );
    gf2d_scene_add_to_drawables(btn, DET_UI);
    tilemap_editor.height_control.less = btn;

    btn = gf2d_ui_button_new(
        gf2d_label_new( ">", NULL, 32, vector2d(1169.0f, 150.0f) ),
        NULL
    );
    gf2d_scene_add_to_drawables(btn, DET_UI);
    tilemap_editor.height_control.more = btn;
    #pragma endregion

    #pragma region TILE SPRITE
    /* TILE SPRITE DISPLAY */
    if( tilemap_editor.tile_sprite )
    {
        gf2d_render_ent_free(tilemap_editor.tile_sprite);
        free(tilemap_editor.tile_sprite);
        tilemap_editor.tile_sprite = NULL;
    }
    tilemap_editor.tile_sprite = gf2d_render_ent_new( tilemap_editor.tilemap->spriteSheet );
    tilemap_editor.tile_sprite->position = vector2d(1075.0f, 350.0f);
    gf2d_scene_add_to_drawables(tilemap_editor.tile_sprite, DET_REND);
    
    btn = gf2d_ui_button_new(
        gf2d_label_new( "<", NULL, 32, vector2d(1025.0f, 350.0f) ),
        NULL
    );
    gf2d_scene_add_to_drawables(btn, DET_UI);
    tilemap_editor.tile_control.less = btn;

    btn = gf2d_ui_button_new(
        gf2d_label_new( ">", NULL, 32, vector2d(1169.0f, 350.0f) ),
        NULL
    );
    gf2d_scene_add_to_drawables(btn, DET_UI);
    tilemap_editor.tile_control.more = btn;
    #pragma endregion

    /* FINALIZE INITIALIZATION */
    if(!tilemap_editor.initialized)
    {
        atexit(tilemap_editor_close);
    }

    tilemap_editor.initialized = 1;
}