#include "simple_logger.h"
#include "game_object.h"
#include "sentry.h"

extern float frameTime;

float coolDown = 0.0f;

void sentry_update(Entity *self);
void sentry_touch(Entity *self, Entity *other);

typedef struct 
{
    Sprite *sprite;
    CollisionShape shape;
    PhysicsEntityType physType;
    uint8_t canCollide;
} SentryJson;

static SentryJson config = {0};

void sentry_load_config(const char *filename)
{
    SJson *json = NULL;
    PhysicsEntity *phys = NULL;
    if(!filename) return;

    json = sj_load(filename);
    phys = gf2d_physics_entity_load(json);
    sj_free(json);

    if(!phys) return;
    config.canCollide = phys->canCollide;
    config.physType = phys->type;
    config.shape = phys->modelBox;
    config.sprite = phys->entity->anim->rend->sprite;

    gf2d_physics_entity_free(phys);
}

PhysicsEntity *sentry_new()
{
    PhysicsEntity *sentry = NULL;

    sentry = gf2d_physics_entity_new(NULL);
    sentry_init(sentry);

    return sentry;
}

void sentry_init(PhysicsEntity *phys)
{
    GameObject *obj = NULL;
    if(!phys || !phys->entity) return;

    phys->entity->update = sentry_update;
    phys->entity->touch = sentry_touch;
    obj = game_object_new();
    if(!obj)
    {
        slog("unable to allocate game obj for sentry");
        gf2d_physics_entity_free(phys);
        return;
    }

    obj->coolDown = gfc_random() * 5.0f;
    phys->entity->abstraction = obj;
    slog("cool down %.2f", obj->coolDown);

    phys->canCollide = config.canCollide;
    phys->modelBox = config.shape;
    phys->entity->anim->rend->sprite = config.sprite;
    phys->type = config.physType;
}

void sentry_update(Entity *self)
{
    GameObject *obj = NULL;
    if(!self || !self->abstraction) return;

    obj = (GameObject*)self->abstraction;
}

void sentry_touch(Entity *self, Entity *other)
{
    if(!self || !other) return;
}
