#ifndef _GF2D_JSON_H_
#define _GF2D_JSON_H_

#include "simple_json.h"
#include "gfc_vector.h"
#include "gf2d_sprite.h"

Vector2D gf2d_json_vector2d(SJson *src);
Vector3D gf2d_json_vector3d(SJson *src);
Vector4D gf2d_json_vector4d(SJson *src);

uint8_t gf2d_json_uint8( SJson *src );
uint16_t gf2d_json_uint16( SJson *src );
uint32_t gf2d_json_uint32( SJson *src );

Sint8 gf2d_json_sint8( SJson *src );
Sint16 gf2d_json_sint16( SJson *src );
Sint32 gf2d_json_sint32( SJson *src );

Sprite *gf2d_json_sprite(SJson *src);

#endif