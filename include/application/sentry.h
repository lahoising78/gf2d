#ifndef _SENTRY_H_
#define _SENTRY_H_

#include "gf2d_physics_entity.h"

void sentry_load_config(const char *filename);
PhysicsEntity *sentry_new();
void sentry_init(PhysicsEntity *ent);

#endif