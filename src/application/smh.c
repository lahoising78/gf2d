#include "smh.h"
#include "simple_logger.h"

#include "gf2d_scene.h"
#include "gf2d_physics_entity.h"

extern float frameTime;
void do_the_thing( Entity *self )
{
    self->position.y = 350.0f + 100.0f * cosf( 0.025f * self->position.x );
}

void smh_awake()
{
    Animation *anim = NULL;
    // Entity *ent = NULL;

    // ent = gf2d_entity_new();
    // ent->render_ent->sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    // // ent->update = do_the_thing;
    // gf2d_scene_add_entity(ent);

    anim = gf2d_animation_new();
    anim->rend->sprite = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16);
    gf2d_animation_play(anim, 0, 16);
    gf2d_scene_add_animation(anim);
}