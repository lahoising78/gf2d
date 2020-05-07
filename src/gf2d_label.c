#include <SDL_ttf.h>
#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "gf2d_label.h"
#include "gf2d_camera.h"

typedef struct
{
    Label *label_list;
    uint32_t count;
    SDL_Color defaultColor;
} LabelManager;

static LabelManager gf2d_label_manager = {0};

Label *gf2d_label_get_new();
void gf2d_label_set_display(Label *label);

void gf2d_label_manager_close();

void gf2d_label_manager_init( uint32_t count )
{
    gf2d_label_manager.label_list = (Label*)gfc_allocate_array(sizeof(Label), count);
    if(!gf2d_label_manager.label_list)
    {
        slog("Unable to initialize labels");
        return;
    }
    gf2d_label_manager.count = count;

    gf2d_label_manager.defaultColor.r = 255;
    gf2d_label_manager.defaultColor.g = 255;
    gf2d_label_manager.defaultColor.b = 255;
    gf2d_label_manager.defaultColor.a = 255;
}

void gf2d_label_manager_close()
{
    Label *label = NULL;
    int i;

    for(i = 0; i < gf2d_label_manager.count; i++)
    {
        label = &gf2d_label_manager.label_list[i];
        gf2d_label_free(label);
    }

    free( gf2d_label_manager.label_list );
}

Label *gf2d_label_get_new()
{
    Label *label = NULL;
    int i;

    for(i = 0; i < gf2d_label_manager.count; i++)
    {
        label = &gf2d_label_manager.label_list[i];
        if(label->_inuse) continue;

        label->_inuse = 1;

        return label;
    }

    slog("No more labels available");
    return NULL;
}

Label *gf2d_label_new(const char *text, TTF_Font *font, uint32_t fontSize, Vector2D position)
{
    Label *label = NULL;

    label = gf2d_label_get_new();
    if(!label) return NULL;    

    gfc_line_cpy(label->_text, text);

    label->font = font? font : gf2d_font_default();
    label->fontSize = fontSize;
    label->_display = gf2d_render_ent_new(NULL);
    if(!label->_display)
    {
        free(label);
        return NULL;
    }
    vector2d_copy(label->_display->position, position);

    gf2d_label_set_display(label);

    return label;
}

void gf2d_label_set_display(Label *label)
{
    Sprite *sprite = NULL;
    SDL_Surface *surface = NULL;
    SDL_Color color = {0};
    if(!label) return;

    if( label->_display )
    {
        color.r = label->_display->colorShift.x;
        color.g = label->_display->colorShift.y;
        color.b = label->_display->colorShift.z;
        color.a = label->_display->colorShift.w;
    }
    else
        color = gf2d_label_manager.defaultColor;

    sprite = gf2d_sprite_new();
    if(!sprite) 
    {
        slog("unable to get a sprite for label");
        return;
    }
    surface = TTF_RenderText_Solid( label->font, label->_text, color );
    if(!surface)
    {
        slog("Could not create surface for label sprite: %s", SDL_GetError());
        gf2d_sprite_free(sprite);
        return;
    }

    surface = gf2d_graphics_screen_convert(&surface);
    if(!surface)
    {
        slog("Could not convert surface for label");
        gf2d_sprite_free(sprite);
        return;
    }

    sprite->texture = SDL_CreateTextureFromSurface( gf2d_graphics_get_renderer(), surface );
    if(!sprite->texture)
    {
        slog("could not create texture from surface for label");
        gf2d_sprite_free(sprite);
        SDL_FreeSurface(surface);
        return;
    }

    SDL_SetTextureBlendMode(sprite->texture,SDL_BLENDMODE_BLEND);        
    SDL_UpdateTexture(sprite->texture, NULL, surface->pixels, surface->pitch);

    sprite->frame_w = surface->w;
    sprite->frame_h = surface->h;
    sprite->frames_per_line = 1;
    snprintf(sprite->filepath, GFCLINELEN, "%s", label->_text);

    label->_display->sprite = sprite;
    SDL_FreeSurface(surface);
}

void gf2d_label_render( Label *label )
{
    Vector2D pos = {0};
    if(!label) return;
    if(!label->_display) return;

    pos = label->_display->position;
    vector2d_sub(label->_display->position, label->_display->position, gf2d_camera_get_displaced_position(label->_display->position));
    vector2d_add(label->_display->position, label->_display->position, pos);
        gf2d_render_ent_draw( label->_display );
    vector2d_add(label->_display->position, label->_display->position, gf2d_camera_get_displaced_position(label->_display->position));
    label->_display->position = pos;
}

void gf2d_label_free(Label *label)
{
    if(!label) return;

    if(label->_display) gf2d_render_ent_free(label->_display);
    memset( label, 0, sizeof(Label) );
}

void gf2d_label_set_text_color(Label *label, Vector4D newColor)
{
    if(!label) return;
    vector4d_copy(label->_display->colorShift, newColor);
}