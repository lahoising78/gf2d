#ifndef _GF2D_INPUT_H_
#define _GF2D_INPUT_H_

#include <SDL.h>

/** 
 * @brief initialize the input manager
 * @param maxKeys the max amount of keys to retrieve
 * @note maxKeys goes according to sdl scancode values
 */
void gf2d_input_init(int maxKeys);

/** 
 * @brief update the state of inputs
 */
void gf2d_input_update();

/** 
 * @brief check is a certain key is being pressed
 * @param scancode Scancode of the key to check
 * @return 1 if it is being pressed and 0 otherwise
 */
uint8_t gf2d_input_is_key_pressed( SDL_Scancode scancode );

uint8_t gf2d_input_key_just_pressed( SDL_Scancode scancode );

#endif