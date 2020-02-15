#include "smh.h"
#include "simple_logger.h"

#include "gf2d_input.h"
#include "gf2d_scene.h"
#include "gf2d_physics_entity.h"
#include "gf2d_tilemap.h"
#include "gf2d_camera.h"

void somethink(Entity *self)
{
    Vector2D camera = {0};

    if(!self) return;
    if( gf2d_input_is_key_pressed(SDL_SCANCODE_RIGHT) )
        self->velocity.x = 3.0f;
    else if( gf2d_input_is_key_pressed(SDL_SCANCODE_LEFT) )
        self->velocity.x = -3.0f;
    else if( gf2d_input_key_released(SDL_SCANCODE_RIGHT) || gf2d_input_key_released(SDL_SCANCODE_LEFT) )
        self->velocity.x = 0.0f;
    
    if( gf2d_input_is_key_pressed(SDL_SCANCODE_DOWN) )
        self->velocity.y = 3.0f;
    else if( gf2d_input_is_key_pressed(SDL_SCANCODE_UP) )
        self->velocity.y = -3.0f;
    else if( gf2d_input_key_released(SDL_SCANCODE_DOWN) || gf2d_input_key_released(SDL_SCANCODE_UP) )
        self->velocity.y = 0.0f;

    vector2d_set(camera, ((float)self->anim->rend->sprite->frame_w) * 0.5f, (float)(self->anim->rend->sprite->frame_h) * 0.5f);
    vector2d_add(camera, self->position, camera);
    gf2d_camera_set_position( camera );
}

void touching(Entity *self, Entity *other)
{
    // slog("touching");
}

void smh_awake()
{
    PhysicsEntity *p = NULL;
    Tilemap *tilemap = NULL;
    uint32_t map[] =
    {
        0, 1, 0, 2, 0, 
        3, 4, 5, 6, 7,
        8, 9, 0, 9, 10, 
        0, 0, 0, 0, 0,
        1, 2, 3, 4, 5
    };
    CollisionShape solid[] = {
        gf2d_collision_shape( vector2d(0.0f, 0.0f), vector2d(0.0f, 0.0f), CST_BOX ),
        gf2d_collision_shape( vector2d(0.0f, 0.0f), vector2d(0.0f, 0.0f), CST_BOX ),
        gf2d_collision_shape( vector2d(0.0f, 0.0f), vector2d(128.0f, 128.0f), CST_BOX ),
        gf2d_collision_shape( vector2d(0.0f, 0.0f), vector2d(128.0f, 128.0f), CST_BOX ),
        gf2d_collision_shape( vector2d(0.0f, 0.0f), vector2d(128.0f, 128.0f), CST_BOX ),
        gf2d_collision_shape( vector2d(0.0f, 0.0f), vector2d(128.0f, 128.0f), CST_BOX ),
        gf2d_collision_shape( vector2d(0.0f, 0.0f), vector2d(128.0f, 128.0f), CST_BOX ),
        gf2d_collision_shape( vector2d(0.0f, 0.0f), vector2d(128.0f, 128.0f), CST_BOX ),
        gf2d_collision_shape( vector2d(0.0f, 0.0f), vector2d(128.0f, 128.0f), CST_BOX ),
        gf2d_collision_shape( vector2d(0.0f, 0.0f), vector2d(128.0f, 128.0f), CST_BOX )
    };

    p = gf2d_physics_entity_new();
    // slog("--== %.2f %.2f ==--", p->entity->anim->rend->position.x, p->entity->anim->rend->position.y);
    p->entity->anim->rend->sprite = gf2d_sprite_load_all("images/space_bug.png", 128, 128, 16);
    p->entity->position.x = 0;
    p->entity->position.y = 0;
    p->entity->update = somethink;
    p->entity->touch = touching;
    p->useGravity = 1;
    p->type = PET_KINETIC;
    p->canCollide = 1;

    p->modelBox = gf2d_collision_shape(vector2d(14.0f, 14.0f), vector2d(100.0f, 100.0f), CST_BOX);
    
    gf2d_animation_play(p->entity->anim, 0, 16);
    if( gf2d_scene_add_to_drawables(p, DET_PHYS) < 0 )
        slog("no hay espacion para phys");

    tilemap = gf2d_tilemap_load( gf2d_sprite_load_all("images/ed210.png", 128, 128, 16), map, solid, 5, 5 );
    gf2d_scene_add_to_drawables(tilemap, DET_TMAP);
}