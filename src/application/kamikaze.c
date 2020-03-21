#include "simple_logger.h"
#include "game_object.h"
#include "kamikaze.h"

typedef struct
{
    ENEMY_COMMON
} KamikazeConfig;

static KamikazeConfig config = {0};

void kamikaze_config(const char *filepath)
{
    ENEMY_CONFIG_BEGIN(config, filepath)

    sj_free(json);
}

void kamikaze_init(PhysicsEntity *phys)
{
    ENEMY_INIT_BEGIN(config, phys)
}