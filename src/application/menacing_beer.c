#include "game_object.h"
#include "menacing_beer.h"

typedef struct
{
    ENEMY_COMMON
} MenacingBeerConfig;

static MenacingBeerConfig config = {0};

void menacing_beer_config(const char *filepath)
{
    ENEMY_CONFIG_BEGIN(config, filepath)

    sj_free(json);
}

void menacing_beer_init(PhysicsEntity *phys)
{
    ENEMY_INIT_BEGIN(config, phys)
}