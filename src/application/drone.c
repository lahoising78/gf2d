#include "simple_logger.h"
#include "game_object.h"
#include "drone.h"

typedef struct
{
    HAZARD_COMMON
} DroneConfig;

static DroneConfig config = {0};

void drone_config(const char *filepath)
{
    HAZARD_CONFIG_BEGIN("drone")

    gf2d_physics_entity_free(phys);
    sj_free(json);
}

HAZARD_NEW(drone)

void drone_init(PhysicsEntity *self)
{
    HAZARD_INIT_BEGIN(self)


}