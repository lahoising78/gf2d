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
} PuntiJordanConfig;

static PuntiJordanConfig pj_config = {0};

void punti_jordan_load_anim_values(uint32_t *dst, float *speed, SJson *obj);

void punti_jordan_load(const char *filename)
{
    SJson *json = NULL;

    json = sj_load(filename);

    punti_jordan_load_anim_values(pj_config.idle, &pj_config.idleSpeed, sj_object_get_value(json, "idle"));
    punti_jordan_load_anim_values(pj_config.walking, &pj_config.walkingSpeed, sj_object_get_value(json, "walking"));

    sj_free(json);
}

void punti_jordan_load_anim_values(uint32_t *dst, float *speed, SJson *obj)
{
    dst[0] = gf2d_json_uint32( sj_object_get_value(obj, "animation") );
    dst[1] = gf2d_json_uint32( sj_object_get_value(obj, "maxFrame") );
    sj_get_float_value( sj_object_get_value(obj, "playbackSpeed"), speed );
}

uint32_t *punti_jordan_get_idle()
{
    return pj_config.idle;
}

float punti_jordan_get_idle_speed()
{
    return pj_config.idleSpeed;
}

uint32_t *punti_jordan_get_walking()
{
    return pj_config.walking;
}

float punti_jordan_get_walking_speed()
{
    return pj_config.walkingSpeed;
}