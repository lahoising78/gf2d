#include "gf2d_tilemap.h"
#include "gf2d_graphics.h"
#include "simple_logger.h"

typedef struct
{
    Tilemap         *tilemaps;
    uint32_t        count;
} TilemapManager;

static TilemapManager gf2d_tilemap_manager = {0};

Tilemap *gf2d_tilemap_new();
void gf2d_tilemap_init(Tilemap *tilemap);

void gf2d_tilemap_manager_init( uint32_t count )
{
    gf2d_tilemap_manager.tilemaps = (Tilemap*)gfc_allocate_array(sizeof(Tilemap), count);
    if( !gf2d_tilemap_manager.tilemaps )
    {
        slog("not enough space for tilemaps");
        return;
    }
    memset(gf2d_tilemap_manager.tilemaps, 0, sizeof(Tilemap)*count);
    gf2d_tilemap_manager.count = count;
}

Tilemap *gf2d_tilemap_new()
{
    int i;
    Tilemap *tilemap = NULL;

    for(i = 0; i < gf2d_tilemap_manager.count; i++)
    {
        tilemap = &gf2d_tilemap_manager.tilemaps[i];
        if(tilemap->_inuse) continue;
        gf2d_tilemap_init(tilemap);
        return tilemap;
    }

    return NULL;
}

void gf2d_tilemap_init(Tilemap *tilemap)
{
    if(!tilemap) return;
    tilemap->_inuse = 1;
}

Tilemap *gf2d_tilemap_load(Sprite *sprite, uint32_t *map[], uint32_t w, uint32_t h)
{
    Tilemap *tilemap = NULL;
    SDL_Surface *surface = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Rect src = {0};
    SDL_Rect dst = {0};
    int i, j;

    if(!map) return NULL;
    if(!sprite) return NULL;

    tilemap = gf2d_tilemap_new();
    if(!tilemap)
    {
        slog("no more tilemaps available");
        return NULL;
    }

    tilemap->rend = gf2d_render_ent_new(NULL);
    if(!tilemap->rend)
    {
        slog("not enough memory to allocate render entity for tilemap");
        gf2d_tilemap_free(tilemap);
        return NULL;
    }

    tilemap->rend->sprite = gf2d_sprite_new();
    if(!tilemap->rend->sprite)
    {
        slog("no sprite available for tilemap");
        gf2d_tilemap_free(tilemap);
        return NULL;
    }
    if(sprite->texture)
        slog("there is a texture");

    surface = gf2d_graphics_create_surface(w * sprite->frame_w, h * sprite->frame_h);
    if(!surface)
    {
        slog("could not create surface for tilemap");
        gf2d_tilemap_free(tilemap);
        return NULL;
    }
    
    // renderer = gf2d_graphics_get_renderer();
    renderer = SDL_CreateSoftwareRenderer(surface);
    if(!renderer)
    {
        slog("could not create renderer for tilemap");
        SDL_FreeSurface(surface);
        gf2d_tilemap_free(tilemap);
        return NULL;
    }

    for(i = 0; i < h; i++)
    {
        for(j = 0; j < w; j++)
        {
            src.x = 0;
            src.y = 0;
            src.w = sprite->frame_w;
            src.h = sprite->frame_h;

            dst.x = sprite->frame_w * j;
            dst.y = sprite->frame_h * i;
            dst.w = sprite->frame_w;
            dst.h = sprite->frame_h;

            // slog("src: %d %d %d %d", src.x, src.y, src.w, src.h);
            // slog("dst: %d %d %d %d", dst.x, dst.y, dst.w, dst.h);

            SDL_RenderCopy(renderer, sprite->texture, &src, &dst);
        }
    }


    surface = gf2d_graphics_screen_convert(&surface);
    if (!surface)
    {
        slog("failed to create surface for tilemap");
        SDL_DestroyRenderer(renderer);
        gf2d_sprite_free(sprite);
        gf2d_tilemap_free(tilemap);
        return NULL;
    }

    tilemap->rend->sprite->texture = SDL_CreateTextureFromSurface(renderer, surface);
    if(!tilemap->rend->sprite->texture)
    {
        slog("unable to create texture from surface");
        SDL_DestroyRenderer(renderer);
        SDL_FreeSurface(surface);
        gf2d_tilemap_free(tilemap);
        return NULL;
    }

    SDL_SetTextureBlendMode(tilemap->rend->sprite->texture,SDL_BLENDMODE_BLEND);        
    SDL_UpdateTexture(tilemap->rend->sprite->texture,
                    NULL,
        surface->pixels,
        surface->pitch
    );

    tilemap->rend->sprite->frame_w = surface->w;
    tilemap->rend->sprite->frame_h = surface->h;
    sprite->frames_per_line = 1;
    gfc_line_cpy(sprite->filepath,"tilemap");
    tilemap->rend->frame = 0;

    gf2d_sprite_free(sprite);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surface);
    return tilemap;
}

void gf2d_tilemap_free(Tilemap *tilemap)
{
    if(!tilemap) return;

    if(tilemap->rend)
    {
        gf2d_render_ent_free(tilemap->rend);
        free(tilemap->rend);
    }

    memset(tilemap, 0, sizeof(Tilemap));
}

void gf2d_tilemap_render(Tilemap *tilemap)
{
    if(!tilemap) return;
    // slog("rendering the tilemap");
    gf2d_render_ent_draw(tilemap->rend);
}
