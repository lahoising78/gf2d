#include "smh.h"
#include "simple_logger.h"

#include "gf2d_scene.h"
#include "punti_jordan.h"

void smh_awake()
{
    punti_jordan( gf2d_physics_entity_get_by_name("punti") );
}