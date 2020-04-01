#include "two.h"
#include "player.h"
#include "gf2d_physics_entity.h"

void two_awake()
{
    player_create( gf2d_physics_entity_get_by_name("punti") );
}