#include "smh.h"
#include "simple_logger.h"

#include "gf2d_scene.h"
#include "player.h"

void smh_awake()
{
    player_create( gf2d_physics_entity_get_by_name("punti") );
}