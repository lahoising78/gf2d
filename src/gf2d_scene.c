#include "gf2d_scene.h"
#include "simple_logger.h"

static Scene gf2d_scene = {0};

void gf2d_scene_load( uint32_t physics_entities_count, uint32_t entities, void (*scene_awake)() )
{
    slog("--== Loading new scene ==--");

    gf2d_physics_entity_manager_clean(0);
    gf2d_entity_manager_clean(0);
}