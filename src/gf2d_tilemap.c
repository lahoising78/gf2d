#include "gf2d_tilemap.h"
#include "gf2d_graphics.h"
#include "simple_logger.h"
#include "gf2d_camera.h"
#include "gf2d_main.h"

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

    if(gf2d_tilemap_manager.tilemaps) free(gf2d_tilemap_manager.tilemaps);
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

Tilemap *gf2d_tilemap_create(Sprite *sprite, uint32_t *map, CollisionShape *solidMap, uint32_t w, uint32_t h)
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
                tile->body = solidMap[ tile->id ];
            }
        }
    }
    tilemap->w = w;
    tilemap->h = h;

    return tilemap;
}

Tilemap *gf2d_tilemap_load_from_file(const char *filename)
{
    Tilemap *tilemap = NULL;
    SJson *json = NULL;

    json = sj_load(filename);
    tilemap = gf2d_tilemap_load(json);
    sj_free(json);

    return tilemap;
}

Tilemap *gf2d_tilemap_load(SJson *json)
{
    Tilemap *tilemap = NULL;
    Sprite *spriteSheet = NULL;
    uint32_t *map = NULL;
    CollisionShape *solid = NULL;
    uint32_t w = 0;
    uint32_t h = 0;
    uint32_t maxTileId = 0;
    int i;

    SJson *obj = NULL;
    if(!json) return NULL;

    if( sj_is_string(json) )
        return gf2d_tilemap_load_from_file( sj_get_string_value(json) );

    slog("load tilemap");

    spriteSheet = gf2d_json_sprite( sj_object_get_value(json, "sprite") );

    w = gf2d_json_uint32(sj_object_get_value(json, "width"));
    h = gf2d_json_uint32(sj_object_get_value(json, "height"));

    map = (uint32_t*)gfc_allocate_array(sizeof(uint32_t), w*h);
    if(map)
    {
        obj = sj_object_get_value(json, "map");
        if(!obj || !sj_is_array(obj))
        {
            free(map);
            gf2d_sprite_free(spriteSheet);
            return NULL;
        }

        for(i = 0; i < sj_array_get_count(obj); i++)
        {
            map[i] = gf2d_json_uint32(sj_array_get_nth(obj, i));
            if(map[i]+1 > maxTileId) maxTileId = map[i]+1;
        }
    }

    solid = (CollisionShape*)gfc_allocate_array(sizeof(CollisionShape), maxTileId);
    if(solid)
    {
        obj = sj_object_get_value(json, "solid");
        if(!obj || !sj_is_array(obj) || maxTileId < sj_array_get_count(obj) )
        {
            free(map);
            free(solid);
            gf2d_sprite_free(spriteSheet);
            return NULL;
        }

        for(i = 0; i < maxTileId; i++)
        {
            solid[i] = gf2d_collision_shape_load( sj_array_get_nth(obj, i) );
        }
    }

    tilemap = gf2d_tilemap_create(spriteSheet, map, solid, w, h);

    if(map) free(map);
    if(solid) free(solid);

    return tilemap;
}

void gf2d_tilemap_free(Tilemap *tilemap)
{
    if(!tilemap) return;

    if( tilemap->tiles ) free(tilemap->tiles);
    if( tilemap->spriteSheet ) gf2d_sprite_free(tilemap->spriteSheet);

    memset(tilemap, 0, sizeof(Tilemap));
}

void gf2d_tilemap_render(Tilemap *tilemap)
{
    int i, j;
    Tile *tile = NULL;
    Vector2D finalPosition = {0};
    Vector2D view = {0};
    if(!tilemap) return;

    view = gf2d_camera_get_view();
    for(i = 0; i < tilemap->h; i++)
    {
        for(j = 0; j < tilemap->w; j++)
        {
            tile = &tilemap->tiles[i * tilemap->w + j];
            if(tile->id == 0) continue;

            finalPosition = gf2d_camera_get_displaced_position(tile->_pos);
            if (finalPosition.x < -(float)tilemap->spriteSheet->frame_w || finalPosition.x > view.x ||
                finalPosition.y < -(float)tilemap->spriteSheet->frame_h || finalPosition.y > view.y)
            {
                continue;
            }

            gf2d_sprite_draw(
                tilemap->spriteSheet,
                gf2d_camera_get_displaced_position(tile->_pos),
                NULL, NULL, NULL, NULL, NULL,
                tile->id-1
            );

            if( gf2d_main_get_draw_collisions() )
            {
                vector2d_add(tile->body.position, tile->body.position, tile->_pos);
                gf2d_collision_draw(&tile->body);
                vector2d_sub(tile->body.position, tile->body.position, tile->_pos);
            }
                
        }
    }
}

Vector2D gf2d_tilemap_world_to_map(const Tilemap *tilemap, Vector2D position)
{
    Vector2D ret = {0};
    if(!tilemap || !tilemap->spriteSheet) return vector2d(-1.0f, -1.0f);

    ret.x = floorf( position.x/tilemap->spriteSheet->frame_w );
    ret.y = floorf( position.y/tilemap->spriteSheet->frame_h );

    if( ret.x < 0.0f )
        ret.x = -3.0f;
    else if( ret.x >= (float)tilemap->w )
        ret.x = -2.0f;

    if( ret.y < 0.0f )
        ret.y = -3.0f;
    else if( ret.y >= (float)tilemap->h )
        ret.y = -2.0f;

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

uint8_t gf2d_tilemap_save(Tilemap *tmap)
{
    SJson *json = NULL;
    SJson *val = NULL;
    SJson *obj = NULL;
    SJson *col = NULL;
    uint32_t i, j, k; 
    uint32_t maxId = 0;
    uint8_t found = 0;
    Tile *tile = NULL;
    char buf[128];
    if(!tmap) return 0;

    json = sj_object_new();
    if(!json) return 0;

    obj = sj_object_new();
    if(obj)
    {
        val = sj_new_str(tmap->spriteSheet->filepath);
        sj_object_insert(obj, "filename", val);

        val = sj_new_int((int)tmap->spriteSheet->frame_w);
        sj_object_insert(obj, "frameWidth", val);

        val = sj_new_int((int)tmap->spriteSheet->frame_h);
        sj_object_insert(obj, "frameHeight", val);

        val = sj_new_int((int)tmap->spriteSheet->frames_per_line);
        sj_object_insert(obj, "framesPerLine", val);

        sj_object_insert(json, "sprite", obj);
    }

    obj = sj_new_int(tmap->w);
    if(obj) sj_object_insert(json, "width", obj);

    obj = sj_new_int(tmap->h);
    if(obj) sj_object_insert(json, "height", obj);

    obj = sj_array_new();
    if(obj)
    {
        for(i = 0; i < tmap->h; i++)
        {
            for(j = 0; j < tmap->w; j++)
            {
                k = i * tmap->w + j;
                if( tmap->tiles[k].id > maxId ) maxId = tmap->tiles[ k ].id;
                val = sj_new_int((int)tmap->tiles[ k ].id);
                sj_array_append(obj, val);
            }
        }

        sj_object_insert(json, "map", obj);
    }
    
    obj = sj_array_new();
    if(obj)
    {
        for(k = 0; k <= maxId; k++)
        {
            found = 0;
            for(i = 0; i < tmap->h; i++)
            {
                for(j = 0; j < tmap->w; j++)
                {
                    tile = &tmap->tiles[i * tmap->w + j];
                    
                    if( tile->id == k )
                    {
                        val = sj_object_new();

                        col = sj_array_new();
                        gf2d_json_vec2d_save(col, tile->body.position);
                        sj_object_insert( val, "position", col );

                        col = sj_array_new();
                        gf2d_json_vec2d_save(col, tile->body.dimensions.wh);
                        sj_object_insert( val, "dimensions", col );

                        gf2d_collision_shape_type_to_string(buf, tile->body.shapeType);
                        col = sj_new_str( buf );
                        sj_object_insert( val, "shapeType", col );

                        sj_array_append(obj, val);
                        found = 1;
                        break;
                    }
                }
                if(found) break;
            }
            if(found) continue;

            val = sj_object_new();

            col = sj_array_new();
            gf2d_json_vec2d_save(col, vector2d(0.0f, 0.0f));
            sj_object_insert( val, "position", col );

            col = sj_array_new();
            gf2d_json_vec2d_save(col, vector2d(0.0f, 0.0f));
            sj_object_insert( val, "dimensions", col );

            gf2d_collision_shape_type_to_string(buf, CST_BOX);
            col = sj_new_str( buf );
            sj_object_insert( val, "shapeType", col );

            sj_array_append(obj, val);
        }

        sj_object_insert(json, "solid", obj);
    }

    sj_save(json, "test.json");
    slog("save file %s", "test.json");
    sj_free(json);
    return 1;
}