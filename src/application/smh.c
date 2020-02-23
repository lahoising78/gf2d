#include "smh.h"
#include "simple_logger.h"

#include "gf2d_scene.h"
#include "player.h"
#include "punti_jordan.h"

void smh_awake()
{
    punti_jordan_load("application/punti_jordan_anim_config.json");
    player_create( gf2d_physics_entity_get_by_name("punti") );
}