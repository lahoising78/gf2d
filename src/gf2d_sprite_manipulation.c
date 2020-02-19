#include "gf2d_graphics.h"
#include "simple_logger.h"
#include "gf2d_draw.h"
#include "gf2d_sprite_manipulation.h"

static Sprite *gf2d_solid = NULL;

Sprite *gf2d_sprite_manipulation_get_default_solid()
{
    if( !gf2d_solid )
        gf2d_solid = gf2d_sprite_manipulation_solid(255, 255, 255, 255);

    return gf2d_solid;
}

Sprite *gf2d_sprite_manipulation_solid(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    Sprite *sprite = NULL;
    SDL_Surface *surface = NULL;
    SDL_Renderer *renderer = NULL;

    sprite = gf2d_sprite_new();
    if(!sprite) return NULL;

    surface = gf2d_graphics_create_surface(1, 1);
    if(!surface) 
    {
        slog("Unable to create surface for sprite solid");
        gf2d_sprite_free(sprite);
        return NULL;
    }

    renderer = SDL_CreateSoftwareRenderer(surface);
    if(!renderer)
    {
        slog("Unable to create renderer for sprite solid");
        gf2d_sprite_free(sprite);
        SDL_FreeSurface(surface);
        return NULL;
    }

    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);

    surface = gf2d_graphics_screen_convert(&surface);
    if(!surface)
    {
        slog("Unable to convert surface for sprite solid");
        gf2d_sprite_free(sprite);
        SDL_DestroyRenderer(renderer);
    }

    sprite->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), surface);
    if(!sprite->texture)
    {
        slog("texture");
        SDL_FreeSurface(surface);
        SDL_DestroyRenderer(renderer);
        gf2d_sprite_free(sprite);
        return NULL;
    }

    sprite->frame_h = surface->h;
    sprite->frame_w = surface->w;
    sprite->frames_per_line = 1;
    snprintf(sprite->filepath, GFCLINELEN, "Solid_%u_%u_%u_%u", r, g, b, a);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
    return sprite;
}