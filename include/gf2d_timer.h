#ifndef _GF2D_TIMER_H_
#define _GF2D_TIMER_H_

// #include <time.h>
#include <SDL.h>

typedef struct Timer_s
{
    uint32_t startTicks;
    uint32_t pausedTicks;

    uint8_t paused;
    uint8_t started;
} Timer;

/**
 * @brief create a new timer
 * @return a new timer
 */
Timer gf2d_timer_new();

/** 
 * @brief start the timer
 * @param timer the timer to be started
 */
void gf2d_timer_start(Timer *timer);

/** 
 * @brief stop timer
 * @param timer the timer to be stopped
 */
void gf2d_timer_stop(Timer *timer);

/** 
 * @brief pause timer
 * @param timer the timer to be paused
 */
void gf2d_timer_pause(Timer *timer);

/** 
 * @brief unpause timer
 * @param timer to be unpaused
 */
void gf2d_timer_unpause(Timer *timer);

/** 
 * @brief get ticks since timer started
 * @param timer to get ticks from
 * @return a floating point with ticks since timer started
 */
uint32_t gf2d_timer_get_ticks(Timer *timer);

#endif