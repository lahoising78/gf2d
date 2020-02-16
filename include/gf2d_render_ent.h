#ifndef _GF2D_RENDER_ENT_H_
#define _GF2D_RENDER_ENT_H_

#include "gf2d_sprite.h"
#include "gf2d_json.h"

typedef struct
{
    Sprite              *sprite;
    Vector2D            position;
    Vector2D            *scale;
    Vector2D            *scaleCenter;
    Vector3D            *rotation;
    Vector2D            *flip;
    Vector4D            *colorShift;
    float               frame;
} RenderEntity;

/** 
 * @brief allocate a new RenderEntity
 * @param sprite the sprite the render entity will have
 * @return a pointer to the RenderEntity
 */
RenderEntity *gf2d_render_ent_new( Sprite *sprite );

RenderEntity *gf2d_render_ent_load( SJson *json );

/** 
 * @brief clear data from render entity
 * @note this does not remove the render entity from heap
 * @param ent render ent to free
 */
void gf2d_render_ent_free( RenderEntity *ent );

/** 
 * @brief draw the render entity
 * @param ent the ent to be drawn
 */
void gf2d_render_ent_draw(RenderEntity *ent);

#endif