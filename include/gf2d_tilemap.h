#ifndef _GF2D_TILEMAP_H_
#define _GF2D_TILEMAP_H_

#include "gf2d_render_ent.h"
#include "gf2d_collision.h"
#include "gf2d_json.h"

typedef struct
{
    Vector2D            _pos;
    uint32_t            id;
    CollisionShape      body;
} Tile;

typedef struct
{
    Sprite              *spriteSheet;
    Tile                *tiles;
    uint32_t            w;
    uint32_t            h;
    uint8_t             _inuse;
} Tilemap;

/** 
 * @brief initialize the tilemap manager
 * @param count how many tilemaps to initialize
 */
void gf2d_tilemap_manager_init( uint32_t count );

/** 
 * @brief load and return a tilemap
 * @param sprite the spritesheet that holds the tile textures
 * @param map an array of unsigned integers containing the id of each tile
 * @note a tile id of 0 means null, and tile id of 2 means it the uses second texture in spritesheet
 * @param solidMad an array of numbers that says if a tile texture is solid or not. 
 * @note This goes according to tile id. e.g. solidMap[2] == 1 means tile with the third texture in spritesheet is solid
 * @param w how many tiles to have horizontally
 * @param h how many tiles to have vertically
 * @return a new tilemap with specified properties
 */
Tilemap *gf2d_tilemap_create(Sprite *sprite, uint32_t *map, CollisionShape *solidMap, uint32_t w, uint32_t h);
uint8_t gf2d_tilemap_save(Tilemap *tmap, char *fileOutput);

Tilemap *gf2d_tilemap_load_from_file(const char *filename);
Tilemap *gf2d_tilemap_load(SJson *json);

/** 
 * @brief free the tilemap
 * @param tilemap the tilemap to free
 */
void gf2d_tilemap_free(Tilemap *tilemap);

/** 
 * @brief render the tilemap
 * @param tilemap the tilemap to be rendered
 */
void gf2d_tilemap_render(Tilemap *tilemap);

/** 
 * @brief convert a world position to the position in the tilemap
 * @param tilemap the tilemap to compare against
 * @param position the position to get map coordinate from
 * @return vector with mag -1 if tilemap is null or doesn't have a sprite
 * @return vector with mag -2 if position is outside the map
 * @return vector indicating which tile in the tilemap the position is in
 */
Vector2D gf2d_tilemap_world_to_map(const Tilemap *tilemap, Vector2D position);

/** 
 * @brief convert a map coordinate to world position
 * @param tilemap the tileamp to compare against
 * @param position the coordinate to get world position from
 * @return vector with mag -1 if tilemap is null or doesn't have a sprite
 * @return vector with mag -2 if position is outside the map
 * @return vector indicating world position of the tile coordinate
 */
Vector2D gf2d_tilemap_map_to_world(const Tilemap *tilemap, Vector2D position);

/** 
 * @brief get an array containing all tilemaps, even the ones that are not being used
 * @return an array containing all tilemaps, even the ones that are not being used
 */
const Tilemap *gf2d_tilemap_get_all();

/** 
 * @brief get how many tilemaps can be used
 * @return a number containing how many tilemaps can and/or are being used
 */
const uint32_t gf2d_tilemap_get_count();

#endif