#include "gf2d_render_ent.h"
#include "gf2d_camera.h"
#include "simple_logger.h"

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
    if(!ent) return;
    if(!ent->sprite) return;
    // slog("render ent draw");
    gf2d_sprite_draw(
        ent->sprite,
        gf2d_camera_get_displaced_position(ent->position),
        ent->scale,
        ent->scaleCenter,
        ent->rotation,
        ent->flip,
        ent->colorShift, 
        (uint32_t)ent->frame
    );
}