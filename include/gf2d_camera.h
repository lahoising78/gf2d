#ifndef _GF2D_CAMERA_H_
#define _GF2D_CAMERA_H_

#include "gfc_vector.h"
#include "gf2d_collision.h"

void gf2d_camera_set_view(Vector2D view);
Vector2D gf2d_camera_get_view();

/** 
 * @brief get position after displacing by camera
 * @param position world position
 * @return position relative to camera
 */
Vector2D gf2d_camera_get_displaced_position(Vector2D position);
Vector2D gf2d_camera_get_position();

/** 
 * @brief move the camera for RenderEntities
 * @param displacement a 2d vector indicating how much to add to the position of the camera
 */
void gf2d_camera_move(Vector2D displacement);

/** 
 * @brief move the camera to the specified position 
 * @param position the position to set the camera to
 */
void gf2d_camera_set_position(Vector2D position);

CollisionShape gf2d_camera_collision_shape();

#endif