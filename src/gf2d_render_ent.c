#include "gf2d_render_ent.h"
#include "gf2d_camera.h"

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

void gf2d_render_ent_draw(RenderEntity *ent)
{
    if(!ent) return;

    ent->sec += frameTime;
    if( ent->sec >= SECS_PER_FRAME )
    {
        ent->frame++;
        ent->sec = 0.0f;
    }

    if( ent->frame >= ent->sprite->frames_per_line )
    {
        ent->frame = 0;
    }

    gf2d_sprite_draw(
        ent->sprite,
        gf2d_camera_get_displaced_position(ent->position),
        ent->scale,
        ent->scaleCenter,
        ent->rotation,
        ent->flip,
        ent->colorShift, 
        ent->frame
    );
}