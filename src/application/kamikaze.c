#include "simple_logger.h"
#include "game_object.h"
#include "kamikaze.h"

typedef struct
{
    ENEMY_COMMON
} KamikazeConfig;

static KamikazeConfig config = {0};

typedef enum
{
    KAMIKAZE_IDLE = 0,
    KAMIKAZE_ATTACKING,
    KAMIKAZE_CHARGING,
    KAMIKAZE_EXPLODING
} KamikazeState;

void kamikaze_update(Entity *self);
void kamikaze_think(Entity *self);
void kamikaze_damage(GameObject *self, GameObject *other);

void kamikaze_config(const char *filepath)
{
    ENEMY_CONFIG_BEGIN(config, filepath)

    sj_free(json);
}

void kamikaze_init(PhysicsEntity *phys)
{
    ENEMY_INIT_BEGIN(config, phys)

    if(gobj)
    {
        gobj->damage = kamikaze_damage;
    }

    phys->entity->update = kamikaze_update;
    phys->entity->think = kamikaze_think;
}

extern float frameTime;
void kamikaze_think(Entity *self)
{
    GameObject *gobj = NULL;
    if(!self) return;
    gobj = (GameObject*)self->abstraction;
    if(!gobj) return;

    if( game_object_player_in_area(gobj) )
    {
        if(gobj->state != KAMIKAZE_EXPLODING)
        {
        //     switch (gobj->state)
        //     {
        //     case KAMIKAZE_CHARGING:
                
        //         break;
            
        //     default:
                gobj->state = KAMIKAZE_ATTACKING;
        //         break;
        //     }
        }
    }
    else
    {
        gobj->state = KAMIKAZE_IDLE;
    }
}
void kamikaze_update(Entity *self)
{
    GameObject *gobj = NULL;
    if(!self) return;
    gobj = (GameObject*)self->abstraction;
    if(!gobj) return;

    if(gobj->state == KAMIKAZE_CHARGING) gobj->state = KAMIKAZE_ATTACKING;
    game_object_update(gobj);

    if(gobj->state != KAMIKAZE_CHARGING && gobj->state != KAMIKAZE_EXPLODING)
    {
        gobj->coolDown = 0.0f;
    }
    // else
    // {
        switch (gobj->state)
        {
        case KAMIKAZE_CHARGING:
            gobj->coolDown -= frameTime;
            if(gobj->coolDown <= 0.0f) 
            {
                gobj->state = KAMIKAZE_EXPLODING;
                gobj->coolDown = config.cooldown;
            }
            break;

        case KAMIKAZE_EXPLODING:
            gobj->coolDown -= frameTime;
            if(gobj->coolDown <= 0.0f)
            {
                game_object_free(gobj);
            }
            break;
        
        default:
            break;
        }
    // }

    slog("kamikaze state %d %.2f", gobj->state, gobj->coolDown);
}
void kamikaze_damage(GameObject *self, GameObject *other)
{
    if(!self || !other) return;
    if(other->selfPhys != config.player) return;

    switch (self->state)
    {
    case KAMIKAZE_ATTACKING:
        self->state = KAMIKAZE_CHARGING;
        if(self->coolDown <= 0.0f) 
        {
            self->coolDown = config.anticipation;
            slog("SET THE COOLDOWN %.2f", self->coolDown);
        }
        break;
    
    default:
        break;
    }
}