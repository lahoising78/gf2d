#include "gf2d_scene.h"
#include "simple_logger.h"

static Scene gf2d_scene = {0};

void gf2d_scene_swap_in_render_list( uint32_t a, uint32_t b );

void gf2d_scene_load( uint32_t physics_entities_count, uint32_t entities, void (*scene_awake)() )
{
    slog("--== Loading new scene ==--");

    gf2d_scene_close();

    gf2d_physics_entity_manager_clean(0);
    gf2d_entity_manager_clean(0);
    gf2d_physics_entity_manager_initialize_entities();

    gf2d_scene.physics_entities = (PhysicsEntity**)gfc_allocate_array(sizeof(PhysicsEntity*), physics_entities_count);
    if(gf2d_scene.physics_entities) gf2d_scene.physics_entities_count = physics_entities_count;

    gf2d_scene.entities = (Entity**)gfc_allocate_array(sizeof(Entity*), entities);
    if(gf2d_scene.entities) gf2d_scene.entities_count = entities;

    gf2d_scene.render_list = gfc_list_new_size( entities + physics_entities_count );

    if(scene_awake) scene_awake();
}

void gf2d_scene_close()
{
    if( gf2d_scene.entities ) free(gf2d_scene.entities);
    if( gf2d_scene.physics_entities ) free(gf2d_scene.physics_entities);
    if( gf2d_scene.render_list ) gfc_list_delete(gf2d_scene.render_list);
    memset(&gf2d_scene, 0, sizeof(Scene));
}

void gf2d_scene_render()
{
    int i;
    Entity *ent = NULL;

    for(i = 0; i < gfc_list_get_count(gf2d_scene.render_list); i++)
    {
        ent = (Entity*)gfc_list_get_nth(gf2d_scene.render_list, i);
        if(!ent) continue;
        if( !ent->_inuse || !ent->render_ent ) continue;

        vector2d_add(ent->render_ent->position, ent->render_ent->position, ent->position);
            gf2d_render_ent_draw(ent->render_ent);
        vector2d_sub(ent->render_ent->position, ent->render_ent->position, ent->position);
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
            gfc_list_append(gf2d_scene.render_list, ent);
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

    if( gfc_list_delete_data(gf2d_scene.render_list, e) )
    {
        slog("me pide leche");
    }

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
    void *data;
    data = gf2d_scene.render_list->elements[a].data;
    gf2d_scene.render_list->elements[a].data = gf2d_scene.render_list->elements[b].data;
    gf2d_scene.render_list->elements[b].data = data;
}