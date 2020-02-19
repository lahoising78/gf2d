#include "gf2d_render_ent.h"
#include "gf2d_camera.h"
#include "simple_logger.h"
#include "gf2d_collision.h"

#define SECS_PER_FRAME 0.1f/32.0f

extern float frameTime;

RenderEntity *gf2d_render_ent_new( Sprite *sprite )
{
    RenderEntity *ent = NULL;
    size_t s = sizeof(RenderEntity);
    
    ent = (RenderEntity*)malloc(s);
    memset(ent, 0, s);
    ent->sprite = sprite;
    vector2d_set(ent->scale, 1.0f, 1.0f);
    vector4d_set(ent->colorShift, 255.0f, 255.0f, 255.0f, 255.0f);

    return ent;
}

RenderEntity *gf2d_render_ent_load( SJson *json )
{
    SJson *obj = NULL;
    RenderEntity *rend = NULL;
    if(!json) return NULL;

    if( sj_is_string(json) )
    {
        obj = sj_load( sj_get_string_value(json) );
            rend = gf2d_render_ent_load(obj);
        sj_free(obj);
        return rend;
    }

    rend = gf2d_render_ent_new(NULL);
    if(!rend) return NULL;
    
    rend->sprite = gf2d_json_sprite( sj_object_get_value(json, "sprite") );
    rend->position = gf2d_json_vector2d( sj_object_get_value(json, "position") );
    sj_get_float_value( sj_object_get_value(json, "frame"), &rend->frame );

    return rend;
}

void gf2d_render_ent_free( RenderEntity *ent )
{
    if(!ent) return;

    gf2d_sprite_free(ent->sprite);
    memset(ent, 0, sizeof(RenderEntity));
    vector2d_set(ent->scale, 1.0f, 1.0f);
    vector4d_set(ent->colorShift, 255.0f, 255.0f, 255.0f, 255.0f);
}

void gf2d_render_ent_draw(RenderEntity *ent)
{
    // CollisionShape e = {0}, a = {0};

    if(!ent) 
    {
        slog("Cannot render a null render ent");
        return;
    }
    if(!ent->sprite) 
    {
        slog("Cannot render a null sprite");
        return;
    }

    // e.shapeType = CST_BOX;
    // e.position = gf2d_camera_get_displaced_position(ent->position);
    // e.dimensions.wh.x = ent->sprite->frame_w;
    // e.dimensions.wh.y = ent->sprite->frame_h;

    // a = gf2d_camera_collision_shape();
    // if(gf2d_collision_check(&e, &a)) return;
    
    gf2d_sprite_draw(
        ent->sprite,
        gf2d_camera_get_displaced_position(ent->position),
        // e.position,
        &ent->scale,
        &ent->scaleCenter,
        &ent->rotation,
        &ent->flip,
        &ent->colorShift, 
        (uint32_t)ent->frame
    );
}