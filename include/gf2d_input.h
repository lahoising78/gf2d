#ifndef _GF2D_INPUT_H_
#define _GF2D_INPUT_H_

#include <SDL.h>
#include "gfc_vector.h"

/** 
 * @brief initialize the input manager
 * @param maxKeys the max amount of keys to retrieve
 * @note maxKeys goes according to sdl scancode values
 */
void gf2d_input_init( int maxKeys, uint32_t maxJoysticks, uint32_t maxMouseButtons );

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

/** 
 * @brief get the numeric value of an axis of the specified joystick
 * @param joystickId the id of the joystick to check
 * @param axis the axis to be checked
 * @return a float between -1.0f (inclusive) and 1.0f (inclusive)
 */
float gf2d_input_joystick_get_axis(uint32_t joystickId, int axis);

/** 
 * @brief check if button in joystick is being pressed
 * @param joystickId the id of the joystick to check
 * @param button which button the check for
 * @return 1 if button is being pressed, 0 otherwise
 */
uint8_t gf2d_input_joystick_button_pressed(uint32_t joystickId, int button);

/** 
 * @brief check if button in joystick has been released
 * @param joystickId the id of the joystick to check
 * @param button which button to check for
 * @return 1 if button was released, 0 otherwise
 */
uint8_t gf2d_input_joystick_button_released(uint32_t joystickId, int button);

uint8_t gf2d_input_joystick_get_hat(uint32_t joystickId, int hat, uint8_t hatDir);

uint8_t gf2d_input_mouse_just_pressed(uint32_t button);
uint8_t gf2d_input_mouse_released(uint32_t button);
void gf2d_input_mouse_position(int *x, int *y);

#endif