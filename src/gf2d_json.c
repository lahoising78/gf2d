#include "gf2d_json.h"
#include "simple_logger.h"

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

Sint8 gf2d_json_sint8( SJson *src )
{
    int res = 0;

    if( !src ) return 0;
    sj_get_integer_value(src, &res);

    return (Sint8)res;
}

Sint16 gf2d_json_sint16( SJson *src )
{
    int res = 0;

    if( !src ) return 0;
    sj_get_integer_value(src, &res);

    return (Sint16)res;
}

Sint32 gf2d_json_sint32( SJson *src )
{
    int res = 0;

    if( !src ) return 0;
    sj_get_integer_value(src, &res);

    return (Sint32)res;
}

Sprite *gf2d_json_sprite(SJson *src)
{
    const char *filename;
    Sint32 w = 0;
    Sint32 h = 0;
    Sint32 framesPerLine = 0;
    Sprite *sprite = NULL;

    SJson *obj = NULL;
    if(!src) return NULL;

    if( sj_is_string(src) )
    {
        obj = sj_load( sj_get_string_value(src) );
            sprite = gf2d_json_sprite(obj);
        sj_free(obj);
        return sprite;
    }

    // slog("sprite load");

    obj = sj_object_get_value(src, "filename");
    if(!obj) return NULL;
    filename = sj_get_string_value(obj);

    obj = sj_object_get_value(src, "framesPerLine");
    if(!obj) 
    {
        slog("return sprite %s", filename);
        return gf2d_sprite_load_image( filename );
    }
    framesPerLine = gf2d_json_sint32(obj);

    obj = sj_object_get_value(src, "frameWidth");
    if(obj) w = gf2d_json_sint32(obj);

    obj = sj_object_get_value(src, "frameHeight");
    if(obj) h = gf2d_json_sint32(obj);

    slog("sprite %s %d %d %d", filename, w, h, framesPerLine);
    return gf2d_sprite_load_all(filename, w, h, framesPerLine);
}