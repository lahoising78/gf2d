#ifndef _STICK_TRAP_H_
#define _STICK_TRAP_H_

#include "gf2d_physics_entity.h"

void stick_trap_config(const char *filepath);
PhysicsEntity *stick_trap_new();
void stick_trap_init(PhysicsEntity *self);

#endif