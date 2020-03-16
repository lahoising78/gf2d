#ifndef _LAND_MINE_H_
#define _LAND_MINE_H_

#include "gf2d_physics_entity.h"

void land_mine_config(const char *filepath);
PhysicsEntity *land_mine_new();
void land_mine_init(PhysicsEntity *phys);

#endif