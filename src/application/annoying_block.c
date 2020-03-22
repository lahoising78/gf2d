#include "simple_logger.h"
#include "game_object.h"
#include "annoying_block.h"

typedef struct
{
    ENEMY_COMMON

    float duration;
    float velocity;
} AnnoyingBlockConfig;

static AnnoyingBlockConfig config = {0};

typedef enum
{
    ANNOY_IDLE = 0,
    ANNOY_ATTACKING,
    ANNOY_RESTING
} AnnoyingBlockState;

float playerTimeout = 0.0f;

void annoy_think(Entity *self);
void annoy_update(Entity *self);
void annoy_damage(GameObject *self, GameObject *other);

void annoy_config(const char *filepath)
{
    ENEMY_CONFIG_BEGIN(config, filepath)

    sj_get_float_value( sj_object_get_value(json, "duration"), &config.duration );
    sj_get_float_value( sj_object_get_value(json, "duration"), &config.velocity );

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

extern float frameTime;
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
    float v = 0.0f;
    if(!self) return;
    gobj = (GameObject*)self->abstraction;
    if(!gobj) return;

    if(gobj->health <= 0.0f)
        game_object_free(gobj);

    game_object_update(gobj);

    if(playerTimeout > 0.0f)
        playerTimeout -= frameTime;

    if(gobj->state != ANNOY_ATTACKING)
    {
        self->velocity.x = 0.0f;
    }

    switch (gobj->state)
    {
    case ANNOY_ATTACKING:
        v = config.player->entity->position.x - self->position.x;
        self->velocity.x = fabsf(v) / v * config.velocity;
        break;
    
    case ANNOY_RESTING:
        gobj->coolDown -= frameTime;
        if(gobj->coolDown <= 0.0f)
        {
            gobj->coolDown = 0.0f;
            gobj->state = ANNOY_IDLE;
        }
        break;

    default:
        break;
    }

    // slog("annoy state %d", gobj->state);
}

void annoy_damage(GameObject *self, GameObject *other)
{
    if(!self || !other) return;
    if(other->selfPhys != config.player) return;

    switch (self->state)
    {
    case ANNOY_ATTACKING:
        self->state = ANNOY_RESTING;
        self->coolDown = config.cooldown;
        if(playerTimeout <= 0.0f) 
            playerTimeout = config.duration;
        break;
    
    default:
        break;
    }
}
