#include "game_object.h"
#include "land_mine.h"
#include "simple_logger.h"

typedef struct
{
    Sprite                      *sprite;
    CollisionShape              shape;
    PhysicsEntityType           physType;
    uint8_t                     canCollide;
} LandMineConfig;

static LandMineConfig config = {0};

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
    if(!phys) return;
    slog("init land mine");

    phys->canCollide = config.canCollide;
    phys->entity->anim->rend->sprite = config.sprite;
    phys->modelBox = config.shape;
    phys->type = config.physType;
}
