#include "smh.h"
#include "simple_logger.h"

#include "gf2d_scene.h"
#include "game_object.h"
#include "player.h"
#include "punti_jordan.h"
#include "sentry.h"
#include "land_mine.h"
#include "ground_spikes.h"
#include "stick_trap.h"
#include "toxic_bomb.h"
#include "scene_door.h"
#include "drone.h"
#include "speed_boost.h"
#include "burning_tire.h"
#include "menacing_beer.h"
#include "kamikaze.h"
#include "annoying_block.h"

void load_next_level(Entity *self, Entity *other)
{
    gf2d_scene_load_from_file("application/scenes/second_scene.json");
}

void init_all(const char *obj, void (*init_func)(PhysicsEntity*), uint32_t num)
{
    PhysicsEntity *s = NULL;
    int i;
    char name[GFCLINELEN];
    if(!obj || !init_func) return;

    for(i = 0; i < num; i++)
    {
        snprintf(name, GFCLINELEN, "%s%d", obj, i);
        slog("init %s", name);
        s = gf2d_physics_entity_get_by_name(name);
        init_func(s);   
    }
}

void smh_awake()
{
    GameObject *gobj = NULL;
    PhysicsEntity *s = NULL;
    char name[128];
    int i;

    punti_jordan_load("application/punti_jordan_anim_config.json");
    sentry_load_config("application/sentry_config.json");
    land_mine_config("application/land_mine_config.json");
    ground_spikes_config("application/ground_spikes_config.json");
    stick_trap_config("application/stick_trap_config.json");
    toxic_bomb_config("application/toxic_bomb_config.json");
    scene_door_config("application/scene_door_config.json");
    drone_config("application/drone_config.json");
    speed_boost_config("application/speed_boost_config.json");
    burning_tire_config("application/burning_tire_config.json");
    menacing_beer_config("application/menacing_beer_config.json");
    kamikaze_config("application/kamikaze_config.json");
    annoy_config("application/annoying_block_config.json");

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

    for(i = 0; i < 1; i++)
    {
        snprintf(name, GFCLINELEN, "spike%d", i);
        s = gf2d_physics_entity_get_by_name(name);
        ground_spikes_init(s);
    }

    for(i = 0; i < 1; i++)
    {
        snprintf(name, GFCLINELEN, "trap%d", i);
        s = gf2d_physics_entity_get_by_name(name);
        stick_trap_init(s);
    }

    for(i = 0; i < 1; i++)
    {
        snprintf(name, GFCLINELEN, "tox%d", i);
        s = gf2d_physics_entity_get_by_name(name);
        toxic_bomb_init(s);
    }

    s = gf2d_physics_entity_get_by_name("door");
    scene_door_init( s );
    gobj = (GameObject*)s->entity->abstraction;
    if(gobj)
    {
        gfc_line_cpy( gobj->buf, "application/scenes/second_scene.json" );
    }

    for(i = 0; i < 1; i++)
    {
        snprintf(name, GFCLINELEN, "drone%d", i);
        s = gf2d_physics_entity_get_by_name(name);
        drone_init(s);
    }

    init_all("speed", speed_boost_init, 1);
    init_all("tire", burning_tire_init, 1);
    init_all("beer", menacing_beer_init, 1);
    init_all("kaze", kamikaze_init, 1);
    init_all("block", annoy_init, 1);
}