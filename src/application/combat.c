#include "simple_logger.h"
#include "combat.h"

void combat_do_damage(GameObject *attacker, GameObject *target, float damage)
{
    if(!target) return;

    target->health -= damage;
    slog("new health %.2f", target->health);
}

void combat_knockback(GameObject *attacker, GameObject *target, Vector2D dir, float knockback)
{
    if(!target) return;
    if(!target->self) return;

    vector2d_set_magnitude(&dir, knockback);
    vector2d_copy(target->self->velocity, dir);
}