#include "simple_logger.h"
#include "game_object.h"
#include "combat.h"
#include "burning_tire.h"

typedef struct
{
    ENEMY_COMMON


} BurningTireConfig;

static BurningTireConfig config = {0};

typedef enum
{
    TIRE_IDLE = 0,
    TIRE_ATTACKING,
    TIRE_REST
} BurningTireState;

void burning_tire_think(Entity *self);
void burning_tire_update(Entity *self);
void burning_tire_damage(GameObject *self, GameObject *other);

void burning_tire_config(const char *filepath)
{
    ENEMY_CONFIG_BEGIN(config, filepath)



    sj_free(json);
}

void burning_tire_init(PhysicsEntity *self)
{
    ENEMY_INIT_BEGIN(config, self)

    if(gobj)
    {
        gobj->damage = burning_tire_damage;
    }

    self->entity->think = burning_tire_think;
    self->entity->update = burning_tire_update;
}

extern float frameTime;
void burning_tire_think(Entity *self)
{
    GameObject *gobj = NULL;
    if(!self) return;

    gobj = (GameObject*) self->abstraction;
    if(!gobj) return;
    
    if(gobj->state != TIRE_REST)
    {
        if( game_object_player_in_area(gobj) )
        {
            gobj->state = TIRE_ATTACKING;
            // slog("in area");
        }
        else
        {
            // slog("not in area");
            gobj->state = TIRE_IDLE;
        }
    }
}

void burning_tire_update(Entity *self)
{
    GameObject *gobj = NULL;
    if(!self) return;
    gobj = (GameObject*)self->abstraction;
    if(!gobj) return;

    game_object_update(gobj);
    switch (gobj->state)
    {
    case TIRE_ATTACKING:
        self->velocity.x = config.player->entity->position.x - self->position.x;
        if(gobj->selfPhys->_onFloor)
        {
            self->velocity.y = -10.0f;
        }

        break;

    case TIRE_REST:
        gobj->coolDown -= frameTime;
        if(gobj->coolDown <= 0.0f)
        {
            gobj->state = TIRE_IDLE;
            gobj->coolDown = 0.0f;
        }
        break;
    
    default:
        break;
    }
}

void burning_tire_damage(GameObject *self, GameObject *other)
{
    if(!self || !other) return;
    if(other->selfPhys != config.player) return;

    if( combat_do_damage(self, other, config.damage, config.hitstun) )
    {
        self->state = TIRE_REST;
        self->coolDown = config.cooldown;
    }
}