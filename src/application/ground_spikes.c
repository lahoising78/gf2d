#include "simple_logger.h"
#include "game_object.h"
#include "ground_spikes.h"
#include "combat.h"

typedef struct
{
    Sprite          *sprite;
    uint8_t         canCollide;
    uint8_t         useGravity;
    CollisionShape  shape;

    /* dmg */
    Vector3D        damage;         /* damage, cooldown, knockback */

    PhysicsEntity *player;
} GroundSpikesConfig;

static GroundSpikesConfig config = {0};

void ground_spikes_update(Entity *self);
void ground_spikes_touch(GameObject *self, GameObject *other);

void ground_spikes_config(const char *filepath)
{
    SJson *json = NULL;
    PhysicsEntity *phys = NULL;

    json = sj_load(filepath);
    if(!json)
    {
        slog("unable to find file %s", filepath);
        return;
    }

    phys = gf2d_physics_entity_load( sj_object_get_value(json, "phys") );
    if(!phys)
    {
        slog("couldn't get an available phys entity to load ground spikes config");
        return;
    }

    config.sprite = phys->entity->anim->rend->sprite;
    config.canCollide = phys->canCollide;
    config.useGravity = phys->useGravity;
    config.shape = phys->modelBox;
    slog("%u %u %u %.2f", config.sprite, config.canCollide, config.useGravity, config.shape.position.x);
    
    config.player = gf2d_physics_entity_get_by_name("punti");

    config.damage = gf2d_json_vector3d( sj_object_get_value(json, "damage") );

    sj_free(json);
}

PhysicsEntity *ground_spikes_new()
{
    PhysicsEntity *phys = NULL;

    phys = gf2d_physics_entity_new(NULL);
    ground_spikes_init(phys);

    return phys;
}

void ground_spikes_init(PhysicsEntity *self)
{
    GameObject *gobj = NULL;

    if(!self) return;
    slog("init ground spike");

    self->entity->anim->rend->sprite = config.sprite;
    self->canCollide = config.canCollide;
    self->modelBox = config.shape;
    self->useGravity = config.useGravity;

    gobj = game_object_new();
    if(gobj)
    {
        gobj->selfPhys = self;
        gobj->self = self->entity;
        gobj->damage = ground_spikes_touch;
        gobj->hitbox = config.shape;
        self->entity->abstraction = gobj;
    }

    self->entity->update = ground_spikes_update;
}

extern float frameTime;
void ground_spikes_update(Entity *self)
{
    GameObject *gobj = NULL;
    if(!self) return;

    gobj = (GameObject*)self->abstraction;
    if(!gobj) return;

    gobj->coolDown -= frameTime;
    if(gobj->coolDown < 0.0f) gobj->coolDown = 0.0f;

    game_object_update(gobj);
}

void ground_spikes_touch(GameObject *self, GameObject *other)
{
    Vector2D dir = {0};
    if(!self || !other) return;
    if(!config.player) return;
    if(other != config.player->entity->abstraction) return;

    if(self->coolDown > 0.0f) return;
    self->coolDown = config.damage.y;

    vector2d_scale(dir, self->hitbox.dimensions.wh, 0.5f);
    vector2d_add(dir, dir, self->self->position);
    vector2d_sub(dir, other->self->position, dir);

    combat_do_damage(self, other, config.damage.x);
    combat_knockback(self, other, dir, config.damage.z);
}