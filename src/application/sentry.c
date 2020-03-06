#include "simple_logger.h"
#include "game_object.h"
#include "sentry.h"

extern float frameTime;

float coolDown = 0.0f;

void sentry_update(Entity *self);
void sentry_touch(Entity *self, Entity *other);

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
