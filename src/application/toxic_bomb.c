#include "simple_logger.h"
#include "game_object.h"
#include "toxic_bomb.h"

typedef struct
{
    HAZARD_COMMON
} ToxicBombConfig;

static ToxicBombConfig config = {0};

void toxic_bomb_config(const char *filepath)
{
    HAZARD_CONFIG_BEGIN("toxic bomb")

    gf2d_physics_entity_free(phys);
    sj_free(json);
}

HAZARD_NEW(toxic_bomb)

void toxic_bomb_init(PhysicsEntity *self)
{
    HAZARD_INIT_BEGIN(self)


}