#include "simple_logger.h"
#include "game_object.h"
#include "gf2d_scene.h"
#include "speed_boost.h"

typedef struct
{
    HAZARD_COMMON

    float ttf;
    float multiplier;
} SpeedBoostConfig;

static SpeedBoostConfig config = {0};
float speedMultiplier = 1.0;

typedef enum
{
    SPEED_STATE_PICKUP = 0,
    SPEED_STATE_ACTIVE
} SpeedBoostState;

void speed_boost_update(Entity *self);
void speed_boost_touch(Entity *self, Entity *other);

void speed_boost_config(const char *filepath)
{
    HAZARD_CONFIG_BEGIN("speed boost")

    sj_get_float_value( sj_object_get_value(json, "multiplier"), &config.multiplier );
    sj_get_float_value( sj_object_get_value(json, "ttf"), &config.ttf );

    gf2d_physics_entity_free(phys);
    sj_free(json);
}

HAZARD_NEW(speed_boost)

void speed_boost_init(PhysicsEntity *self)
{
    HAZARD_INIT_BEGIN(self)

    if(gobj)
    {
        gobj->state = SPEED_STATE_PICKUP;
        gobj->health = config.ttf;
    }

    self->entity->update = speed_boost_update;
    self->entity->touch = speed_boost_touch;
}

extern float frameTime;
void speed_boost_update(Entity *self)
{
    GameObject *gobj = NULL;
    if(!self) return;

    gobj = (GameObject*)self->abstraction;
    if(!gobj) return;
    if(gobj->state != SPEED_STATE_ACTIVE) return;

    gobj->health -= frameTime;
    if(gobj->health <= 0.0f) 
    {
        speedMultiplier = 1.0f;
        gf2d_scene_remove_from_drawables(gobj->selfPhys);
        game_object_free(gobj);
        return;
    }

    speedMultiplier = config.multiplier;
    if(!config.player) return;
    vector2d_copy(self->position, config.player->entity->position);
}

void speed_boost_touch(Entity *self, Entity *other)
{
    GameObject *gobj = NULL;
    HAZARD_TOUCH_CHECK(self, other)

    gobj = (GameObject*)self->abstraction;
    if(!gobj) return;

    switch (gobj->state)
    {
    case SPEED_STATE_PICKUP:
        if(config.player->entity != other) return;

        gobj->state = SPEED_STATE_ACTIVE;

        break;

    default:
        break;
    }
}
