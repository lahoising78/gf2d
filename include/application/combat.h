#ifndef _COMBAT_H_
#define _COMBAT_H_

#include "game_object.h"

uint8_t combat_do_damage(GameObject *attacker, GameObject *target, float damage, float hitstun);
void combat_knockback(GameObject *attacker, GameObject *target, Vector2D dir, float knockback);

#endif