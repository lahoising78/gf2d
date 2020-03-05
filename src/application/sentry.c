#include "simple_logger.h"
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
    if(!phys || !phys->entity) return;
    coolDown = gfc_random() * 5.0f;
    slog("cool down to %.2f", coolDown);

    phys->entity->update = sentry_update;
    phys->entity->touch = sentry_touch;
}

void sentry_update(Entity *self)
{
    if(!self) return;

    // if(self->name)
        // slog("%s has cool down of %.2f", self->name, coolDown);
}

void sentry_touch(Entity *self, Entity *other)
{
    if(!self || !other) return;
}
