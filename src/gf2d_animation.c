#include "gf2d_animation.h"
#include "simple_logger.h"

typedef struct
{
    Animation   *animations;
    uint32_t    count;
} AnimationManager;

static AnimationManager gf2d_animation_manager = {0};

void gf2d_animation_manager_init_all_animations();
void gf2d_animation_manager_close();

void gf2d_animation_manager_init( uint32_t count )
{
    gf2d_animation_manager.animations = (Animation*)gfc_allocate_array(sizeof(Animation), count);
    if(!gf2d_animation_manager.animations)
    {
        slog("Not enough space for animations");
        return;
    }
    memset(gf2d_animation_manager.animations, 0, sizeof(Animation) * count);
    gf2d_animation_manager.count = count;

    gf2d_animation_manager_init_all_animations();
    atexit(gf2d_animation_manager_close);
}

void gf2d_animation_manager_init_all_animations()
{
    int i;
    Animation *anim = NULL;

    for(i = 0; i < gf2d_animation_manager.count; i++)
    {
        anim = &gf2d_animation_manager.animations[i];
        anim->rend = gf2d_render_ent_new(NULL);
    }
}

void gf2d_animation_manager_close()
{
    gf2d_animation_manager_clean( 1 );
    if( gf2d_animation_manager.animations ) free(gf2d_animation_manager.animations);
}

void gf2d_animation_manager_clean( uint8_t del )
{
    int i;
    Animation *anim = NULL;

    for(i = 0; i < gf2d_animation_manager.count; i++)
    {
        anim = &gf2d_animation_manager.animations[i];
        gf2d_animation_free(anim, del);
    }
}

void gf2d_animation_manager_wipe()
{
}

void gf2d_animation_init(Animation *anim)
{
    if(!anim) return;
    anim->_inuse = 1;
    anim->playbackSpeed = 1.0f;
}

Animation *gf2d_animation_new()
{
    int i;
    Animation *anim = NULL;

    for(i = 0; i < gf2d_animation_manager.count; i++)
    {
        anim = &gf2d_animation_manager.animations[i];
        if(anim->_inuse) continue;

        gf2d_animation_init(anim);

        return anim;
    }

    return NULL;
}

Animation *gf2d_animation_load( SJson *json )
{
    SJson *obj = NULL;
    Animation *anim = NULL;

    if( sj_is_string(json) )
    {
        obj = sj_load( sj_get_string_value(json) );
            anim = gf2d_animation_load(obj);
        sj_free(obj);
        return anim;
    }

    anim = gf2d_animation_new();
    gf2d_animation_load_to_animation(anim, json);

    return anim;
}

void gf2d_animation_load_to_animation( Animation *anim, SJson *json )
{
    uint32_t animation = 0;
    uint32_t maxFrame = 0;

    SJson *obj = NULL;
    if(!anim) return;
    if(!json) 
    {
        gf2d_animation_free(anim, 0);
        return;
    }

    if( !anim->rend ) 
    {
        anim->rend = gf2d_render_ent_new(NULL);
    }
    
    anim->rend->sprite = gf2d_json_sprite( sj_object_get_value(json, "sprite") );
    anim->rend->position = gf2d_json_vector2d( sj_object_get_value(json, "position") );
    sj_get_float_value( sj_object_get_value(json, "playbackSpeed"), &anim->playbackSpeed );
    
    anim->rend->scale = gf2d_json_vector2d( sj_object_get_value(json, "scale") );
    if( anim->rend->scale.x == 0 )
        vector2d_set(anim->rend->scale, 1.0f, 1.0f);

    obj = sj_object_get_value(json, "playing");
    if(obj)
    {
        animation = gf2d_json_uint32( sj_object_get_value(obj, "animation") );
        maxFrame = gf2d_json_uint32( sj_object_get_value(obj, "maxFrame") );
        gf2d_animation_play(anim, animation, maxFrame);
    }
    else
    {
        anim->playing = 0;
        anim->maxFrame = 1;
    }
}

void gf2d_animation_free(Animation *anim, uint8_t del)
{
    RenderEntity *rend = NULL;
    if(!anim) return;

    rend = anim->rend;
    gf2d_render_ent_free(anim->rend);
    memset(anim, 0, sizeof(Animation));

    if(del) free(rend);
    else anim->rend = rend;
}

void gf2d_animation_play( Animation *anim, uint32_t animation, uint32_t maxFrame )
{
    if(!anim) return;

    if(anim->rend && anim->rend->sprite) 
    {
        anim->rend->frame = anim->rend->sprite->frames_per_line * animation;
    }
    anim->maxFrame = maxFrame;
    anim->animation = animation;
    anim->playing = 1;
    if(anim->playbackSpeed == 0.0f) anim->playbackSpeed = 1.0f;
}

void gf2d_animation_pause( Animation *anim )
{
    if(!anim) return;

    anim->playing = 0;
}

extern float frameTime;
void gf2d_animation_render( Animation *anim )
{
    if(!anim || !anim->rend || !anim->rend->sprite) return;
    
    if(anim->playing)
    {
        anim->rend->frame += frameTime * anim->playbackSpeed * 250.0f;
        if( anim->rend->frame >= (float)(anim->maxFrame + anim->rend->sprite->frames_per_line * anim->animation))
        {
            anim->rend->frame -= (float)anim->maxFrame;
        }
    }

    gf2d_render_ent_draw(anim->rend);
}