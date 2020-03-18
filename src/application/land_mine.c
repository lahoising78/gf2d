#include "simple_logger.h"
#include "game_object.h"
#include "gf2d_scene.h"
#include "combat.h"
#include "land_mine.h"

typedef struct
{
    Sprite                      *sprite;
    CollisionShape              shape;
    PhysicsEntityType           physType;
    uint8_t                     canCollide;
    uint8_t                     useGravity;

    PhysicsEntity               *player;

    float damage;
} LandMineConfig;

static LandMineConfig config = {0};

void land_mine_touch(GameObject *self, GameObject *other);
void land_mine_update(Entity *self);

void land_mine_config(const char *filepath)
{
    SJson *json = NULL;
    PhysicsEntity *phys = NULL;

    json = sj_load(filepath);

    phys = gf2d_physics_entity_load( sj_object_get_value(json, "phys") );
    if(!phys)
    {
        sj_free(json);
        return;
    }

    config.sprite = phys->entity->anim->rend->sprite;
    config.canCollide = phys->canCollide;
    config.physType = phys->type;
    config.shape = phys->modelBox;
    config.useGravity = phys->useGravity;

    config.player = gf2d_physics_entity_get_by_name("punti");
    
    sj_get_float_value( sj_object_get_value(json, "damage"), &config.damage );

    sj_free(json);
}

PhysicsEntity *land_mine_new()
{
    PhysicsEntity *phys = NULL;
    phys = gf2d_physics_entity_new(NULL);
    land_mine_init(phys);
    return phys;
}

void land_mine_init(PhysicsEntity *phys)
{
    GameObject *obj;

    if(!phys) return;
    slog("init land mine");

    phys->canCollide = config.canCollide;
    phys->entity->anim->rend->sprite = config.sprite;
    phys->modelBox = config.shape;
    phys->type = config.physType;
    phys->useGravity = config.useGravity;

    obj = game_object_new();
    if(obj)
    {
        obj->self = phys->entity;
        obj->selfPhys = phys;
        obj->damage = land_mine_touch;
        obj->hitbox = config.shape;
        phys->entity->abstraction = obj;
        phys->entity->update = land_mine_update;
        slog("land mine gobj");
    }
}

void land_mine_update(Entity *self)
{
    if(!self) return;

    game_object_update((GameObject*)self->abstraction);
}

void land_mine_touch(GameObject *self, GameObject *other)
{
    slog("land mine touch");
    if(!self || !other) return;
    if(!config.player) return;
    if(other != config.player->entity->abstraction) return;

    combat_do_damage(self, other, config.damage);

    gf2d_scene_remove_from_drawables(self->selfPhys);
    game_object_free( self );
}