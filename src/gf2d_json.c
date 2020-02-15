#include "gf2d_json.h"

Vector2D gf2d_json_vector2d(SJson *src)
{
    Vector2D ret = {0};

    if(!src || !sj_is_array(src) || sj_array_get_count(src) < 2) return ret;
    
    sj_get_float_value(sj_array_get_nth(src, 0), &ret.x);
    sj_get_float_value(sj_array_get_nth(src, 1), &ret.y);

    return ret;
}

Vector3D gf2d_json_vector3d(SJson *src)
{
    Vector3D ret = {0};
    Vector2D buf = {0};

    if(!src || !sj_is_array(src) || sj_array_get_count(src) < 3) return ret;

    buf = gf2d_json_vector2d(src);
    vector2d_copy(ret, buf);
    sj_get_float_value( sj_array_get_nth(src, 2) , &ret.z);

    return ret;
}

Vector4D gf2d_json_vector4d(SJson *src)
{
    Vector4D ret = {0};
    Vector3D buf = {0};

    if(!src || !sj_is_array(src) || sj_array_get_count(src) < 4) return ret;

    buf = gf2d_json_vector3d(src);
    vector3d_copy(ret, buf);
    sj_get_float_value( sj_array_get_nth(src, 3) , &ret.w);

    return ret;
}

uint8_t gf2d_json_uint8( SJson *src )
{
    int res = 0;

    if( !src ) return 0;
    sj_get_integer_value(src, &res);

    return (uint8_t)res;
}

uint16_t gf2d_json_uint16( SJson *src )
{
    int res = 0;

    if( !src ) return 0;
    sj_get_integer_value(src, &res);

    return (uint16_t)res;
}

uint32_t gf2d_json_uint32( SJson *src )
{
    int res = 0;

    if( !src ) return 0;
    sj_get_integer_value(src, &res);

    return (uint32_t)res;
}