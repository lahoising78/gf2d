#ifndef _GF2D_TILEMAP_H_
#define _GF2D_TILEMAP_H_

#include "gf2d_render_ent.h"
#include "gf2d_collision.h"

typedef struct
{
    Vector2D            _pos;
    uint32_t            id;
    uint8_t             solid;
} Tile;

typedef struct
{
    Sprite              *spriteSheet;
    Tile                *tiles;
    uint32_t            w;
    uint32_t            h;
    uint8_t             _inuse;
} Tilemap;

void gf2d_tilemap_manager_init( uint32_t count );

Tilemap *gf2d_tilemap_load(Sprite *sprite, uint32_t *map, uint32_t w, uint32_t h);
void gf2d_tilemap_free(Tilemap *tilemap);
void gf2d_tilemap_render(Tilemap *tilemap);
Vector2D gf2d_tilemap_world_to_map(const Tilemap *tilemap, Vector2D position);
Vector2D gf2d_tilemap_map_to_world(const Tilemap *tilemap, Vector2D position);
const Tilemap *gf2d_tilemap_get_all();
const uint32_t gf2d_tilemap_get_count();

#endif