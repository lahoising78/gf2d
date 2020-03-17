#include "simple_logger.h"
#include "game_object.h"
#include "stick_trap.h"

typedef struct
{
    /* phys */
    Sprite                      *sprite;
    uint8_t                     canCollide;
    uint8_t                     useGravity;
    CollisionShape              shape;

    /* game object */
    float                       cooldown;
} StickTrapConfig;

static StickTrapConfig config = {0};

void stick_trap_config(const char *filepath)
{
    SJson *json = NULL;
    PhysicsEntity *phys = NULL;
    slog("stick trap load config");

    json = sj_load(filepath);
    if(!json) return;

    phys = gf2d_physics_entity_load( sj_object_get_value(json, "phys") );
    if(!phys)
    {
        slog("could not find placeholder to load config");
    }

    config.sprite = phys->entity->anim->rend->sprite;
    config.useGravity = phys->useGravity;
    config.canCollide = phys->canCollide;
    config.shape = phys->modelBox;
    slog("%u %u %u %.2f %.2f %.2f %.2f", config.sprite, config.useGravity, config.canCollide, 
        config.shape.position.x,
        config.shape.position.y,
        config.shape.dimensions.wh.x,
        config.shape.dimensions.wh.y
    );

    sj_free(json);
}

PhysicsEntity *stick_trap_new()
{
    PhysicsEntity *self = NULL;

    self = gf2d_physics_entity_new(NULL);
    stick_trap_init(self);

    return self;
}

void stick_trap_init(PhysicsEntity *self)
{
    GameObject *gobj = NULL;
    if(!self) return;

    self->entity->anim->rend->sprite = config.sprite;
    self->modelBox = config.shape;
    self->canCollide = config.canCollide;
    self->useGravity = config.useGravity;

    gobj = game_object_new();
    if(gobj)
    {
        gobj->selfPhys = self;
        gobj->self = self->entity;
        self->entity->abstraction = gobj;
    }
}
