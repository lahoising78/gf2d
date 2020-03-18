#ifndef _COMBAT_H_
#define _COMBAT_H_

#include "game_object.h"

void combat_do_damage(GameObject *attacker, GameObject *target, float damage);
void combat_knockback(GameObject *attacker, GameObject *target, Vector2D dir, float knockback);

#endif