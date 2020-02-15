#include "simple_json.h"
#include "gf2d_main.h"
#include "gf2d_json.h"

typedef struct
{
    char windowName[MAX_WINDOW_NAME_LENGTH];
    int renderWidth;
    int renderHeight;
    Vector4D bgcolor;
    uint8_t fullscreen;
    uint8_t drawCollisions;
} WindowConfiguration;

typedef struct
{
    uint32_t frameDelay;
    uint32_t spriteCount;
    uint32_t animationCount;
    uint32_t entityCount;
    uint32_t physicsEntityCount;
    uint32_t tilemapCount;
    uint32_t inputMaxKeys;
    uint32_t inputMaxjoysticks;
} EngineConfiguration;

static WindowConfiguration gf2d_window_configuration = {0};
static EngineConfiguration gf2d_engine_configuration = {0};

void gf2d_config()
{
    SJson *json = NULL;
    SJson *obj = NULL;
    SJson *arr = NULL;

    json = sj_load("config/gf2d_main.json");
    if(!json) return;

    obj = sj_object_get_value(json, "windowName");
    if( obj )
        snprintf(gf2d_window_configuration.windowName, MAX_WINDOW_NAME_LENGTH, "%s", sj_get_string_value(obj));
    else
        strcpy(gf2d_window_configuration.windowName, "gf2d");

    obj = sj_object_get_value(json, "renderWidth");
    if(obj)
        sj_get_integer_value( obj, &gf2d_window_configuration.renderWidth );
    else 
        gf2d_window_configuration.renderWidth = 1200;

    obj = sj_object_get_value(json, "renderHeight");
    if(obj)
        sj_get_integer_value( obj, &gf2d_window_configuration.renderHeight );
    else 
        gf2d_window_configuration.renderHeight = 720;

    arr = sj_object_get_value(json, "bgcolor");
    if(arr)
    {
        gf2d_window_configuration.bgcolor = gf2d_json_vector4d(arr);
    }
    else 
        vector4d_set(gf2d_window_configuration.bgcolor, 0.0f, 0.0f, 0.0f, 255.0f);

    obj = sj_object_get_value(json, "fullscreen");
    if(obj)
        gf2d_window_configuration.fullscreen = gf2d_json_uint8(obj);
    else 
        gf2d_window_configuration.fullscreen = 0;

    obj = sj_object_get_value(json, "frameDelay");
    if(obj)
        gf2d_engine_configuration.frameDelay = gf2d_json_uint32(obj);
    else
        gf2d_engine_configuration.frameDelay = 16;
    
    obj = sj_object_get_value(json, "spriteCount");
    if(obj)
        gf2d_engine_configuration.spriteCount = gf2d_json_uint32(obj);
    else
        gf2d_engine_configuration.spriteCount = 4096;

    obj = sj_object_get_value(json, "animationCount");
    if(obj)
        gf2d_engine_configuration.animationCount = gf2d_json_uint32(obj);
    else
        gf2d_engine_configuration.animationCount = 2048;
    
    obj = sj_object_get_value(json, "entityCount");
    if(obj)
        gf2d_engine_configuration.entityCount = gf2d_json_uint32(obj);
    else
        gf2d_engine_configuration.entityCount = 1024;
    
    obj = sj_object_get_value(json, "physicsEntityCount");
    if(obj)
        gf2d_engine_configuration.physicsEntityCount = gf2d_json_uint32(obj);
    else
        gf2d_engine_configuration.physicsEntityCount = 256;
    
    obj = sj_object_get_value(json, "tilemapCount");
    if(obj)
        gf2d_engine_configuration.tilemapCount = gf2d_json_uint32(obj);
    else
        gf2d_engine_configuration.tilemapCount = 8;
    
    obj = sj_object_get_value(json, "inputMaxKeys");
    if(obj)
        gf2d_engine_configuration.inputMaxKeys = gf2d_json_uint32(obj);
    else
        gf2d_engine_configuration.inputMaxKeys = 512;
    
    obj = sj_object_get_value(json, "inputMaxjoysticks");
    if(obj)
        gf2d_engine_configuration.inputMaxjoysticks = gf2d_json_uint32(obj);
    else
        gf2d_engine_configuration.inputMaxjoysticks = 8;
    
    sj_free(json);
}

void gf2d_main_set_window_name( char *name )
{
    strncpy( gf2d_window_configuration.windowName, name, MAX_WINDOW_NAME_LENGTH );
}

char *gf2d_main_get_window_name()
{
    return gf2d_window_configuration.windowName;
}

void gf2d_main_set_render_width( int w )
{
    gf2d_window_configuration.renderWidth = w;
}

int gf2d_main_get_render_width()
{
    return gf2d_window_configuration.renderWidth;
}

void gf2d_main_set_render_height( int h )
{
    gf2d_window_configuration.renderHeight = h;
}

int gf2d_main_get_render_height()
{
    return gf2d_window_configuration.renderHeight;
}

void gf2d_main_set_bgcolor(Vector4D color)
{
    vector4d_copy(gf2d_window_configuration.bgcolor, color);
}

Vector4D gf2d_main_get_bgcolor()
{
    return gf2d_window_configuration.bgcolor;
}

void gf2d_main_set_fullscreen( uint8_t fullscreen )
{
    gf2d_window_configuration.fullscreen = fullscreen;
}

uint8_t gf2d_main_get_fullscreen()
{
    return gf2d_window_configuration.fullscreen;
}

void gf2d_main_set_draw_collisions(uint8_t draw)
{
    gf2d_window_configuration.drawCollisions = draw;
}

uint8_t gf2d_main_get_draw_collisions()
{
    return gf2d_window_configuration.drawCollisions;
}

/* ==================ENGINE CONFIG=============== */
uint32_t gf2d_engine_config_get_frame_delay()
{
    return gf2d_engine_configuration.frameDelay;
}

uint32_t gf2d_engine_config_get_sprite_count()
{
    return gf2d_engine_configuration.spriteCount;
}

uint32_t gf2d_engine_config_get_animation_count()
{
    return gf2d_engine_configuration.animationCount;
}

uint32_t gf2d_engine_config_get_entity_count()
{
    return gf2d_engine_configuration.entityCount;
}

uint32_t gf2d_engine_config_get_physics_entity_count()
{
    return gf2d_engine_configuration.physicsEntityCount;
}

uint32_t gf2d_engine_config_get_tilemap_count()
{
    return gf2d_engine_configuration.tilemapCount;
}

uint32_t gf2d_engine_config_get_input_max_keys()
{
    return gf2d_engine_configuration.inputMaxKeys;
}

uint32_t gf2d_engine_config_get_input_max_joysticks()
{
    return gf2d_engine_configuration.inputMaxjoysticks;
}