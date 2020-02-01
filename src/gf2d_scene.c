#include "gf2d_scene.h"
#include "simple_logger.h"

typedef enum
{
    DET_NONE = 0,
    DET_REND = 1,
    DET_ANIM = 2,
    DET_ENT  = 3,
    DET_PHYS = 4
} DrawableEntityType;

typedef struct
{
    union
    {
        RenderEntity    *rend;
        Animation       *anim;
        Entity          *ent;
        PhysicsEntity   *phys;
    } drawable;
    DrawableEntityType  _type;
    uint8_t             _inuse;
} DrawableEntity;

typedef struct
{
    char                name[GFCLINELEN];
    PhysicsEntity       **physics_entities;
    uint32_t            physics_entities_count;
    Entity              **entities;
    uint32_t            entities_count;
    Animation           **animations;
    uint32_t            animCount;

    DrawableEntity      *drawable_entities;
    uint32_t            drawable_entities_count;
    uint32_t            drawable_entities_size;
} Scene;

static Scene gf2d_scene = {0};

void gf2d_scene_add_to_drawables( void *de, DrawableEntityType type );

void gf2d_scene_init_anims();
void gf2d_scene_swap_in_render_list( uint32_t a, uint32_t b );

void gf2d_scene_load( uint32_t physics_entities_count, uint32_t entities, uint32_t animCount, void (*scene_awake)() )
{
    slog("--== Loading new scene ==--");

    gf2d_scene_close();

    gf2d_physics_entity_manager_clean(0);
    gf2d_entity_manager_clean(0);
    gf2d_animation_manager_clean(0);
    gf2d_physics_entity_manager_initialize_entities();

    gf2d_scene.physics_entities = (PhysicsEntity**)gfc_allocate_array(sizeof(PhysicsEntity*), physics_entities_count);
    if(gf2d_scene.physics_entities) gf2d_scene.physics_entities_count = physics_entities_count;

    gf2d_scene.entities = (Entity**)gfc_allocate_array(sizeof(Entity*), entities);
    if(gf2d_scene.entities) gf2d_scene.entities_count = entities;

    gf2d_scene.animations = (Animation**)gfc_allocate_array(sizeof(Animation*), animCount);
    if(gf2d_scene.animations) gf2d_scene.animCount = animCount;

    gf2d_scene.drawable_entities = (DrawableEntity*)gfc_allocate_array(sizeof(DrawableEntity), entities + animCount + physics_entities_count);
    if( gf2d_scene.drawable_entities ) gf2d_scene.drawable_entities_size = entities + animCount + physics_entities_count;

    if(scene_awake) scene_awake();
}

// void gf2d_scene_init_anims()
// {
//     int i;
//     Animation *anim = NULL;

//     for(i = 0; i < gf2d_scene.animCount; i++)
//     {
//         anim = &gf2d_scene.animations[i];
//         anim->
//     }
// }

void gf2d_scene_close()
{
    if( gf2d_scene.entities ) free(gf2d_scene.entities);
    if( gf2d_scene.physics_entities ) free(gf2d_scene.physics_entities);
    if( gf2d_scene.animations ) free(gf2d_scene.animations);
    if( gf2d_scene.drawable_entities ) free(gf2d_scene.drawable_entities);
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
        case DET_ENT:
            
            if( !ent->drawable.ent->anim ) continue;

            vector2d_add(ent->drawable.ent->anim->rend->position, ent->drawable.ent->anim->rend->position, ent->drawable.ent->position);
                gf2d_animation_render(ent->drawable.ent->anim);
            vector2d_sub(ent->drawable.ent->anim->rend->position, ent->drawable.ent->anim->rend->position, ent->drawable.ent->position);

            break;

        case DET_ANIM:

            if( !ent->drawable.anim->rend ) continue;
            gf2d_animation_render(ent->drawable.anim);

            break;
        
        default:
            break;
        }

    }
}

void gf2d_scene_add_to_drawables( void *de, DrawableEntityType type )
{
    DrawableEntity *drawable = NULL;
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
    
    default:
        break;
    }
}

int gf2d_scene_add_entity( Entity *ent )
{
    int i;
    Entity *e = NULL;

    for( i = 0; i < gf2d_scene.entities_count; i++ )
    {
        e = gf2d_scene.entities[i];
        if(!e) 
        {
            gf2d_scene.entities[i] = ent;
            gf2d_scene_add_to_drawables(ent, DET_ENT);
            return i;
        }
    }

    return -1;
}

void gf2d_scene_remove_entity( Entity *e )
{
    int i;
    Entity *ent = NULL;

    if(!e) return;

    for( i = 0; i < gf2d_scene.entities_count; i++ )
    {
        ent = gf2d_scene.entities[i];
        if(!ent) continue;
        if(ent == e) 
        {
            gf2d_entity_free(e);
            gf2d_scene.entities[i] = NULL;
            break;
        }
    }

    /** TODO: delete the entity from drawables */

    // for( i = 0; i < gfc_list_get_count(gf2d_scene.render_list); i++ )
    // {
    //     if( !gfc_list_get_nth(gf2d_scene.render_list, i) )
    //     {
    //         for( ; !(ent = gfc_list_get_nth(gf2d_scene.render_list, i+1)); i++ )
    //         {
    //             gf2d_scene_swap_in_render_list(i, i+1);
    //         }
    //     }
    // }
}

void gf2d_scene_swap_in_render_list( uint32_t a, uint32_t b )
{
    // void *data;
    // data = gf2d_scene.render_list->elements[a].data;
    // gf2d_scene.render_list->elements[a].data = gf2d_scene.render_list->elements[b].data;
    // gf2d_scene.render_list->elements[b].data = data;
}

int gf2d_scene_add_animation(Animation *anim)
{
    int i;

    for(i = 0; i < gf2d_scene.animCount; i++)
    {
        if(gf2d_scene.animations[i]) continue;

        gf2d_scene.animations[i] = anim;
        gf2d_scene_add_to_drawables(anim, DET_ANIM);

        return i;
    }

    return -1;
}