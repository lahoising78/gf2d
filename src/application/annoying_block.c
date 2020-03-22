#include "simple_logger.h"
#include "game_object.h"
#include "annoying_block.h"

typedef struct
{
    ENEMY_COMMON
} AnnoyingBlockConfig;

static AnnoyingBlockConfig config = {0};

typedef enum
{
    ANNOY_IDLE = 0,
    ANNOY_ATTACKING,
    ANNOY_RESTING
} AnnoyingBlockState;

void annoy_think(Entity *self);
void annoy_update(Entity *self);
void annoy_damage(GameObject *self, GameObject *other);

void annoy_config(const char *filepath)
{
    ENEMY_CONFIG_BEGIN(config, filepath)

    sj_free(json);
}

void annoy_init(PhysicsEntity *phys)
{
    ENEMY_INIT_BEGIN(config, phys)

    if(gobj)
    {
        gobj->damage = annoy_damage;
    }

    phys->entity->update = annoy_update;
    phys->entity->think = annoy_think;
}

void annoy_think(Entity *self)
{
    GameObject *gobj = NULL;
    if(!self) return;
    gobj = (GameObject*)self->abstraction;
    if(!gobj) return;

    if(gobj->state != ANNOY_RESTING)
    {
        if( game_object_player_in_area(gobj) )
        {
            gobj->state = ANNOY_ATTACKING;
        }
        else
        {
            gobj->state = ANNOY_IDLE;
        }
    }
}

void annoy_update(Entity *self)
{
    GameObject *gobj = NULL;
    if(!self) return;
    gobj = (GameObject*)self->abstraction;
    if(!gobj) return;

    slog("annoy state %d", gobj->state);
}

void annoy_damage(GameObject *self, GameObject *other)
{
    if(!self || !other) return;
    if(other->selfPhys != config.player) return;
}
