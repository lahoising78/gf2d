#include "simple_logger.h"
#include "combat.h"
#include "player.h"

uint8_t combat_do_damage(GameObject *attacker, GameObject *target, float damage, float hitstun)
{
    ProgressBar *p_hp_ui = NULL;
    // char p_hp_txt[32];
    if(!target) return 0;
    if(target->hitstun > 0.0f || target->isProtected) return 0;

    target->health -= damage;
    slog("new health %.2f", target->health);
    if(target->selfPhys == player_get_phys())
    {
        p_hp_ui = player_hp_ui();
        if(p_hp_ui)
        {
            gf2d_progress_bar_set_value(p_hp_ui, target->health);
        }
    }

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