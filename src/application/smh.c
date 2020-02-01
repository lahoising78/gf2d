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
    Entity *ent = NULL;
    Entity *e = NULL;

    ent = gf2d_entity_new();
    ent->anim->rend->sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    // ent->update = do_the_thing;
    gf2d_scene_add_entity(ent);

    e = gf2d_entity_new();
    e->anim->rend->sprite = gf2d_sprite_load_all("images/space_bug.png", 128, 128, 16);
    e->position = vector2d(300.0f, 300.0f);
    gf2d_animation_play(e->anim, 1, 16);
    gf2d_scene_add_entity(e);

    anim = gf2d_animation_new();
    anim->rend->sprite = gf2d_sprite_load_all("images/pointer.png", 32, 32, 16);
    gf2d_animation_play(anim, 0, 16);
    gf2d_scene_add_animation(anim);
}