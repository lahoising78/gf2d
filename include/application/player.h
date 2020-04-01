#ifndef _APP_PLAYER_H_
#define _APP_PLAYER_H_

#include "gf2d_physics_entity.h"
#include "gf2d_ui.h"

void player_create(PhysicsEntity *self);
ProgressBar *player_hp_ui();
PhysicsEntity *player_get_phys();

#endif