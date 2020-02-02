#ifndef _GF2D_ANIMATION_H_
#define _GF2D_ANIMATION_H_

#include "gf2d_render_ent.h"

typedef struct
{
    RenderEntity    *rend;
    uint8_t         playing;
    uint32_t        animation;
    uint32_t        maxFrame;
    float           playbackSpeed;
    uint8_t         _inuse;
} Animation;

void gf2d_animation_manager_init( uint32_t count );
void gf2d_animation_manager_init_all_animations();
void gf2d_animation_manager_clean( uint8_t del );
void gf2d_animation_manager_wipe();

void gf2d_animation_init(Animation *anim);
Animation *gf2d_animation_new();
void gf2d_animation_render( Animation *anim );
void gf2d_animation_free(Animation *anim, uint8_t del);

void gf2d_animation_play( Animation *anim, uint32_t animation, uint32_t maxFrame );

void gf2d_animation_pause( Animation *anim );


#endif