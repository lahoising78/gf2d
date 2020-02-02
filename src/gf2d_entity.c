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

void gf2d_entity_manager_clean( uint8_t del )
{
    int i;
    Entity *ent = NULL;

    slog("clean all entities");

    for(i = 0; i < gf2d_entity_manager.count; i++)
    {
        ent = &gf2d_entity_manager.entity_list[i];
        gf2d_entity_free(ent);
    }
}

void gf2d_entity_manager_initialize_all_entities()
{
    int i;
    Entity *ent = NULL;

    for(i = 0; i < gf2d_entity_manager.count; i++)
    {
        ent = &gf2d_entity_manager.entity_list[i];
        ent->anim = gf2d_animation_new();
        // ent->acceleration = vector2d( 10, 10 );
    }
}

void gf2d_entity_manager_close()
{
    slog("close entity manager");

    gf2d_entity_manager_clean(1);
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
        if(!ent->_inuse || !ent->anim) continue;

        vector2d_add(ent->anim->rend->position, ent->anim->rend->position, ent->position);
        // slog("pos %.2f %.2f", ent->render_ent->position.x, ent->render_ent->position.y);
            gf2d_animation_render(ent->anim);
        vector2d_sub(ent->anim->rend->position, ent->anim->rend->position, ent->position);
    }
}

void gf2d_entity_init(Entity *ent)
{
    if(!ent) return;

    ent->_inuse = 1;
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

        gf2d_entity_init(ent);

        return ent;
    }

    slog("No more entities available");
    return NULL;
}

void gf2d_entity_free(Entity *ent)
{
    Animation *anim = NULL;

    // slog("free entity");
    anim = ent->anim;
    if(anim)
    {
        gf2d_animation_free(anim, 0);
    }

    memset(ent, 0, sizeof(Entity));

    ent->anim = anim;
    gf2d_animation_init(anim);
}