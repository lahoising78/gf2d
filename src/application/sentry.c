#include "simple_logger.h"
#include "game_object.h"
#include "gf2d_projectile.h"
#include "gf2d_scene.h"
#include "sentry.h"
#include "combat.h"

extern float frameTime;

void sentry_update(Entity *self);
void sentry_touch(Entity *self, Entity *other);

void sentry_shoot(Entity *self);
void sentry_shot_touch(Entity *self, Entity *other);

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
    float projDistance;
    Vector3D burst;
    Vector2D projOffset;
    float projScale;
    CollisionShape projShape;

    float maxHealth;
} SentryJson;

static SentryJson config = {0};
static PhysicsEntity *player = NULL;

typedef enum
{
    SENTRY_IDLE = 0,
    SENTRY_SHOOT
} SentryState;

void sentry_think(Entity *self);

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
        sj_get_float_value( sj_object_get_value(obj, "distance"), &config.projDistance );
        sj_get_float_value( sj_object_get_value(obj, "scale"), &config.projScale );
        config.burst = gf2d_json_vector3d( sj_object_get_value(obj, "burst") );
        if(config.projScale == 0.0f) config.projScale = 1.0f;
        config.projOffset = gf2d_json_vector2d( sj_object_get_value(obj, "offset") );
        config.projShape = gf2d_collision_shape_load( sj_object_get_value(obj, "hitbox") );
    }
    sj_get_float_value( sj_object_get_value(json, "maxHealth"), &config.maxHealth );

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
    phys->entity->think = sentry_think;
    obj = game_object_new();
    if(!obj)
    {
        slog("unable to allocate game obj for sentry");
        gf2d_physics_entity_free(phys);
        return;
    }

    obj->coolDown = config.burst.x;
    obj->selfPhys = phys;
    obj->self = phys->entity;
    obj->awareArea = gf2d_collision_shape(
        vector2d(-236.0f, -236.0f),
        vector2d(600.0f, 600.0f),
        CST_BOX
    );
    obj->health = config.maxHealth;
    phys->entity->abstraction = obj;

    phys->canCollide = config.canCollide;
    phys->modelBox = config.shape;
    phys->entity->anim->rend->sprite = config.sprite;
    phys->type = config.physType;

    player = gf2d_physics_entity_get_by_name("punti");
}

void sentry_think(Entity *self)
{
    GameObject *gobj = NULL;
    if(!self) return;

    gobj = (GameObject*)self->abstraction;
    if(gobj)
    {
        if( game_object_player_in_area(gobj) )
        {
            gobj->state = SENTRY_SHOOT;
        }
        else
        {
            gobj->state = SENTRY_IDLE;
        }
    }
}

void sentry_update(Entity *self)
{
    GameObject *obj = NULL;
    if(!self || !self->abstraction) return;

    obj = (GameObject*)self->abstraction;
    if(!obj) return;
    self->anim->rend->flip.x = player->entity->position.x - self->position.x <= 0.0f;

    if(obj->health <= 0.0f)
        game_object_free(obj);
    
    switch (obj->state)
    {
    case SENTRY_SHOOT:
        if(obj->coolDown <= 0.0f)
        {
            sentry_shoot(self);
        }
        else
        {
            obj->coolDown -= frameTime;
        }
        break;
    
    default:
        break;
    }

    game_object_update(obj);
}

void sentry_touch(Entity *self, Entity *other)
{
    if(!self || !other) return;
}

void sentry_shoot(Entity *self)
{
    GameObject *obj = NULL;
    Vector2D start = {0};
    Vector2D dir = {0};
    PhysicsEntity *proj = NULL;

    obj = (GameObject*)self->abstraction;

    if(player)
        vector2d_sub(dir, player->entity->position, self->position);
    start.y = self->position.y + config.projOffset.y;
    start.x = self->position.x;
    if( self->anim->rend->flip.x )
        start.x -= config.projOffset.x / 2.0f;
    else
        start.x += config.projOffset.x;
    proj = gf2d_projectile_create(
        start,
        dir,
        config.projSpeed,
        config.projDistance,
        self
    );
    proj->entity->anim->rend->sprite = config.projSprite;
    proj->modelBox = config.projShape;
    proj->entity->touch = sentry_shot_touch;
    gf2d_scene_add_to_drawables(proj, DET_PHYS);
    obj->shotsFired++;
    if(obj->shotsFired >= config.burst.z)
    {
        obj->shotsFired = 0.0f;
        obj->coolDown = config.burst.y;
    }
    else
    {
        obj->coolDown = config.burst.x;
    }
}

void sentry_shot_touch(Entity *self, Entity *other)
{
    if(!self || !other) return;

    if(other != player->entity) return;

    combat_do_damage(NULL, other->abstraction, 1.0f, 0.02f);
    gf2d_projectile_free(self);
}