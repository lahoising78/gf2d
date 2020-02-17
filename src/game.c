#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "gf2d_main.h"

#include "gf2d_timer.h"
#include "gf2d_render_ent.h"
#include "gf2d_camera.h"
#include "gf2d_input.h"
#include "gf2d_scene.h"

#include "gf2d_entity.h"
#include "gf2d_physics_entity.h"
#include "gf2d_tilemap.h"
#include "gf2d_font.h"
#include "gf2d_ui.h"

float frameTime = 0.0f;

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    int i;

    Timer perSecond = gf2d_timer_new();
    Timer fTimer = gf2d_timer_new();

    /* reading arguments */
    for(i = 1; i < argc; i++)
    {
        printf( "argument %d: %s\n", i - 1, argv[i] );
    }

    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");

    gf2d_config();

    gf2d_graphics_initialize(
        gf2d_main_get_window_name(),
        gf2d_main_get_render_width(),
        gf2d_main_get_render_height(),
        gf2d_main_get_render_width(),
        gf2d_main_get_render_height(),
        gf2d_main_get_bgcolor(),
        gf2d_main_get_fullscreen()
    );
    gf2d_graphics_set_frame_delay( gf2d_engine_config_get_frame_delay() );
    gf2d_sprite_init( gf2d_engine_config_get_sprite_count() );
    gf2d_animation_manager_init( gf2d_engine_config_get_animation_count() );
    gf2d_entity_manager_init( gf2d_engine_config_get_entity_count() );
    gf2d_physics_entity_manager_init( gf2d_engine_config_get_physics_entity_count() );
    gf2d_tilemap_manager_init( gf2d_engine_config_get_tilemap_count() );
    gf2d_input_init( gf2d_engine_config_get_input_max_keys() , gf2d_engine_config_get_input_max_joysticks() );
    gf2d_camera_set_view( vector2d((float)gf2d_main_get_render_width(), (float)gf2d_main_get_render_height()) );
    gf2d_scene_awake_list_start( gf2d_engine_config_get_scene_awake_max() );
    gf2d_font_start();
    gf2d_font_add("fonts/doodlefont.ttf", 32);
    gf2d_label_manager_init(32);
    gf2d_ui_manager_init(32);
    
    /*demo setup*/
    gf2d_main();
    gf2d_timer_start(&perSecond);

    /*main game loop*/
    while(!done)
    {
        gf2d_timer_start(&fTimer);

        /* update input states */
        gf2d_input_update();

        /*update things here*/
        gf2d_entity_manager_update();
        gf2d_physics_entity_manager_update();
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            
            //branckground draw

            //entities and game objects
            gf2d_scene_render();
            
            //UI elements last

        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

        if ( gf2d_input_is_key_pressed(SDL_SCANCODE_ESCAPE) )
        {
            gf2d_scene_close();
            done = 1; // exit condition
        }
        if( gf2d_input_key_just_pressed(SDL_SCANCODE_TAB) )
        {
            gf2d_main_set_draw_collisions( !gf2d_main_get_draw_collisions() );
            slog("pressing tab");
        }
        if( gf2d_timer_get_ticks(&perSecond) >= 0.1f )
        {
            slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
            gf2d_timer_start(&perSecond);
        }
        
        frameTime = gf2d_timer_get_ticks(&fTimer);
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
