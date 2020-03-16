#include "smh.h"
#include "simple_logger.h"

#include "gf2d_scene.h"
#include "player.h"
#include "punti_jordan.h"
#include "sentry.h"
#include "land_mine.h"

void load_next_level(Entity *self, Entity *other)
{
    gf2d_scene_load_from_file("application/scenes/second_scene.json");
}

void smh_awake()
{
    PhysicsEntity *s = NULL;
    char name[128];
    int i;

    punti_jordan_load("application/punti_jordan_anim_config.json");
    sentry_load_config("application/sentry_config.json");
    land_mine_config("application/land_mine_config.json");

    player_create( gf2d_physics_entity_get_by_name("punti") );

    for(i = 0; i < 1; i++)
    {
        snprintf(name, GFCLINELEN, "sentry%d", i);
        s = gf2d_physics_entity_get_by_name(name);
        sentry_init(s);
    }

    for(i = 0; i < 1; i++)
    {
        snprintf(name, GFCLINELEN, "mine%d", i);
        s = gf2d_physics_entity_get_by_name(name);
        land_mine_init(s);
    }
}