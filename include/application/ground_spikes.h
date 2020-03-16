#ifndef _GROUND_SPIKES_H_
#define _GROUND_SPIKES_H_

#include "gf2d_physics_entity.h"

void ground_spikes_config(const char *filepath);
PhysicsEntity *ground_spikes_new();
void ground_spikes_init(PhysicsEntity *self);

#endif