#ifndef _GF2D_MAIN_H_
#define _GF2D_MAIN_H_

#include "gfc_vector.h"

#define MAX_WINDOW_NAME_LENGTH 64

/** 
 * @brief set the name of the window
 * @param name the string to set the window name to
 */
void gf2d_main_set_window_name( char *name );

/** 
 * @brief get the window name
 * @return the window name
 */
char *gf2d_main_get_window_name();

/** 
 * @brief set the render width of the window
 * @param w the width to set the window to
 */
void gf2d_main_set_render_width( int w );

/** 
 * @brief get the render width
 * @return the width of the window
 */
int gf2d_main_get_render_width();

/** 
 * @brief set the render height of the window
 * @param h the height to set the window height to
 */
void gf2d_main_set_render_height( int h );

/** 
 * @brief set get render height of the window
 * @return the render height of the window
 */
int gf2d_main_get_render_height();

/** 
 * @brief set the background color
 * @param color the color to set the background color to
 */
void gf2d_main_set_bgcolor(Vector4D color);

/** 
 * @brief get the background color
 * @return the background color
 */
Vector4D gf2d_main_get_bgcolor();

/** 
 * @brief tell the game if the window should be fullscreen or not
 * @param fullscreen 1 for fullscreen and 0 otherwise
 */
void gf2d_main_set_fullscreen( uint8_t fullscreen );

/** 
 * @brief get fullscreen flag
 * @return 1 for fullscreen and 0 otherwise
 */
uint8_t gf2d_main_get_fullscreen();

void gf2d_main_set_draw_collisions(uint8_t draw);
uint8_t gf2d_main_get_draw_collisions();

/* TO BE OVERRIDEN */

/** 
 * @brief configure the game's window
 * @note needs to be overriden by the user in a file in application/ folder
 */
void gf2d_config();

/** 
 * @brief the entry point of the game for the user
 * @note need to be overriden by the user in a file in application/ folder
 */
void gf2d_main();


#endif