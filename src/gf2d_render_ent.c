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

    return ent;
}

void gf2d_render_ent_free( RenderEntity *ent )
{
    if(!ent) return;

    gf2d_sprite_free(ent->sprite);
    if(ent->scale) free(ent->scale);
    if(ent->scaleCenter) free(ent->scaleCenter);
    if(ent->rotation) free(ent->rotation);
    if(ent->flip) free(ent->flip);
    if(ent->colorShift) free(ent->colorShift);

    memset(ent, 0, sizeof(RenderEntity));
}

void gf2d_render_ent_draw(RenderEntity *ent)
{
    // CollisionShape e = {0}, a = {0};

    if(!ent) return;
    if(!ent->sprite) return;

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
        ent->scale,
        ent->scaleCenter,
        ent->rotation,
        ent->flip,
        ent->colorShift, 
        (uint32_t)ent->frame
    );
}