#include "simple_logger.h"
#include "game_object.h"
#include "gf2d_scene.h"
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

    /* other */
    PhysicsEntity               *player;
    float                       gravity;
    float                       yOffset;
} StickTrapConfig;

static StickTrapConfig config = {0};

void stick_trap_touch(Entity *self, Entity *other);

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

    config.player = gf2d_physics_entity_get_by_name("punti");
    sj_get_float_value( sj_object_get_value(json, "cooldown"), &config.cooldown );
    sj_get_float_value( sj_object_get_value(json, "gravity"), &config.gravity );

    if(config.player)
        config.yOffset = config.player->modelBox.dimensions.wh.y - config.shape.dimensions.wh.y;

    sj_free(json);
}

PhysicsEntity *stick_trap_new()
{
    PhysicsEntity *self = NULL;

    self = gf2d_physics_entity_new(NULL);
    stick_trap_init(self);
    gf2d_scene_add_to_drawables(self, DET_PHYS);

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
        gobj->coolDown = config.cooldown;
        self->entity->abstraction = gobj;
    }

    self->entity->touch = stick_trap_touch;
}

extern float frameTime;
void stick_trap_touch(Entity *self, Entity *other)
{
    GameObject *gobj = NULL;
    if(!config.player) return;
    if(!self || !other) return;
    if(other != config.player->entity) return;

    gobj = (GameObject*)self->abstraction;
    if(!gobj) return;

    if(gobj->coolDown <= 0.0f)
    {
        game_object_free(gobj);
        return;
    }
    gobj->coolDown -= frameTime;

    other->velocity.x = other->velocity.y = 0.0f;
    other->position.x += (self->position.x - other->position.x) * frameTime * config.gravity;
    other->position.y += (self->position.y - config.yOffset - other->position.y) * frameTime * config.gravity;
}