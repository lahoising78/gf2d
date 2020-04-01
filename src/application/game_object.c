#include "simple_logger.h"
#include "game_object.h"

typedef struct
{
    GameObject *list;
    uint32_t count;
} GameObjectManager;

static GameObjectManager manager = {0};
PhysicsEntity *playerGobj = NULL;

void game_object_manager_close();
uint8_t game_object_collision(GameObject *self, GameObject **o);

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

void game_object_manager_save(SJson *dst)
{
    SJson *json = NULL;
    GameObject *obj = NULL;
    int i;
    if(!dst);

    for(i = 0; i < manager.count; i++)
    {
        obj = &manager.list[i];
        if(!obj->selfPhys) continue;
        if(obj->selfPhys->name[0] == 0) continue;

        json = gf2d_physics_entity_save(obj->selfPhys);
        game_object_save(obj, json);
        sj_array_append(dst, json);
    }
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
    GameObject *other = NULL;
    if(!gobj) return;

    gobj->hitstun -= frameTime;
    if(gobj->hitstun < 0.0f) gobj->hitstun = 0.0f;

    if(gobj->selfPhys && gobj->damage)
    {
        vector2d_add(gobj->hitbox.position, gobj->hitbox.position, gobj->selfPhys->entity->position);
        // slog("gobj update %.2f %.2f", gobj->hitbox.position.x, gobj->hitbox.position.y);
        while( game_object_collision(gobj, &other) )
        {
            gobj->damage(gobj, other);
        }
        if(!gobj->_inuse || !gobj->selfPhys) return;
        vector2d_sub(gobj->hitbox.position, gobj->hitbox.position, gobj->selfPhys->entity->position);
    }
}

uint8_t game_object_player_in_area(GameObject *self)
{
    uint8_t res = 0;
    if(!self || !self->self) return 0;
    if(!playerGobj)
    {
        slog("there is no player gobj");
        return 0;
    }

    vector2d_add(self->awareArea.position, self->awareArea.position, self->self->position);
    vector2d_add(playerGobj->modelBox.position, playerGobj->modelBox.position, playerGobj->entity->position);
    res = gf2d_collision_check(&self->awareArea, &playerGobj->modelBox, NULL);
    vector2d_sub(self->awareArea.position, self->awareArea.position, self->self->position);
    vector2d_sub(playerGobj->modelBox.position, playerGobj->modelBox.position, playerGobj->entity->position);

    return res;
}

void game_object_free(GameObject *obj)
{
    PhysicsEntity *phys = obj->selfPhys;
    if(!obj) return;

    gf2d_physics_entity_free(phys);
    memset(obj, 0, sizeof(GameObject));
}

uint8_t game_object_collision(GameObject *self, GameObject **o)
{
    GameObject *other = NULL;
    GameObject *end = &manager.list[ manager.count - 1 ];
    Vector2D zero = {0};
    if(!self || !o) return 0;
    other = *o;

    if(other == NULL) other = &manager.list[0];
    else other++;

    if( vector2d_equal(zero, self->hitbox.dimensions.wh) ) return 0;

    for(; other <= end; other++)
    {
        if(!other->_inuse || !other->selfPhys) continue;
        if(other == self) continue;

        vector2d_add(other->selfPhys->modelBox.position, other->selfPhys->modelBox.position, other->selfPhys->entity->position);
        // slog("other %.2f %.2f", other->selfPhys->modelBox.position.x, other->selfPhys->modelBox.position.y);
        if( gf2d_collision_check(&self->hitbox, &other->selfPhys->modelBox, NULL) )
        {
            vector2d_sub(other->selfPhys->modelBox.position, other->selfPhys->modelBox.position, other->selfPhys->entity->position);
            *o = other;
            return 1;
        }
        vector2d_sub(other->selfPhys->modelBox.position, other->selfPhys->modelBox.position, other->selfPhys->entity->position);
    }

    return 0;
}

void game_object_save(GameObject *obj, SJson *dst)
{
    if(!obj || !dst) return;

    sj_object_insert(dst, "health", sj_new_float(obj->health));
    sj_object_insert(dst, "maxHealth", sj_new_float(obj->maxHealth));
}