#include "gf2d_entity.h"
#include "simple_logger.h"

typedef struct
{
    uint32_t count;
    Entity *entity_list;
} EntityManager;

static EntityManager gf2d_entity_manager = {0};

void gf2d_entity_manager_initialize_all_entities();
void gf2d_entity_manager_close();

void gf2d_entity_manager_init(uint32_t count)
{
    slog("initializing entity manager with %d entities", (int)count);

    if(gf2d_entity_manager.entity_list)
    {
        slog("the entity manager has already been initiated");
        return;
    }

    gf2d_entity_manager.entity_list = (Entity*)gfc_allocate_array(sizeof(Entity), count);
    if(!gf2d_entity_manager.entity_list)
    {
        slog("not enough space to allocate entities");
        return;
    }
    memset(gf2d_entity_manager.entity_list, 0, sizeof(Entity) * count);

    gf2d_entity_manager.count = count;

    gf2d_entity_manager_initialize_all_entities();

    atexit(gf2d_entity_manager_close);
}

void gf2d_entity_manager_initialize_all_entities()
{
    int i;
    Entity *ent = NULL;

    for(i = 0; i < gf2d_entity_manager.count; i++)
    {
        ent = &gf2d_entity_manager.entity_list[i];
        ent->render_ent = gf2d_render_ent_new(NULL);
    }
}

void gf2d_entity_manager_close()
{
    int i;
    Entity *ent = NULL;

    slog("close entity manager");

    for(i = 0; i < gf2d_entity_manager.count; i++)
    {
        ent = &gf2d_entity_manager.entity_list[i];
        gf2d_entity_free(ent);
        if(ent->render_ent) free(ent->render_ent);
    }

    free(gf2d_entity_manager.entity_list);
}

void gf2d_entity_manager_update()
{
    int i;
    Entity *ent = NULL;

    for(i = 0; i < gf2d_entity_manager.count; i++)
    {
        ent = &gf2d_entity_manager.entity_list[i];
        if(!ent->_inuse) continue;

        if(ent->think) ent->think(ent);
        if(ent->update) ent->update(ent);
    }
}

void gf2d_entity_manager_draw()
{
    int i;
    Entity *ent = NULL;

    for(i = 0; i < gf2d_entity_manager.count; i++)
    {
        ent = &gf2d_entity_manager.entity_list[i];
        if(!ent->_inuse || !ent->render_ent) continue;

        vector2d_add(ent->render_ent->position, ent->render_ent->position, ent->position);
            gf2d_render_ent_draw(ent->render_ent);
        vector2d_sub(ent->render_ent->position, ent->render_ent->position, ent->position);
    }
}

Entity *gf2d_entity_new()
{
    int i;
    Entity *ent = NULL;

    // slog("get new entity");

    for(i = 0; i < gf2d_entity_manager.count; i++)
    {
        ent = &gf2d_entity_manager.entity_list[i];
        if(ent->_inuse) continue;

        ent->_inuse = 1;

        return ent;
    }

    slog("No more entities available");
    return NULL;
}

void gf2d_entity_free(Entity *ent)
{
    RenderEntity *rend_ent = NULL;

    // slog("free entity");
    rend_ent = ent->render_ent;
    if(rend_ent)
    {
        gf2d_render_ent_free(rend_ent);
    }

    memset(ent, 0, sizeof(Entity));

    ent->render_ent = rend_ent;
}