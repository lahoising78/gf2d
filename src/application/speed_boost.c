#include "simple_logger.h"
#include "game_object.h"
#include "speed_boost.h"

typedef struct
{
    HAZARD_COMMON


} SpeedBoostConfig;

static SpeedBoostConfig config = {0};

void speed_boost_config(const char *filepath)
{
    HAZARD_CONFIG_BEGIN("speed boost")

    gf2d_physics_entity_free(phys);
    sj_free(json);
}

HAZARD_NEW(speed_boost)

void speed_boost_init(PhysicsEntity *self)
{
    HAZARD_INIT_BEGIN(self)


}