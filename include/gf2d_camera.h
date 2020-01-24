#ifndef _GF2D_CAMERA_H_
#define _GF2D_CAMERA_H_

#include "gfc_vector.h"

/** 
 * @brief get position after displacing by camera
 * @param position world position
 * @return position relative to camera
 */
Vector2D gf2d_camera_get_displaced_position(Vector2D position);

/** 
 * @brief move the camera for RenderEntities
 * @param displacement a 2d vector indicating how much to add to the position of the camera
 */
void gf2d_camera_move(Vector2D displacement);

#endif