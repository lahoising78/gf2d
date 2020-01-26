#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "simple_logger.h"

#include "gf2d_timer.h"
#include "gf2d_render_ent.h"
#include "gf2d_camera.h"

#include "gf2d_entity.h"
#include "gf2d_physics_entity.h"

#define ENTITY_COUNT                1024
#define PHYSICS_ENTITY_COUNT        256

float frameTime = 0.0f;
int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    int i;
    const Uint8 * keys;
    Sprite *sprite;
    
    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};

    Timer perSecond = gf2d_timer_new();
    Timer fTimer = gf2d_timer_new();
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);
    gf2d_entity_manager_init(ENTITY_COUNT);
    gf2d_physics_entity_manager_init(PHYSICS_ENTITY_COUNT);
    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
    gf2d_timer_start(&perSecond);

    /*main game loop*/
    while(!done)
    {
        gf2d_timer_start(&fTimer);

        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
        mf+=0.1;
        if (mf >= 16.0)mf = 0;
        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
            gf2d_sprite_draw_image(sprite,vector2d(0,0));
            
            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
        if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
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
