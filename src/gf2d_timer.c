#include "gf2d_timer.h"
#include "simple_logger.h"

Timer gf2d_timer_new()
{
    Timer timer;
    timer.startTicks = 0;
    timer.pausedTicks = 0;
    timer.started = 0;
    timer.paused = 0;
    return timer;
}

void gf2d_timer_start(Timer *timer)
{
    timer->started = 1;
    timer->paused = 0;

    timer->startTicks = SDL_GetTicks();
    
    timer->pausedTicks = 0;
}

void gf2d_timer_stop(Timer *timer)
{
    timer->started = 0;
    timer->paused = 0;

    timer->startTicks = 0;
    timer->pausedTicks = 0;
}

void gf2d_timer_pause(Timer *timer)
{
    if (!timer->started || timer->paused)
    {
        return;
    }

    timer->paused = 1;
    timer->pausedTicks = SDL_GetTicks();
    
    timer->pausedTicks -= timer->startTicks;
    timer->startTicks = 0;
}

void gf2d_timer_unpause(Timer *timer)
{
    if ( !timer->started || !timer->paused )
    {
        return;
    }

    timer->paused = 0;
    timer->startTicks = SDL_GetTicks();
    
    timer->startTicks -= timer->pausedTicks;
    timer->pausedTicks = 0;
}

uint32_t gf2d_timer_get_ticks(Timer *timer)
{
    uint32_t t = 0;
    
    if ( !timer->started )
    {
        return t;
    }

    if ( timer->paused )
    {
        t = timer->pausedTicks;
    }
    else
    {
        t = SDL_GetTicks();
        t -= timer->startTicks;
    }

    return t;

}