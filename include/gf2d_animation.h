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

/** 
 * @brief initialize the animation manager
 * @param count how many animations to initialize
 * @note this calls gf2d_animation_manager_init_all_animations
 */
void gf2d_animation_manager_init( uint32_t count );

/** 
 * @brief initialize each animation in the animation manager
 * @note this is where each animation gets assigned a render entity
 */
void gf2d_animation_manager_init_all_animations();

/** 
 * @brief reset all data for all animations
 * @param del if 1 delete render ent for all animations. if 0, just clean up without getting rid of render ent
 */
void gf2d_animation_manager_clean( uint8_t del );

/** 
 * @brief set initial values
 * @note does not reset all values. it assumes that everything is 0
 * @param anim the animation to be init
 */
void gf2d_animation_init(Animation *anim);

/** 
 * @brief get new animation
 * @return a new animation in animation manager
 */
Animation *gf2d_animation_new();

/** 
 * @brief load an animation from a json object
 * @note the animation will be paused by default and set to use only one frame.
 * To override this, need to include a "playing" object in the json file
 * that contains the animation row and max number of frame on that animation
 * @param json the json object to read from
 * @return the resulting animation
 */
Animation *gf2d_animation_load( SJson *json );

/** 
 * @brief render the animation
 * @param anim the animation to be rendered
 */
void gf2d_animation_render( Animation *anim );

/** 
 * @brief free the animation
 * @param anim the animation to be free
 * @param del if 1, delete render ent. 0 otherwise
 */
void gf2d_animation_free(Animation *anim, uint8_t del);

/** 
 * @brief play the animation
 * @param anim the animation to play
 * @param animation which animation to play from the sprite sheet, 0 being the top most
 * @param maxFrame how many frame to play from the animation
 */
void gf2d_animation_play( Animation *anim, uint32_t animation, uint32_t maxFrame );

/** 
 * @brief pause the animation
 * @param anim the animation to play
 */
void gf2d_animation_pause( Animation *anim );


#endif