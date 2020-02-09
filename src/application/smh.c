#include "smh.h"
#include "simple_logger.h"

#include "gf2d_scene.h"
#include "gf2d_physics_entity.h"
#include "gf2d_tilemap.h"

void smh_awake()
{
    Tilemap *tm = NULL;
    uint32_t w = 8, h = 8;
    uint32_t map[8 * 8] = {
         0,  1,  2,  3,  4,  5,  6,  7,
         8,  9, 10, 11, 12, 13, 14, 15, 
        13, 12,  6,  9,  5,  1,  4,  2,
        13, 15,  7,  8, 12, 12, 12, 14,
         0,  1,  2,  3,  4,  5,  6,  7,
         8,  9, 10, 11, 12, 13, 14, 15, 
        13, 12,  6,  9,  5,  1,  4,  2,
        13, 15,  7,  8, 12, 12, 12, 14,
    };

    tm = gf2d_tilemap_load( gf2d_sprite_load_all("images/pointer.png", 32, 32, 16), map, w, h );
    gf2d_scene_add_to_drawables(tm, DET_TMAP);
    gf2d_scene_remove_from_drawables(tm);
    gf2d_tilemap_free(tm);
}