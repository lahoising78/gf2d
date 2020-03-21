#include "simple_logger.h"
#include "game_object.h"
#include "annoying_block.h"

typedef struct
{
    ENEMY_COMMON
} AnnoyingBlockConfig;

static AnnoyingBlockConfig config = {0};

void annoy_config(const char *filepath)
{
    ENEMY_CONFIG_BEGIN(config, filepath)

    sj_free(json);
}

void annoy_init(PhysicsEntity *phys)
{
    ENEMY_INIT_BEGIN(config, phys)
}