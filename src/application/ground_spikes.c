#include "simple_logger.h"
#include "game_object.h"
#include "ground_spikes.h"

typedef struct
{
    Sprite          *sprite;
    uint8_t         canCollide;
    uint8_t         useGravity;
    CollisionShape  shape;

    /* dmg */
    float dmgCooldown;

    PhysicsEntity *player;
} GroundSpikesConfig;

static GroundSpikesConfig config = {0};

void ground_spikes_update(Entity *self);
void ground_spikes_touch(Entity *self, Entity *other);

void ground_spikes_config(const char *filepath)
{
    SJson *json = NULL;
    PhysicsEntity *phys = NULL;
    SJson *obj = NULL;

    json = sj_load(filepath);
    if(!json)
    {
        slog("unable to find file %s", filepath);
        return;
    }

    phys = gf2d_physics_entity_load( sj_object_get_value(json, "phys") );
    if(!phys)
    {
        slog("couldn't get an available phys entity to load ground spikes config");
        return;
    }

    config.sprite = phys->entity->anim->rend->sprite;
    config.canCollide = phys->canCollide;
    config.useGravity = phys->useGravity;
    config.shape = phys->modelBox;
    slog("%u %u %u %.2f", config.sprite, config.canCollide, config.useGravity, config.shape.position.x);
    
    obj = sj_object_get_value(json, "damage");
    if(obj)
    {
        sj_get_float_value( sj_object_get_value(obj, "cooldown"), &config.dmgCooldown );
    }

    config.player = gf2d_physics_entity_get_by_name("punti");

    sj_free(json);
}

PhysicsEntity *ground_spikes_new()
{
    PhysicsEntity *phys = NULL;

    phys = gf2d_physics_entity_new(NULL);
    ground_spikes_init(phys);

    return phys;
}

void ground_spikes_init(PhysicsEntity *self)
{
    GameObject *gobj = NULL;

    if(!self) return;
    slog("init ground spike");

    self->entity->anim->rend->sprite = config.sprite;
    self->canCollide = config.canCollide;
    self->modelBox = config.shape;
    self->useGravity = config.useGravity;

    gobj = game_object_new();
    if(gobj)
    {
        gobj->selfPhys = self;
        gobj->self = self->entity;
        self->entity->abstraction = gobj;
    }

    self->entity->update = ground_spikes_update;
    self->entity->touch = ground_spikes_touch;
}

extern float frameTime;
void ground_spikes_update(Entity *self)
{
    GameObject *gobj = NULL;
    if(!self) return;

    gobj = (GameObject*)self->abstraction;
    if(!gobj) return;

    gobj->coolDown -= frameTime;
    if(gobj->coolDown < 0.0f) gobj->coolDown = 0.0f;
}

void ground_spikes_touch(Entity *self, Entity *other)
{
    GameObject *gobj = NULL;
    if(!config.player) return;
    if(!self || !other) return;
    if(other != config.player->entity) return;

    gobj = (GameObject*)self->abstraction;
    if(!gobj || gobj->coolDown > 0.0f) return;

    slog("spike do dmg");
    gobj->coolDown = config.dmgCooldown;
}