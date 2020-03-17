#include "simple_logger.h"
#include "game_object.h"
#include "drone.h"

typedef struct
{
    HAZARD_COMMON

    float ttf;
    Vector2D damage;
    CollisionShape area;
} DroneConfig;

static DroneConfig config = {0};

typedef enum
{
    DRONE_STATE_PICKUP =                0,
    DRONE_STATE_ACTIVE =                1
} DroneState;

void drone_touch(Entity *self, Entity *other);
void drone_update(Entity *self);

void drone_config(const char *filepath)
{
    HAZARD_CONFIG_BEGIN("drone")

    config.damage = gf2d_json_vector2d( sj_object_get_value(json, "dmg") );
    config.area = gf2d_collision_shape_load( sj_object_get_value(json, "dmgArea") );

    if(config.player)
    {
        if(config.area.shapeType == CST_BOX)
        {
            vector2d_add(config.area.dimensions.wh, config.area.dimensions.wh, config.player->modelBox.dimensions.wh);
        }
    }

    gf2d_physics_entity_free(phys);
    sj_free(json);
}

HAZARD_NEW(drone)

void drone_init(PhysicsEntity *self)
{
    HAZARD_INIT_BEGIN(self)

    if(gobj)
    {
        gobj->state = DRONE_STATE_PICKUP;
    }

    self->entity->update = drone_update;
    self->entity->touch =  drone_touch;
}

void drone_touch(Entity *self, Entity *other)
{
    GameObject *gobj = NULL;
    HAZARD_TOUCH_CHECK(self, other)
    
    gobj = (GameObject*)self->abstraction;
    if(!gobj) return;

    switch (gobj->state)
    {
    case DRONE_STATE_PICKUP:
        if(other != config.player->entity) return;
        gobj->state = DRONE_STATE_ACTIVE;
        gobj->selfPhys->modelBox = config.area;
        break;
    
    case DRONE_STATE_ACTIVE:
        if(other == config.player->entity) return;

        slog("touching entity");

        break;

    default:
        break;
    }
}

void drone_update(Entity *self)
{
    GameObject *gobj = NULL;
    if(!self) return;
    if(!config.player) return;

    gobj = (GameObject*)self->abstraction;
    if(!gobj) return;
    if(gobj->state != DRONE_STATE_ACTIVE) return;

    vector2d_copy(self->position, config.player->entity->position);
}
