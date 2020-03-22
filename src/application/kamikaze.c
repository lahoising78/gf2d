#include "simple_logger.h"
#include "combat.h"
#include "game_object.h"
#include "kamikaze.h"

typedef struct
{
    ENEMY_COMMON

    float velocity;
    CollisionShape explosion;
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

    config.explosion = gf2d_collision_shape_load( sj_object_get_value(json, "explosion") );
    sj_get_float_value( sj_object_get_value(json, "velocity"), &config.velocity );

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
            gobj->state = KAMIKAZE_ATTACKING;
        }
    }
    else
    {
        gobj->state = KAMIKAZE_IDLE;
        self->velocity.x = 0.0f;
    }
}
void kamikaze_update(Entity *self)
{
    GameObject *gobj = NULL;
    float v = 0.0f;
    if(!self) return;
    gobj = (GameObject*)self->abstraction;
    if(!gobj) return;

    game_object_update(gobj);

    if(gobj->state != KAMIKAZE_CHARGING && gobj->state != KAMIKAZE_EXPLODING)
    {
        gobj->coolDown = 0.0f;
        self->anim->rend->colorShift.w = 255;
    }
    switch (gobj->state)
    {
    case KAMIKAZE_ATTACKING:
        v = config.player->entity->position.x - self->position.x;
        self->velocity.x = fabsf( v ) / v * config.velocity;
        break;

    case KAMIKAZE_CHARGING:
        gobj->coolDown -= frameTime;
        self->anim->rend->colorShift.w = gobj->coolDown / config.anticipation * 255;
        if(gobj->coolDown <= 0.0f) 
        {
            gobj->state = KAMIKAZE_EXPLODING;
            gobj->coolDown = config.cooldown;
            self->anim->rend->position = config.explosion.position;
            vector2d_scale( self->anim->rend->scale, self->anim->rend->scale, config.explosion.dimensions.wh.x / (float)config.sprite->frame_w );
            gobj->hitbox = config.explosion;
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

    // slog("kamikaze state %d %.2f", gobj->state, gobj->coolDown);
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

    case KAMIKAZE_EXPLODING:
        combat_do_damage(self, other, config.damage, config.hitstun);
        break;
    
    default:
        break;
    }
}