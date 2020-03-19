#include "simple_logger.h"
#include "game_object.h"
#include "toxic_bomb.h"
#include "combat.h"

typedef struct
{
    HAZARD_COMMON

    float alpha;
    Vector3D damage;            //cooldown, damage rate, damage over time
} ToxicBombConfig;

static ToxicBombConfig config = {0};

void toxic_bomb_touch(Entity *self, Entity *other);
void toxic_bomb_update(Entity *self);

void toxic_bomb_config(const char *filepath)
{
    HAZARD_CONFIG_BEGIN("toxic bomb")

    sj_get_float_value( sj_object_get_value(json, "alpha"), &config.alpha );
    config.damage = gf2d_json_vector3d( sj_object_get_value(json, "damage") );

    gf2d_physics_entity_free(phys);
    sj_free(json);
}

HAZARD_NEW(toxic_bomb)

void toxic_bomb_init(PhysicsEntity *self)
{
    HAZARD_INIT_BEGIN(self)

    self->entity->touch = toxic_bomb_touch;
    self->entity->update = toxic_bomb_update;
    self->entity->anim->rend->colorShift.w = config.alpha;

    if(gobj)
    {
        gobj->coolDown = config.damage.x;
    }
}

extern float frameTime;
void toxic_bomb_update(Entity *self)
{
    GameObject *gobj = self->abstraction;

    if(!gobj) return;

    if(gobj->target)
    {
        self->position = gobj->target->position;
    }
}

void toxic_bomb_touch(Entity *self, Entity *other)
{
    GameObject *gobj = NULL;
    HAZARD_TOUCH_CHECK(self, other)
    if(other != config.player->entity) return;

    vector2d_copy(self->position, other->position);

    gobj = (GameObject*)self->abstraction;
    if(gobj)
    {
        gobj->target = other;

        if(gobj->coolDown <= 0.0f)
        {
            game_object_free(gobj);
            return;
        }

        gobj->coolDown -= frameTime;

        if(gobj->shotsFired <= 0.0f)
        {
            slog("toxic bomb damage");
            gobj->shotsFired = config.damage.y;
            combat_do_damage(gobj, other->abstraction, config.damage.z * config.damage.y, 0.0f);
        }
        else
        {
            gobj->shotsFired -= frameTime;              /* use shotsFired for damage rate */
        }
    }
}