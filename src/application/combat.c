#include "simple_logger.h"
#include "combat.h"

void combat_do_damage(GameObject *attacker, GameObject *target, float damage, float hitstun)
{
    if(!target) return;
    if(target->hitstun > 0.0f) return;

    target->health -= damage;
    slog("new health %.2f", target->health);

    target->hitstun = hitstun;
}

void combat_knockback(GameObject *attacker, GameObject *target, Vector2D dir, float knockback)
{
    if(!target) return;
    if(!target->self) return;
    if(target->hitstun > 0.0f) return;

    vector2d_set_magnitude(&dir, knockback);
    vector2d_copy(target->self->velocity, dir);
}