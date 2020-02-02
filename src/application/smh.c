#include "smh.h"
#include "simple_logger.h"

#include "gf2d_scene.h"
#include "gf2d_physics_entity.h"

void smh_awake()
{
    int i;
    Entity *e = NULL;
    Animation *a = NULL;
    Animation *r = NULL;
    PhysicsEntity *p = NULL;

    for(i = 0; i < 4; i++)
    {
        // e = gf2d_entity_new();
        // e->anim->rend->sprite = gf2d_sprite_load_all("images/space_bug.png", 128, 128, 16);
        // e->position.x = i*1200/8;
        // gf2d_animation_play(e->anim, 0, 16);
        // if( gf2d_scene_add_to_drawables(e, DET_ENT) < 0 )
        //     slog("no hay espacio suficiente");
        
        // a = gf2d_animation_new();
        // slog("--== %.2f %.2f ==--", a->rend->position.x, a->rend->position.y);
        // a->rend->sprite = e->anim->rend->sprite;
        // a->rend->position.x = (i+1)*1200/8;
        // a->rend->position.y = 700.0f/4.0f;
        // gf2d_animation_play(a, 1, 16);
        // if( gf2d_scene_add_to_drawables(a, DET_ANIM) < 0 )
        //     slog("no hay espacion para animacion");

        // r = gf2d_animation_new();
        // // slog("--== %.2f %.2f ==--", r->rend->position.x, r->rend->position.y);
        // r->rend->sprite = a->rend->sprite;
        // r->rend->position.x = (i+2)*1200/8;
        // r->rend->position.y = 2.0f*700.0f/4.0f;
        // if( gf2d_scene_add_to_drawables(r->rend, DET_REND) < 0 )
        //     slog("no hay espacion para rend ent");

        p = gf2d_physics_entity_new();
        // slog("--== %.2f %.2f ==--", p->entity->anim->rend->position.x, p->entity->anim->rend->position.y);
        p->entity->anim->rend->sprite = gf2d_sprite_load_all("images/space_bug.png", 128, 128, 16);
        p->entity->position.x = (i)*1200/8;
        p->entity->position.y = 3.0f*700.0f/4.0f;
        gf2d_animation_play(p->entity->anim, i, 16);
        if( gf2d_scene_add_to_drawables(p, DET_PHYS) < 0 )
            slog("no hay espacion para phys");

        // if(i%2 == 0) 
        // {
        //     gf2d_scene_remove_from_drawables(e);
        //     gf2d_scene_remove_from_drawables(a);
        //     gf2d_scene_remove_from_drawables(r);
        //     gf2d_scene_remove_from_drawables(p);
        // }
    }
}