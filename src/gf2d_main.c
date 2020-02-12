#include "gf2d_main.h"

typedef struct
{
    char windowName[MAX_WINDOW_NAME_LENGTH];
    int renderWidth;
    int renderHeight;
    Vector4D bgcolor;
    uint8_t fullscreen;
    uint8_t drawCollisions;
} WindowConfiguration;

static WindowConfiguration gf2d_window_configuration = {
    "gf2d",
    1200,
    700,
    {0, 0, 0, 255},
    0,
    0
};

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