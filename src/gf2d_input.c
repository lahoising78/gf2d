#include "gf2d_input.h"
#include "simple_logger.h"

#define JOYSTICK_DEFAULT_DEAD_ZONE 8000
#define JOYSTICK_GET_AXIS_MAX 32767.0f

typedef struct
{
    SDL_Event           *currentKeys;
    int                 maxKeys;
    size_t              keysArraySize;

    SDL_Joystick        **joysticks;
    uint32_t            maxJoysticks;
    uint16_t            joystickDeadZone;
    
    SDL_Event           *joystickButtons;
    size_t              joystickButtonsSize;
    uint32_t            joystickMaxButtons;

} InputManager;

static InputManager gf2d_input_manager = {0};

void gf2d_input_manager_open_joysticks();
void gf2d_input_manager_close_joysticks();

void gf2d_input_close()
{
    slog("closing input manager");

    free( gf2d_input_manager.currentKeys );
    free( gf2d_input_manager.joystickButtons );
    gf2d_input_manager_close_joysticks();
    if(gf2d_input_manager.joysticks) free( gf2d_input_manager.joysticks );
}

void gf2d_input_manager_close_joysticks()
{
    int i;

    for(i = 0; i < gf2d_input_manager.maxJoysticks; i++)
    {
        if(!gf2d_input_manager.joysticks[i]) continue;
        SDL_JoystickClose( gf2d_input_manager.joysticks[i] );
        gf2d_input_manager.joysticks[i] = NULL;
    }
}

void gf2d_input_init( int maxKeys, uint32_t maxJoysticks )
{
    slog("initilizing input manager");

    if(maxKeys <= 0) return;
    gf2d_input_manager.maxKeys = maxKeys;

    SDL_PumpEvents();
    gf2d_input_manager.keysArraySize = maxKeys * sizeof(SDL_Event);
    gf2d_input_manager.currentKeys = (SDL_Event*)malloc( gf2d_input_manager.keysArraySize );
    atexit( gf2d_input_close );

    gf2d_input_manager.joystickDeadZone = JOYSTICK_DEFAULT_DEAD_ZONE;
    gf2d_input_manager.joysticks = (SDL_Joystick**)malloc( sizeof(SDL_Joystick*) * maxJoysticks );
    if( gf2d_input_manager.joysticks ) 
    {
        memset(gf2d_input_manager.joysticks, 0, sizeof(SDL_Joystick*) * maxJoysticks);
        gf2d_input_manager.maxJoysticks = maxJoysticks;
    }
    slog("There are %d joysticks connected", SDL_NumJoysticks());
    gf2d_input_manager_open_joysticks();

}

void gf2d_input_manager_open_joysticks()
{
    int i;
    int numButtons, numHats;
    gf2d_input_manager.joystickMaxButtons = 0;
    for(i = 0; i < SDL_NumJoysticks() && i < gf2d_input_manager.maxJoysticks; i++)
    {
        gf2d_input_manager.joysticks[i] = SDL_JoystickOpen(i);
        if( !gf2d_input_manager.joysticks[i] ) continue;
        
        /* find the max number of buttons all controlers can have */
        numButtons = SDL_JoystickNumButtons(gf2d_input_manager.joysticks[i]);
        if( numButtons > gf2d_input_manager.joystickMaxButtons )
            gf2d_input_manager.joystickMaxButtons = numButtons;
    }
    /* find how many sdl events we will need */
    gf2d_input_manager.joystickButtonsSize = sizeof(SDL_Event) * gf2d_input_manager.maxJoysticks * gf2d_input_manager.joystickMaxButtons;
    gf2d_input_manager.joystickButtons = (SDL_Event*)gfc_allocate_array(sizeof(SDL_Event), gf2d_input_manager.joystickButtonsSize);
}

void gf2d_input_update()
{
    SDL_Event e = {0};

    if( gf2d_input_manager.maxKeys <= 0 ) return;

    memset(gf2d_input_manager.currentKeys, 0, gf2d_input_manager.keysArraySize );
    memset(gf2d_input_manager.joystickButtons, 0, gf2d_input_manager.joystickButtonsSize );

    while( SDL_PollEvent(&e) )
    {
        switch (e.type)
        {
        case SDL_KEYUP:
        case SDL_KEYDOWN:
            if( e.key.keysym.scancode < gf2d_input_manager.maxKeys )
            {
                gf2d_input_manager.currentKeys[ e.key.keysym.scancode ] = e;
            }
            break;
        
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            gf2d_input_manager.joystickButtons[ e.jbutton.which * gf2d_input_manager.joystickMaxButtons + e.jbutton.button ] = e;
            // slog("joystick %d had event on button %d", e.jbutton.which, e.jbutton.button);
            break;

        default:
            break;
        }
    }

}

uint8_t gf2d_input_is_key_pressed( SDL_Scancode scancode )
{
    return (scancode < gf2d_input_manager.maxKeys)? gf2d_input_manager.currentKeys[ scancode ].type == SDL_KEYDOWN : 0;
}

uint8_t gf2d_input_key_just_pressed( SDL_Scancode scancode )
{
    return gf2d_input_is_key_pressed(scancode) && !gf2d_input_manager.currentKeys[scancode].key.repeat;
}

uint8_t gf2d_input_key_released( SDL_Scancode scancode )
{
    return (scancode < gf2d_input_manager.maxKeys)? gf2d_input_manager.currentKeys[ scancode ].type == SDL_KEYUP : 0;
}

float gf2d_input_joystick_get_axis(uint32_t joystickId, int axis)
{
    Sint16 state = 0.0f;
    if(joystickId >= gf2d_input_manager.maxJoysticks) return 0.0f;

    state = SDL_JoystickGetAxis( gf2d_input_manager.joysticks[joystickId], axis );
    if( state > -gf2d_input_manager.joystickDeadZone && state < gf2d_input_manager.joystickDeadZone ) state = 0;
    return (float)state/JOYSTICK_GET_AXIS_MAX;
}

uint8_t gf2d_input_joystick_button_pressed(uint32_t joystickId, int button)
{
    if( joystickId >= gf2d_input_manager.maxJoysticks ) return 0;
    if( button >= gf2d_input_manager.joystickMaxButtons ) return 0;
    return gf2d_input_manager.joystickButtons[ joystickId * gf2d_input_manager.joystickMaxButtons + button ].type == SDL_JOYBUTTONDOWN;
}

uint8_t gf2d_input_joystick_button_released(uint32_t joystickId, int button)
{
    if( joystickId >= gf2d_input_manager.maxJoysticks ) return 0;
    if( button >= gf2d_input_manager.joystickMaxButtons ) return 0;
    return gf2d_input_manager.joystickButtons[ joystickId * gf2d_input_manager.joystickMaxButtons + button ].type == SDL_JOYBUTTONUP;
}

uint8_t gf2d_input_joystick_get_hat(uint32_t joystickId, int hat, uint8_t hatDir)
{
    uint8_t result = SDL_JoystickGetHat(gf2d_input_manager.joysticks[joystickId], hat);
    if( hatDir == 255 ) return result;
    if( hatDir == SDL_HAT_CENTERED ) return result == SDL_HAT_CENTERED;
    return result & hatDir;
}