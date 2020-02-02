#include "smh.h"
#include "simple_logger.h"

#include "gf2d_scene.h"
#include "gf2d_physics_entity.h"
#include "gf2d_tilemap.h"

void smh_awake()
{
    Tilemap *tm = NULL;
    // RenderEntity *e = NULL;
    uint32_t map[32][32];
    uint32_t w = 32, h = 32;
    tm = gf2d_tilemap_load( gf2d_sprite_load_all("images/space_bug.png", 128, 128, 16), map, w, h );
    gf2d_scene_add_to_drawables(tm, DET_TMAP);
}