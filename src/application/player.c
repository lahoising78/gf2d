#include "simple_logger.h"
#include "gf2d_camera.h"
#include "gf2d_input.h"
#include "gf2d_scene.h"
#include "punti_jordan.h"
#include "gf2d_projectile.h"
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
    PS_ATTACKING =              3,
    PS_DASH =                   4,
    PS_SPECIAL_NEUTRAL =        5
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
uint8_t player_dash();
uint8_t player_special_neutral();

void player_special_neutral_perform(Entity *self);

float walkDir = 0.0f;
uint8_t left, right;
PlayerState currentState = PS_IDLE;
PhysicsEntity *phys = NULL;
uint8_t attacking = 0;
uint8_t canCombo = 0;
float dash = 0.0f;

void player_create(PhysicsEntity *self)
{
    if(!self || !self->entity) return;
    
    self->entity->think = player_think;
    self->entity->update = player_update;
    phys = self;
}

extern float frameTime;
void player_think(Entity *self)
{
    uint32_t *anim = NULL;
    float animSpeed = 0.0f;
    uint32_t frame = 0;
    if(!self) return;

    player_walking();
    player_attacking();
    if(player_dash())
    {
        if(dash < pj_dash()[0])
        {
            dash += frameTime;
            phys->useGravity = 0;
            self->velocity.y = 0.0f;
            currentState = PS_DASH;
        }
        else
        {
            dash = 0.0f;
            phys->useGravity = 1;
        }
    }
    else if(phys && !phys->_onFloor)
    {
        attacking = 0;
        currentState = PS_JUMPING;
    }
    else if( attacking )
    {
        currentState = PS_ATTACKING;
    }
    else if ( player_special_neutral() )
    {
        currentState = PS_SPECIAL_NEUTRAL;
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

    case PS_ATTACKING:
        /* first attack */
        if( self->anim->animation == pj_anim_slash_down()[0] )
        {
            anim = pj_anim_slash_down();
            animSpeed = pj_anim_slash_down_speed();
            frame = gf2d_animation_get_frame(self->anim);

            if ( frame >= anim[1] - 1 )
            {
                if(attacking <= 1)
                {
                    attacking = 0;
                }
                else
                {
                    /* start second attack if you asked for it */
                    anim = pj_anim_slash_up();
                    animSpeed = pj_anim_slash_up_speed();
                    gf2d_animation_play(self->anim, anim[0], anim[1]);
                    self->anim->playbackSpeed = animSpeed;
                    canCombo = 0;
                }
                canCombo = 0;
            }
            else if ( frame >= anim[1] - 5 )
            {
                canCombo = 1;
            }
        }
        /* second attack */
        else if( self->anim->animation == pj_anim_slash_up()[0] )
        {
            anim = pj_anim_slash_up();
            animSpeed = pj_anim_slash_up_speed();
            frame = gf2d_animation_get_frame(self->anim);
            if (frame == anim[1] - 1)
            {
                if( attacking <= 2 )
                {
                    attacking = 0;
                }
                else
                {
                    anim = pj_anim_slash_side();
                    animSpeed = pj_anim_slash_side_speed();
                    gf2d_animation_play(self->anim, anim[0], anim[1]);
                    self->anim->playbackSpeed = animSpeed;
                    canCombo = 0;
                }
                
                canCombo = 0;
            }
            else if ( frame >= anim[1] - 5 )
            {
                canCombo = 1;
            }
        }
        /* third attack */
        else if ( self->anim->animation == pj_anim_slash_side()[0] )
        {
            anim = pj_anim_slash_side();
            animSpeed = pj_anim_slash_side_speed();
            frame = gf2d_animation_get_frame(self->anim);
            if ( frame == anim[1] - 1 )
            {
                attacking = 0;
                canCombo = 0;
            }
        }
        /* in attack state but not playing any attacking animation, play the first anim */
        else
        {
            slog("start slash down");
            anim = pj_anim_slash_down();
            animSpeed = pj_anim_slash_down_speed();
            gf2d_animation_play(self->anim, anim[0], anim[1]);
            self->anim->playbackSpeed = animSpeed;
            canCombo = 0;
        }

        break;

    case PS_DASH:
        anim = pj_anim_walking();
        if( self->anim->animation != anim[0] )
        {
            gf2d_animation_play(self->anim, anim[0], anim[1]);
            self->anim->playbackSpeed = 0.0f;
        }
        break;

    case PS_SPECIAL_NEUTRAL:
        player_special_neutral_perform(self);
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
    if( dash > 0.0f ) self->velocity.x *= pj_dash()[1];

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

    // if(lastAttack >= 0.0f) return;
    if(attacking) return;

    dir = gf2d_input_joystick_get_axis(0, 0);
    if( dir != 0.0f && dash <= 0.0f ) 
    {
        walkDir = dir;
        if(walkDir != 0.0f) phys->entity->anim->rend->flip.x = walkDir < 0.0f;
        return;
    }

    if( gf2d_input_key_just_pressed(SDL_SCANCODE_RIGHT) )
    {
        right = 1;
    }
    else if ( gf2d_input_key_released(SDL_SCANCODE_RIGHT) )
    {
        right = 0;
    }
    
    if ( gf2d_input_key_just_pressed(SDL_SCANCODE_LEFT) )
    {
        left = 1;
    }
    else if ( gf2d_input_key_released(SDL_SCANCODE_LEFT) )
    {
        left = 0;
    }

    if(dash <= 0.0f)
    {
        walkDir = (float)(right - left);
        if(walkDir != 0.0f) phys->entity->anim->rend->flip.x = walkDir < 0.0f;
    }
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
    if( attacking >= 3 ) return;
        
    if( (!attacking || canCombo) ) attacking++;
    if(canCombo) canCombo = 0;
    
    walkDir = 0.0f;
}

uint8_t player_dash()
{
    uint8_t mod = gf2d_input_key_just_pressed(SDL_SCANCODE_C) || gf2d_input_joystick_button_pressed(0, 0);
    uint8_t dir = right || left;
    return (mod && dir) || dash > 0.0f;
}

uint8_t player_special_neutral()
{
    return gf2d_input_key_just_pressed(SDL_SCANCODE_X);
}

void player_special_neutral_perform(Entity *self)
{
    PhysicsEntity *proj = NULL;
    Sprite *projSprite = NULL;
    float fwd = 0.0f;
    uint32_t *anim;
    float animSpeed = 0.0f;
    float distance = 0.0f;
    if(!self) return;

    
    pj_spin_sword(&projSprite, &anim, &animSpeed, &fwd, &distance);
    
    proj = gf2d_projectile_create(
        self->position, 
        vector2d(-((float)(self->anim->rend->flip.x * 2) - 1.0f), 0.0f),
        fwd,
        distance,
        self
    );

    proj->entity->anim->rend->sprite = projSprite;
    gf2d_animation_play(proj->entity->anim, anim[0], anim[1]);
    proj->entity->anim->playbackSpeed = animSpeed;
    
    gf2d_scene_add_to_drawables(proj, DET_PHYS);
}