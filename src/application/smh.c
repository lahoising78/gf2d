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
    Entity *ent = NULL;
    PhysicsEntity *pe = NULL;

    ent = gf2d_entity_new();
    ent->render_ent->sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    gf2d_scene_add_entity(ent);

    pe = gf2d_physics_entity_new();
    pe->entity->render_ent->sprite = gf2d_sprite_load_all("images/space_bug.png", 128, 128, 16);
    pe->entity->position = vector2d(0.0f, 350.0f);
    pe->entity->velocity.x = 3.0f;
    pe->type = PET_KINETIC;
    pe->entity->update = do_the_thing;
    if(gf2d_scene_add_entity(pe->entity) < 0)
        slog("no le pudimo da na");
}