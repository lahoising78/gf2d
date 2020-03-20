#include "game_object.h"
#include "burning_tire.h"

typedef struct
{
    ENEMY_COMMON


} BurningTireConfig;

static BurningTireConfig config = {0};

void burning_tire_config(const char *filepath)
{
    ENEMY_CONFIG_BEGIN(config, filepath)



    sj_free(json);
}

void burning_tire_init(PhysicsEntity *self)
{
    ENEMY_INIT_BEGIN(config, self)


}