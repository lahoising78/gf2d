#include <SDL_ttf.h>
#include "simple_logger.h"
#include "gf2d_graphics.h"
#include "gf2d_label.h"

void gf2d_label_set_display(Label *label);

Label *gf2d_label_new(const char *text, TTF_Font *font, uint32_t fontSize, Vector2D position)
{
    Label *label = NULL;

    label = (Label*) gfc_allocate_array(sizeof(Label), 1);
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
    label->_textColor = vector4d(255.0f, 255.0f, 255.0f, 255.0f);

    gf2d_label_set_display(label);

    return label;
}

void gf2d_label_set_display(Label *label)
{
    Sprite *sprite = NULL;
    SDL_Surface *surface = NULL;
    SDL_Color color = {0};
    if(!label) return;

    color.r = label->_textColor.x;
    color.g = label->_textColor.y;
    color.b = label->_textColor.z;
    color.a = label->_textColor.w;

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
    if(!label) return;

    gf2d_render_ent_draw( label->_display );
}

void gf2d_label_free(Label *label)
{
    if(!label) return;

    if(label->_display) gf2d_render_ent_free(label->_display);
}