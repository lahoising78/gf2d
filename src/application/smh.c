#include "smh.h"
#include "simple_logger.h"

#include "gf2d_input.h"
#include "gf2d_scene.h"
#include "gf2d_physics_entity.h"
#include "gf2d_tilemap.h"

void somethink(Entity *self)
{
    
}

void smh_awake()
{
    PhysicsEntity *p = NULL;
    PhysicsEntity *floor = NULL;

    p = gf2d_physics_entity_new();
    // slog("--== %.2f %.2f ==--", p->entity->anim->rend->position.x, p->entity->anim->rend->position.y);
    p->entity->anim->rend->sprite = gf2d_sprite_load_all("images/space_bug.png", 128, 128, 16);
    p->entity->position.x = 100;
    p->entity->position.y = 10;
    p->entity->update = somethink;
    p->useGravity = 1;
    p->type = PET_KINETIC;
    p->canCollide = 1;
    gf2d_animation_play(p->entity->anim, 7, 16);
    if( gf2d_scene_add_to_drawables(p, DET_PHYS) < 0 )
        slog("no hay espacion para phys");

    floor = gf2d_physics_entity_new();
    floor->entity->anim->rend->sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    floor->entity->position = vector2d(0.0f, 500.0f);
    floor->canCollide = 1;
    gf2d_animation_pause(floor->entity->anim);
    gf2d_scene_add_to_drawables( floor, DET_PHYS );
}