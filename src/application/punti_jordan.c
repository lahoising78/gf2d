#include "simple_logger.h"
#include "gfc_types.h"
#include "gf2d_json.h"
#include "gf2d_collision.h"
#include "punti_jordan.h"

#define ATTACK_CONFIG(var) CollisionShape var##Box; uint32_t var##Start;

#define PJ_ATTACK_GET(var) PJ_ATTACK_SETUP(var) { \
    if(hitbox) *hitbox = pj_config.var##Box; \
    if(frame) *frame = pj_config.var##Start; \
}

typedef struct
{
    uint32_t                idle[2];
    float                   idleSpeed;

    uint32_t                walking[2];
    float                   walkingSpeed;
    
    uint32_t                jumping[2];
    float                   jumpingSpeed;
    
    uint32_t                slashDown[2];
    float                   slashDownSpeed;
    ATTACK_CONFIG           (slashDown)
    
    uint32_t                slashUp[2];
    float                   slashUpSpeed;
    ATTACK_CONFIG           (slashUp)
    
    uint32_t                slashSide[2];
    float                   slashSideSpeed;
    ATTACK_CONFIG           (slashSide)
    
    float                   dash[2];

    uint32_t                swordThrow[2];
    float                   swordThrowSpeed;

    Sprite                  *spinSwordSprite;
    uint32_t                spinSwordAnim[2];
    float                   spinSwordSpeed;
    float                   spinSwordFwd;
    float                   spinSwordDistance;
    float                   spinSwordDamage;
    CollisionShape          spinSwordHitbox;

    uint32_t                tornado[2];
    float                   tornadoSpeed;
    
    uint32_t                recover[2];
    float                   recoverSpeed;
    
    uint32_t                downAttack[2];
    float                   downAttackSpeed;

    uint32_t                taunt[2];
    float                   tauntSpeed;
} PuntiJordanConfig;

static PuntiJordanConfig pj_config = {0};

void punti_jordan_load_anim_values(uint32_t *dst, float *speed, SJson *obj);
void punti_jordan_load_attk_values(CollisionShape *shape, uint32_t *start, SJson *obj);
void punti_jordan_load_dash(SJson *obj);
void punti_jordan_load_spin_sword( SJson *obj );

#define PJ_LOAD_ATTACK(var, json) punti_jordan_load_attk_values( &pj_config.var##Box, &pj_config.var##Start, sj_object_get_value(json, #var) )

void punti_jordan_load(const char *filename)
{
    SJson *json = NULL;

    json = sj_load(filename);

    punti_jordan_load_anim_values(pj_config.idle, &pj_config.idleSpeed, sj_object_get_value(json, "idle"));
    punti_jordan_load_anim_values(pj_config.walking, &pj_config.walkingSpeed, sj_object_get_value(json, "walking"));
    punti_jordan_load_anim_values(pj_config.jumping, &pj_config.jumpingSpeed, sj_object_get_value(json, "jumping"));
    punti_jordan_load_anim_values(pj_config.slashDown, &pj_config.slashDownSpeed, sj_object_get_value(json, "slashDown"));
    punti_jordan_load_anim_values(pj_config.slashUp, &pj_config.slashUpSpeed, sj_object_get_value(json, "slashUp"));
    punti_jordan_load_anim_values(pj_config.slashSide, &pj_config.slashSideSpeed, sj_object_get_value(json, "slashSide"));
    punti_jordan_load_anim_values(pj_config.spinSwordAnim, &pj_config.spinSwordSpeed, sj_object_get_value(json, "spinSword"));
    punti_jordan_load_anim_values(pj_config.swordThrow, &pj_config.swordThrowSpeed, sj_object_get_value(json, "swordThrow"));
    punti_jordan_load_anim_values(pj_config.tornado, &pj_config.tornadoSpeed, sj_object_get_value(json, "tornado"));
    punti_jordan_load_anim_values(pj_config.recover, &pj_config.recoverSpeed, sj_object_get_value(json, "recover"));
    punti_jordan_load_anim_values(pj_config.downAttack, &pj_config.downAttackSpeed, sj_object_get_value(json, "downAttack"));
    punti_jordan_load_anim_values(pj_config.taunt, &pj_config.tauntSpeed, sj_object_get_value(json, "taunt"));

    punti_jordan_load_dash( sj_object_get_value(json, "dash") );
    punti_jordan_load_spin_sword( sj_object_get_value(json, "spinSword") );

    PJ_LOAD_ATTACK(slashDown, json);
    PJ_LOAD_ATTACK(slashUp, json);
    PJ_LOAD_ATTACK(slashSide, json);

    sj_free(json);
}

void punti_jordan_load_anim_values(uint32_t *dst, float *speed, SJson *obj)
{
    dst[0] = gf2d_json_uint32( sj_object_get_value(obj, "animation") );
    dst[1] = gf2d_json_uint32( sj_object_get_value(obj, "maxFrame") );
    sj_get_float_value( sj_object_get_value(obj, "playbackSpeed"), speed );
}

void punti_jordan_load_attk_values(CollisionShape *shape, uint32_t *start, SJson *obj)
{
    if(!obj) return;

    if(shape)
        *shape = gf2d_collision_shape_load( sj_object_get_value(obj, "hitbox") );

    if(start)
        *start = gf2d_json_uint32( sj_object_get_value(obj, "start") );
}

void punti_jordan_load_dash(SJson *obj)
{
    sj_get_float_value( sj_object_get_value(obj, "time"), &pj_config.dash[0] );
    sj_get_float_value( sj_object_get_value(obj, "multiplier"), &pj_config.dash[1] );
}

void punti_jordan_load_spin_sword( SJson *obj )
{
    pj_config.spinSwordSprite = gf2d_json_sprite( sj_object_get_value(obj, "sprite") );
    sj_get_float_value( sj_object_get_value(obj, "fwd"), &pj_config.spinSwordFwd );
    sj_get_float_value( sj_object_get_value(obj, "distance"), &pj_config.spinSwordDistance );
    sj_get_float_value( sj_object_get_value(obj, "damage"), &pj_config.spinSwordDamage );
    pj_config.spinSwordHitbox = gf2d_collision_shape_load( sj_object_get_value(obj, "hitbox") );
}

/* --====== GET ANIM ======-- */
uint32_t *pj_anim_idle()
{
    return pj_config.idle;
}

float pj_anim_idle_speed()
{
    return pj_config.idleSpeed;
}

uint32_t *pj_anim_walking()
{
    return pj_config.walking;
}

float pj_anim_walking_speed()
{
    return pj_config.walkingSpeed;
}

uint32_t *pj_anim_jumping()
{
    return pj_config.jumping;
}

float pj_anim_jumping_speed()
{
    return pj_config.jumpingSpeed;
}

uint32_t *pj_anim_slash_down()
{
    return pj_config.slashDown;
}

float pj_anim_slash_down_speed()
{
    return pj_config.slashDownSpeed;
}

uint32_t *pj_anim_slash_up()
{
    return pj_config.slashUp;
}

float pj_anim_slash_up_speed()
{
    return pj_config.slashUpSpeed;
}

uint32_t *pj_anim_slash_side()
{
    return pj_config.slashSide;
}

float pj_anim_slash_side_speed()
{
    return pj_config.slashSideSpeed;
}

float *pj_dash()
{
    return pj_config.dash;
}

uint32_t *pj_anim_sword_throw()
{
    return pj_config.swordThrow;
}

float pj_anim_sword_throw_speed()
{
    return pj_config.swordThrowSpeed;
}

void pj_spin_sword(Sprite **sprite, uint32_t **anim, float *animSpeed, float *fwd, float *distance, CollisionShape *hitbox)
{
    if(sprite) *sprite = pj_config.spinSwordSprite;
    if(anim) *anim = pj_config.spinSwordAnim;
    if(animSpeed) *animSpeed = pj_config.spinSwordSpeed;
    if(fwd) *fwd = pj_config.spinSwordFwd;
    if(distance) *distance = pj_config.spinSwordDistance;
    if(hitbox) *hitbox = pj_config.spinSwordHitbox;
}

float pj_spin_sword_damage()
{
    return pj_config.spinSwordDamage;
}

uint32_t *pj_anim_tornado()
{
    return pj_config.tornado;
}

float pj_anim_tornado_speed()
{
    return pj_config.tornadoSpeed;
}

uint32_t *pj_anim_recover()
{
    return pj_config.recover;
}

float pj_anim_recover_speed()
{
    return pj_config.recoverSpeed;
}

uint32_t *pj_anim_down_attack()
{
    return pj_config.downAttack;
}

float pj_anim_down_attack_speed()
{
    return pj_config.downAttackSpeed;
}

uint32_t *pj_anim_taunt()
{
    return pj_config.taunt;
}

float pj_anim_taunt_speed()
{
    return pj_config.tauntSpeed;
}

PJ_ATTACK_GET(slashDown)
PJ_ATTACK_GET(slashUp)
PJ_ATTACK_GET(slashSide)