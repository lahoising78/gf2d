#ifndef _HAZARDS_DEFINES_H_
#define _HAZARDS_DEFINES_H_

#define HAZARD_H_SETUP(trap)    \
void trap##_config(const char *filepath); \
PhysicsEntity *trap##_new();\
void trap##_init(PhysicsEntity *self);

#define HAZARD_COMMON \
Sprite *sprite;\
uint8_t canCollide;\
uint8_t useGravity;\
CollisionShape shape;\
PhysicsEntity *player;

#define HAZARD_CONFIG_BEGIN(message) PhysicsEntity *phys = NULL; \
SJson *json = NULL; \
if(!filepath) return; \
slog("%s config", message); \
json = sj_load(filepath); \
if(!json) { slog("unable to find file %s", filepath); return; } \
phys = gf2d_physics_entity_load( sj_object_get_value(json, "phys") ); \
if(!phys) { slog("there are no phys ents to load %s config", message); sj_free(json); return;} \
config.canCollide = phys->canCollide; \
config.shape = phys->modelBox; \
config.sprite = phys->entity->anim->rend->sprite; \
config.useGravity = phys->useGravity; \
config.player = gf2d_physics_entity_get_by_name("punti");

#define HAZARD_NEW(type) PhysicsEntity *type##_new() { \
    PhysicsEntity *phys = NULL; \
    phys = gf2d_physics_entity_new(NULL); \
    type##_init(phys); \
    return phys; \
}

#define HAZARD_INIT_BEGIN(self) GameObject *gobj = NULL; \
if(!self) return; \
self->canCollide = config.canCollide; \
self->modelBox = config.shape; \
self->entity->anim->rend->sprite = config.sprite; \
self->useGravity = config.useGravity; \
gobj = game_object_new(); \
if(gobj) { \
    gobj->self = self->entity; \
    gobj->selfPhys = self; \
    self->entity->abstraction = gobj; \
}

#define HAZARD_TOUCH_CHECK(self, other) if(!config.player) return; \
if(!self || !other) return;

#endif