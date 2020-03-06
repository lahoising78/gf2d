#include "smh.h"
#include "simple_logger.h"

#include "gf2d_scene.h"
#include "player.h"
#include "sentry.h"

void load_next_level(Entity *self, Entity *other)
{
    gf2d_scene_load_from_file("application/scenes/second_scene.json");
}

void smh_awake()
{
    PhysicsEntity *s = NULL;
    char name[128];
    int i;
    player_create( gf2d_physics_entity_get_by_name("punti") );

    for(i = 0; i < 1; i++)
    {
        snprintf(name, GFCLINELEN, "sentry%d", i);
        s = gf2d_physics_entity_get_by_name(name);
        sentry_init(s);
    }
}