#include "gf2d_scene.h"
#include "simple_logger.h"
#include "gf2d_tilemap.h"
#include "gf2d_main.h"

typedef struct
{
    union
    {
        RenderEntity    *rend;
        Animation       *anim;
        Entity          *ent;
        PhysicsEntity   *phys;
        Tilemap         *tmap;
    } drawable;
    DrawableEntityType  _type;
    uint8_t             _inuse;
} DrawableEntity;

typedef struct
{
    char                name[GFCLINELEN];
    DrawableEntity      *drawable_entities;
    uint32_t            drawable_entities_count;
    uint32_t            drawable_entities_size;
} Scene;

static Scene gf2d_scene = {0};

void gf2d_scene_load( uint32_t drawablesCount, void (*scene_awake)() )
{
    slog("--== Loading new scene ==--");

    gf2d_scene_close();

    gf2d_physics_entity_manager_clean(1);
    gf2d_entity_manager_clean(1);
    gf2d_animation_manager_clean(1);
    gf2d_animation_manager_init_all_animations();
    gf2d_entity_manager_initialize_all_entities();
    gf2d_physics_entity_manager_initialize_entities();

    gf2d_scene.drawable_entities = (DrawableEntity*)gfc_allocate_array(sizeof(DrawableEntity), drawablesCount);
    if( gf2d_scene.drawable_entities ) 
    {
        gf2d_scene.drawable_entities_size = drawablesCount;
        gf2d_scene.drawable_entities_count = 0;
        memset(gf2d_scene.drawable_entities, 0, sizeof(DrawableEntity)*gf2d_scene.drawable_entities_size);
    }

    if(scene_awake) scene_awake();
}

void gf2d_scene_close()
{
    if( gf2d_scene.drawable_entities ) 
    {
        free(gf2d_scene.drawable_entities);
    }
    gf2d_scene.drawable_entities_count = gf2d_scene.drawable_entities_size = 0;
    memset(&gf2d_scene, 0, sizeof(Scene));
}

void gf2d_scene_render()
{
    int i;
    DrawableEntity *ent = NULL;

    for(i = 0; i < gf2d_scene.drawable_entities_count; i++)
    {
        ent = &gf2d_scene.drawable_entities[i];
        if(!ent->_inuse) continue;

        switch (ent->_type)
        {
            case DET_REND:

                if(!ent->drawable.rend) continue;
                gf2d_render_ent_draw(ent->drawable.rend);

                break;
            
            case DET_ANIM:

                if( !ent->drawable.anim->rend ) continue;
                // slog("%.2f %.2f", ent->drawable.anim->rend->position.x, ent->drawable.anim->rend->position.y);
                gf2d_animation_render(ent->drawable.anim);

                break;

            case DET_ENT:
                
                if( !ent->drawable.ent->anim || !ent->drawable.ent->anim->rend ) continue;

                vector2d_add(ent->drawable.ent->anim->rend->position, ent->drawable.ent->anim->rend->position, ent->drawable.ent->position);
                // slog("%.2f %.2f", ent->drawable.ent->anim->rend->position.x, ent->drawable.ent->anim->rend->position.y);
                    gf2d_animation_render(ent->drawable.ent->anim);
                vector2d_sub(ent->drawable.ent->anim->rend->position, ent->drawable.ent->anim->rend->position, ent->drawable.ent->position);

                break;

            case DET_PHYS:

                if( !ent->drawable.phys->entity->anim || !ent->drawable.phys->entity->anim->rend ) continue;

                vector2d_add(ent->drawable.phys->entity->anim->rend->position, ent->drawable.phys->entity->anim->rend->position, ent->drawable.phys->entity->position);
                // slog("%.2f %.2f", ent->drawable.phys->entity->anim->rend->position.x, ent->drawable.phys->entity->anim->rend->position.y);
                    gf2d_animation_render(ent->drawable.phys->entity->anim);
                    if( gf2d_main_get_draw_collisions() )
                    {
                        vector2d_add( ent->drawable.phys->modelBox.position, ent->drawable.phys->modelBox.position, ent->drawable.phys->entity->position );
                        gf2d_collision_draw( &ent->drawable.phys->modelBox );
                        vector2d_sub( ent->drawable.phys->modelBox.position, ent->drawable.phys->modelBox.position, ent->drawable.phys->entity->position );
                    }
                vector2d_sub(ent->drawable.phys->entity->anim->rend->position, ent->drawable.phys->entity->anim->rend->position, ent->drawable.phys->entity->position);

                break;

            case DET_TMAP:
                // if( !ent->drawable.tmap->rend ) continue;
                gf2d_tilemap_render(ent->drawable.tmap);
                break;
        
        default:
            break;
        }

    }
}

int gf2d_scene_add_to_drawables( void *de, DrawableEntityType type )
{
    DrawableEntity *drawable = NULL;

    if(gf2d_scene.drawable_entities_count >= gf2d_scene.drawable_entities_size) return -1;

    drawable = &gf2d_scene.drawable_entities[ gf2d_scene.drawable_entities_count++ ];
    drawable->_inuse = 1;
    drawable->_type = type;
    switch (type)
    {
    case DET_REND:
        drawable->drawable.rend = (RenderEntity*)de;
        break;

    case DET_ANIM:
        drawable->drawable.anim = (Animation*)de;
        break;

    case DET_ENT:
        drawable->drawable.ent = (Entity*)de;
        break;

    case DET_PHYS:
        drawable->drawable.phys = (PhysicsEntity*)de;
        break;

    case DET_TMAP:
        drawable->drawable.tmap = (Tilemap*)de;
        break;
    
    default:
        break;
    }

    return (int)gf2d_scene.drawable_entities_count-1;
}

void gf2d_scene_remove_from_drawables( void *e )
{
    int i;
    DrawableEntity *d = NULL;
    
    for(i = 0; i < gf2d_scene.drawable_entities_count; i++)
    {
        d = &gf2d_scene.drawable_entities[i];
        if(d->drawable.ent == e)
        {
            gf2d_scene.drawable_entities_count--;
            if( i == (int)gf2d_scene.drawable_entities_count-1 || i == 0 )
            {
                memset( d, 0, sizeof(DrawableEntity) );
                return;
            }

            memmove( d, d+1, sizeof(DrawableEntityType)*(gf2d_scene.drawable_entities_count - i) );
            return;
        }
    }
}