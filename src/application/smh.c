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
    SJson *json = NULL;
    RenderEntity *rend = NULL;
    Animation *anim = NULL;
    Entity *ent = NULL;
    PhysicsEntity *phys = NULL;

    json = sj_load("application/drawables.json");
    
    rend = gf2d_render_ent_load( sj_object_get_value(json, "renderEnt") );
    gf2d_scene_add_to_drawables(rend, DET_REND);

    anim = gf2d_animation_load( sj_object_get_value(json, "animation") );
    gf2d_scene_add_to_drawables(anim, DET_ANIM);

    ent = gf2d_entity_load( sj_object_get_value(json, "ent") );
    gf2d_scene_add_to_drawables(ent, DET_ENT);

    phys = gf2d_physics_entity_load( sj_object_get_value(json, "phys") );
    phys->entity->update = somethink;
    gf2d_scene_add_to_drawables(phys, DET_PHYS);

    sj_free(json);
}