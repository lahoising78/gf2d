#include "simple_logger.h"
#include "ground_spikes.h"

typedef struct
{
    Sprite          *sprite;
    uint8_t         canCollide;
    uint8_t         useGravity;
    CollisionShape  shape;
} GroundSpikesConfig;

static GroundSpikesConfig config = {0};

void ground_spikes_config(const char *filepath)
{
    SJson *json = NULL;
    PhysicsEntity *phys = NULL;

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
    if(!self) return;
    slog("init ground spike");

    self->entity->anim->rend->sprite = config.sprite;
    self->canCollide = config.canCollide;
    self->modelBox = config.shape;
    self->useGravity = config.useGravity;
}
