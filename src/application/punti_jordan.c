#include "simple_logger.h"
#include "gfc_types.h"
#include "gf2d_json.h"
#include "punti_jordan.h"

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
    uint32_t                slashUp[2];
    float                   slashUpSpeed;
    uint32_t                slashSide[2];
    float                   slashSideSpeed;
} PuntiJordanConfig;

static PuntiJordanConfig pj_config = {0};

void punti_jordan_load_anim_values(uint32_t *dst, float *speed, SJson *obj);

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

    sj_free(json);
}

void punti_jordan_load_anim_values(uint32_t *dst, float *speed, SJson *obj)
{
    dst[0] = gf2d_json_uint32( sj_object_get_value(obj, "animation") );
    dst[1] = gf2d_json_uint32( sj_object_get_value(obj, "maxFrame") );
    sj_get_float_value( sj_object_get_value(obj, "playbackSpeed"), speed );
}

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