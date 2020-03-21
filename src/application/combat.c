#include "simple_logger.h"
#include "combat.h"

uint8_t combat_do_damage(GameObject *attacker, GameObject *target, float damage, float hitstun)
{
    if(!target) return 0;
    if(target->hitstun > 0.0f || target->isProtected) return 0;

    target->health -= damage;
    slog("new health %.2f", target->health);

    target->hitstun = hitstun;

    return 1;
}

void combat_knockback(GameObject *attacker, GameObject *target, Vector2D dir, float knockback)
{
    if(!target) return;
    if(!target->self) return;
    if(target->hitstun > 0.0f || target->isProtected) return;

    vector2d_set_magnitude(&dir, knockback);
    vector2d_copy(target->self->velocity, dir);
}