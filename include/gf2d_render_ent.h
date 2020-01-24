#ifndef _GF2D_RENDER_ENT_H_
#define _GF2D_RENDER_ENT_H_

#include "gf2d_sprite.h"

typedef struct
{
    Sprite              *sprite;
    Vector2D            position;
    Vector2D            *scale;
    Vector2D            *scaleCenter;
    Vector3D            *rotation;
    Vector2D            *flip;
    Vector4D            *colorShift;
    Uint32              frame;
    float               sec;
} RenderEntity;

RenderEntity *gf2d_render_ent_new( Sprite *sprite );

void gf2d_render_ent_draw(RenderEntity *ent);

#endif