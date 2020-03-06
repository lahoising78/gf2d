#include "simple_logger.h"
#include "game_object.h"
#include "gf2d_projectile.h"
#include "gf2d_scene.h"
#include "sentry.h"

extern float frameTime;

float coolDown = 0.0f;

void sentry_update(Entity *self);
void sentry_touch(Entity *self, Entity *other);

void sentry_shoot(Entity *self);

typedef struct 
{
    /* phys */
    Sprite *sprite;
    CollisionShape shape;
    PhysicsEntityType physType;
    uint8_t canCollide;

    /* projectile */
    Sprite *projSprite;
    float projSpeed;
    float projCooldown;
    float projDistance;
} SentryJson;

static SentryJson config = {0};
static PhysicsEntity *player = NULL;

void sentry_load_config(const char *filename)
{
    SJson *json = NULL;
    SJson *obj = NULL;
    PhysicsEntity *phys = NULL;
    if(!filename) return;

    json = sj_load(filename);

    obj = sj_load( sj_get_string_value( sj_object_get_value(json, "phys") ) );
    phys = gf2d_physics_entity_load(obj);
    sj_free(obj);

    if(!phys) return;
    config.canCollide = phys->canCollide;
    config.physType = phys->type;
    config.shape = phys->modelBox;
    config.sprite = phys->entity->anim->rend->sprite;

    gf2d_physics_entity_free(phys);

    obj = sj_object_get_value(json, "projectile");
    if(obj)
    {
        config.projSprite = gf2d_json_sprite( sj_object_get_value(obj, "sprite") );
        sj_get_float_value( sj_object_get_value(obj, "speed"), &config.projSpeed );
        sj_get_float_value( sj_object_get_value(obj, "coolDown"), &config.projCooldown );
        sj_get_float_value( sj_object_get_value(obj, "distance"), &config.projDistance );
    }

    sj_free(json);
}

PhysicsEntity *sentry_new()
{
    PhysicsEntity *sentry = NULL;

    sentry = gf2d_physics_entity_new(NULL);
    sentry_init(sentry);

    return sentry;
}

void sentry_init(PhysicsEntity *phys)
{
    GameObject *obj = NULL;
    if(!phys || !phys->entity) return;

    phys->entity->update = sentry_update;
    phys->entity->touch = sentry_touch;
    obj = game_object_new();
    if(!obj)
    {
        slog("unable to allocate game obj for sentry");
        gf2d_physics_entity_free(phys);
        return;
    }

    obj->coolDown = config.projCooldown;
    phys->entity->abstraction = obj;
    slog("cool down %.2f", obj->coolDown);

    phys->canCollide = config.canCollide;
    phys->modelBox = config.shape;
    phys->entity->anim->rend->sprite = config.sprite;
    phys->type = config.physType;

    player = gf2d_physics_entity_get_by_name("punti");
}

void sentry_update(Entity *self)
{
    GameObject *obj = NULL;
    if(!self || !self->abstraction) return;

    obj = (GameObject*)self->abstraction;
    
    if(obj->coolDown <= 0.0f)
    {
        sentry_shoot(self);
    }
    else
    {
        obj->coolDown -= frameTime;
    }
    slog("cooldown %.2f", obj->coolDown);
}

void sentry_touch(Entity *self, Entity *other)
{
    if(!self || !other) return;
}

void sentry_shoot(Entity *self)
{
    GameObject *obj = NULL;
    Vector2D dir = {0};
    PhysicsEntity *proj = NULL;

    obj = (GameObject*)self->abstraction;

    if(player)
        vector2d_sub(dir, player->entity->position, self->position);
    proj = gf2d_projectile_create(
        self->position,
        dir,
        config.projSpeed,
        config.projDistance,
        self
    );
    proj->entity->anim->rend->sprite = config.projSprite;
    gf2d_scene_add_to_drawables(proj, DET_PHYS);
    obj->coolDown = config.projCooldown;
}