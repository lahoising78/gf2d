#include "simple_logger.h"
#include "combat.h"
#include "game_object.h"
#include "menacing_beer.h"

typedef struct
{
    ENEMY_COMMON

    float velocity;
} MenacingBeerConfig;

static MenacingBeerConfig config = {0};

typedef enum
{
    BEER_IDLE = 0,
    BEER_ATTACKING,
    BEER_RESTING
} MenacingBeerState;

void menacing_beer_think(Entity *self);
void menacing_beer_update(Entity *self);
void menacing_beer_damage(GameObject *self, GameObject *other);

void menacing_beer_config(const char *filepath)
{
    ENEMY_CONFIG_BEGIN(config, filepath)

    sj_get_float_value( sj_object_get_value(json, "velocity"), &config.velocity );

    sj_free(json);
}

void menacing_beer_init(PhysicsEntity *phys)
{
    ENEMY_INIT_BEGIN(config, phys)

    if(gobj)
    {
        gobj->damage = menacing_beer_damage;
    }

    phys->entity->update = menacing_beer_update;
    phys->entity->think = menacing_beer_think;
}

extern float frameTime;
void menacing_beer_think(Entity *self)
{
    GameObject *gobj = NULL;
    if(!self) return;
    gobj = (GameObject*)self->abstraction;
    if(!gobj) return;

    if( gobj->state != BEER_RESTING )
    {
        if( game_object_player_in_area(gobj) )
        {
            gobj->state = BEER_ATTACKING;
        }
        else
        {
            gobj->state = BEER_IDLE;
        }
    }
}

void menacing_beer_update(Entity *self)
{
    GameObject *gobj = NULL;
    if(!self) return;
    gobj = (GameObject*)self->abstraction;
    if(!gobj) return;

    game_object_update(gobj);

    switch (gobj->state)
    {
    case BEER_ATTACKING:

        self->velocity.x = fabsf(config.player->entity->position.x - self->position.x) / (config.player->entity->position.x - self->position.x) * config.velocity;

        break;

    case BEER_RESTING:
        gobj->coolDown -= frameTime;
        if(gobj->coolDown <= 0.0f)
        {
            gobj->coolDown = 0.0f;
            gobj->state = BEER_IDLE;
        }
        break;
    
    default:
        break;
    }
}

void menacing_beer_damage(GameObject *self, GameObject *other)
{
    if(!self || !other) return;
    if(other->selfPhys != config.player) return;

    if( combat_do_damage(self, other, config.damage, config.hitstun) )
    {
        self->state = BEER_RESTING;
        self->coolDown = config.cooldown;
        vector2d_clear(self->self->velocity);
    }
}
