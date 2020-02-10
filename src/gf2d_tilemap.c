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
void gf2d_tilemap_manager_close();

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

    atexit(gf2d_tilemap_manager_close);
}

void gf2d_tilemap_manager_close()
{
    int i;
    Tilemap *tilemap = NULL;

    for(i = 0; i < gf2d_tilemap_manager.count; i++)
    {
        tilemap = &gf2d_tilemap_manager.tilemaps[i];
        gf2d_tilemap_free(tilemap);
    }

    free(gf2d_tilemap_manager.tilemaps);
    memset(&gf2d_tilemap_manager, 0, sizeof(TilemapManager));
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

Tilemap *gf2d_tilemap_load(Sprite *sprite, uint32_t *map, uint8_t *solidMap, uint32_t w, uint32_t h)
{
    Tilemap *tilemap = NULL;
    Tile *tile = NULL;
    int i, j;

    if(!map) return NULL;
    if(!sprite) return NULL;

    tilemap = gf2d_tilemap_new();
    if(!tilemap)
    {
        slog("no more tilemaps available");
        return NULL;
    }

    tilemap->spriteSheet = sprite;
    tilemap->tiles = (Tile*) gfc_allocate_array(sizeof(Tile), (size_t)(w * h));
    if(tilemap->tiles) 
    {
        memset(tilemap->tiles, 0, sizeof(Tile) * w * h);
        for(i = 0; i < h; i++)
        {
            for(j = 0; j < w; j++)
            {
                tile = &tilemap->tiles[i*w + j];
                tile->_pos = vector2d((float)(j*sprite->frame_w), (float)(i * sprite->frame_h));
                tile->id = map[ i*w + j ];
                tile->solid = solidMap[ tile->id ];
            }
        }
    }
    tilemap->w = w;
    tilemap->h = h;

    return tilemap;
}

void gf2d_tilemap_free(Tilemap *tilemap)
{
    if(!tilemap) return;

    if( tilemap->tiles ) free(tilemap->tiles);
    gf2d_sprite_free(tilemap->spriteSheet);

    memset(tilemap, 0, sizeof(Tilemap));
}

void gf2d_tilemap_render(Tilemap *tilemap)
{
    int i, j;
    Tile *tile = NULL;
    if(!tilemap) return;

    for(i = 0; i < tilemap->h; i++)
    {
        for(j = 0; j < tilemap->w; j++)
        {
            tile = &tilemap->tiles[i * tilemap->w + j];
            if(tile->id == 0) continue;
            gf2d_sprite_draw(
                tilemap->spriteSheet,
                tile->_pos,
                NULL, NULL, NULL, NULL, NULL,
                tile->id-1
            );
        }
    }
}

Vector2D gf2d_tilemap_world_to_map(const Tilemap *tilemap, Vector2D position)
{
    Vector2D ret = {0};
    if(!tilemap || !tilemap->spriteSheet) return vector2d(-1.0f, -1.0f);

    ret.x = floorf( position.x/tilemap->spriteSheet->frame_w );
    ret.y = floorf( position.y/tilemap->spriteSheet->frame_h );

    if( ret.x < 0.0f || ret.x >= (float)tilemap->w ||
        ret.y < 0.0f || ret.y >= (float)tilemap->h )
        return vector2d(-2.0f, -2.0f);

    return ret;
}

Vector2D gf2d_tilemap_map_to_world(const Tilemap *tilemap, Vector2D position)
{
    Vector2D ret = {0};
    if(!tilemap || !tilemap->spriteSheet) return vector2d(-1.0f, -1.0f);

    if( position.x < 0.0f || position.x >= (float)tilemap->spriteSheet->frame_w ||
        position.y < 0.0f || position.y >= (float)tilemap->spriteSheet->frame_h )
        return vector2d(-2.0f, -2.0f);

    ret.x = position.x * (float)tilemap->spriteSheet->frame_w;
    ret.y = position.y * (float)tilemap->spriteSheet->frame_h;

    return ret;
}

const Tilemap *gf2d_tilemap_get_all()
{
    return gf2d_tilemap_manager.tilemaps;
}

const uint32_t gf2d_tilemap_get_count()
{
    return gf2d_tilemap_manager.count;
}