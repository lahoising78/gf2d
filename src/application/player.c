#include "simple_logger.h"
#include "gf2d_camera.h"
#include "gf2d_input.h"
#include "gf2d_scene.h"
#include "punti_jordan.h"
#include "player.h"

#define VELOCITY_CONST 3.0f

/* Animation defines */
#define ANIM_IDLE 0
#define ANIM_IDLE_MAX 2
#define ANIM_WALKING 2
#define ANIM_WALKING_MAX 4

typedef enum
{
    PS_IDLE =                   0,
    PS_WALKING =                1,
    PS_JUMPING =                2,
    PS_ATTACK1 =                3,
    PS_ATTACK2 =                4
} PlayerState;

typedef enum
{
    DIR_NONE =                  0x00,
    DIR_UP =                    0x01,
    DIR_RIGHT =                 0x02,
    DIR_DOWN =                  0x04,
    DIR_LEFT =                  0x08
} WalkDirection;

void player_update(Entity *self);
void player_think(Entity *self);

void player_walking();
void player_jumping(Entity *self);
void player_attacking();

float walkDir = 0.0f;
uint8_t left, right;
PlayerState currentState = PS_IDLE;
PhysicsEntity *phys = NULL;
uint8_t attacking = 0;
uint8_t canCombo = 0;

void player_create(PhysicsEntity *self)
{
    if(!self || !self->entity) return;
    
    self->entity->think = player_think;
    self->entity->update = player_update;
    phys = self;
}

void player_think(Entity *self)
{
    uint32_t *anim = NULL;
    float animSpeed = 0.0f;
    uint32_t frame = 0;
    if(!self) return;

    player_walking();
    player_attacking();
    if(phys && !phys->_onFloor)
    {
        currentState = PS_JUMPING;
    }
    else if( attacking )
    {
        currentState = PS_ATTACK1 + attacking - 1;
    }
    else if(walkDir != 0.0f)
    {
        currentState = PS_WALKING;
    }
    else
    {
        currentState = PS_IDLE;
    }

    switch (currentState)
    {
    case PS_WALKING:
        if( self->anim->animation != ANIM_WALKING )
        {
            anim = pj_anim_walking();
            animSpeed = pj_anim_walking_speed();
            gf2d_animation_play(self->anim, anim[0], anim[1]);
            self->anim->playbackSpeed = animSpeed;
        }
        break;

    case PS_JUMPING:
        anim = pj_anim_jumping();
        animSpeed = pj_anim_jumping_speed();
        if( self->anim->animation != anim[0] )
        {
            gf2d_animation_play(self->anim, anim[0], anim[1]);
            gf2d_animation_set_frame(self->anim, anim[1]-1);
            self->anim->playbackSpeed = animSpeed;
        }
        else if ( self->velocity.y < 0.0f && (int)self->anim->rend->frame%anim[1] == anim[1]-1 )
        {
            gf2d_animation_pause(self->anim);
        }
        else
        {
            gf2d_animation_set_frame(self->anim, anim[1]);
        }
        break;

    case PS_ATTACK1:
        anim = pj_anim_slash_down();
        animSpeed = pj_anim_slash_down_speed();
        frame = gf2d_animation_get_frame(self->anim);
        if( self->anim->animation != anim[0] )
        {
            gf2d_animation_play(self->anim, anim[0], anim[1]);
            self->anim->playbackSpeed = animSpeed;
            canCombo = 0;
        }
        else if ( frame == anim[1]-1 )
        {
            attacking = 0;
            canCombo = 0;
        }
        else if ( frame >= anim[1]-4 )
        {
            canCombo = 1;
        }
        break;

    case PS_ATTACK2:
        anim = pj_anim_slash_up();
        animSpeed = pj_anim_slash_up_speed();
        canCombo = 0;
        if( self->anim->animation != anim[0] )
        {
            gf2d_animation_play(self->anim, anim[0], anim[1]);
            self->anim->playbackSpeed = animSpeed;
        }
        else if ( gf2d_animation_get_frame(self->anim) == anim[1]-1 )
        {
            attacking = 0;
        }
        break;
    
    default:
        if( self->anim->animation != ANIM_IDLE )
        {
            anim = pj_anim_idle();
            animSpeed = pj_anim_idle_speed();
            gf2d_animation_play(self->anim, anim[0], anim[1]);
            self->anim->playbackSpeed = animSpeed;
        }
        break;
    }
}

void player_update(Entity *self)
{
    Vector2D cam = {0};
    if(!self) return;

    /* update walking */
    self->velocity.x = walkDir * VELOCITY_CONST;

    /* jump */
    if(phys->_onFloor && gf2d_input_key_just_pressed(SDL_SCANCODE_UP) ) 
        player_jumping(self);

    /* make camera follow player */
    cam.x = self->anim->rend->sprite->frame_w * self->anim->rend->scale.x;
    cam.y = self->anim->rend->sprite->frame_h * self->anim->rend->scale.y;
    vector2d_scale(cam, cam, 0.5);
    vector2d_add(cam, cam, self->position);
    gf2d_camera_set_position(cam);

    if( gf2d_input_key_released(SDL_SCANCODE_R) )
        gf2d_scene_load_from_file("application/scenes/first_scene.json");
}

void player_walking()
{
    float dir = 0.0f;

    if(attacking) return;

    dir = gf2d_input_joystick_get_axis(0, 0);
    if( dir != 0.0f ) 
    {
        walkDir = dir;
        phys->entity->anim->rend->flip.x = (int)walkDir + 1;
        return;
    }

    if( gf2d_input_key_just_pressed(SDL_SCANCODE_RIGHT) )
    {
        right = 1;
        phys->entity->anim->rend->flip.x = 0;
    }
    else if ( gf2d_input_key_released(SDL_SCANCODE_RIGHT) )
        right = 0;
    
    if ( gf2d_input_key_just_pressed(SDL_SCANCODE_LEFT) )
    {
        left = 1;
        phys->entity->anim->rend->flip.x = 1;
    }
    else if ( gf2d_input_key_released(SDL_SCANCODE_LEFT) )
        left = 0;

    walkDir = (float)(right - left);
}

void player_jumping(Entity *self)
{
    if(!self) return;

    self->velocity.y = -15.0f;
}

void player_attacking()
{
    if( !gf2d_input_key_just_pressed(SDL_SCANCODE_Z) ) return;
    if( !phys->_onFloor ) return;
        
    if(!attacking || canCombo) attacking++;
    walkDir = 0.0f;
}