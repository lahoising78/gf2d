#include "simple_logger.h"
#include "game_object.h"

typedef struct
{
    GameObject *list;
    uint32_t count;
} GameObjectManager;

static GameObjectManager manager = {0};

void game_object_manager_close();

void game_object_manager_init(uint32_t count)
{
    if(manager.list) return;
    
    manager.list = gfc_allocate_array(sizeof(GameObject), count);
    if(!manager.list)
    {
        slog("No space for Game Objects");
        return;
    }
    manager.count = count;

    slog("init game object manager with %u objects", count);

    atexit(game_object_manager_close);
}

void game_object_manager_clean()
{
    slog("cleaning object manager");
    if(manager.list) memset(manager.list, 0, sizeof(GameObject) * manager.count);
}

void game_object_manager_close()
{
    game_object_manager_clean();
    slog("closing object manager");
    if(manager.list) free(manager.list);
}

GameObject *game_object_new()
{
    int i;
    GameObject *obj = NULL;

    for(i = 0; i < manager.count; i++)
    {
        obj = &manager.list[i];
        if(obj->_inuse) continue;

        obj->_inuse = 1;

        return obj;
    }

    return NULL;
}

extern float frameTime;
void game_object_update(GameObject *gobj)
{
    if(!gobj) return;

    gobj->hitstun -= frameTime;
    if(gobj->hitstun < 0.0f) gobj->hitstun = 0.0f;
}

void game_object_free(GameObject *obj)
{
    PhysicsEntity *phys = obj->selfPhys;
    if(!obj) return;

    gf2d_physics_entity_free(phys);
    memset(obj, 0, sizeof(GameObject));
}