#ifndef _GF2D_INPUT_H_
#define _GF2D_INPUT_H_

#include <SDL.h>
#include "gfc_vector.h"

/** 
 * @brief initialize the input manager
 * @param maxKeys the max amount of keys to retrieve
 * @note maxKeys goes according to sdl scancode values
 */
void gf2d_input_init(int maxKeys, uint32_t maxJoysticks);

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

/** 
 * @brief check if a certain key has just been pressed
 * @param scancode Scancode of the key to check
 * @return 1 if the key was just pressed and 0 otherwise
 */
uint8_t gf2d_input_key_just_pressed( SDL_Scancode scancode );

/** 
 * @brief check if a certain key has just been released
 * @param scancode Scancode of the key to check
 * @return 1 if key was just released and 0 otherwise
 */
uint8_t gf2d_input_key_released( SDL_Scancode scancode );

float gf2d_input_joystick_get_axis(uint32_t joystickId, int axis);

#endif