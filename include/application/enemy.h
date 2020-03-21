#ifndef _ENEMY_h_
#define _ENEMY_h_

#define ENEMY_H_SETUP(enemy) void enemy##_config(const char *filepath); \
void enemy##_init(PhysicsEntity *self);

/** 
 * @param sprite is the sprite of the enemy
 * @param modelBox is the collision shape that will be used for physics
 * @param hitbox will be the hitbox used to attack
 * @param awareArea a certain area around the enemy to check for the player
 * @param damage how much damage to deal with their attacks
 * @param hitstun the hitstun to apply to the target when damage is dealt
 * @param maxHealth max possible health for the enemy
 * @param cooldown how much time enemy has to wait to become active again
 * @param anticipation how much time to wait before attacking, so player has time to react
 */
#define ENEMY_COMMON Sprite *sprite; \
CollisionShape modelBox; \
CollisionShape hitbox; \
CollisionShape awareArea; \
float maxHealth; \
float damage; \
float hitstun; \
float cooldown; \
float anticipation; \
PhysicsEntity *player;

#define ENEMY_CONFIG_BEGIN(config, filepath)\
SJson *json = NULL; \
if(!filepath) return; \
json = sj_load(filepath); \
if(!json) return; \
config.sprite = gf2d_json_sprite( sj_object_get_value(json, "sprite") ); \
config.modelBox = gf2d_collision_shape_load( sj_object_get_value(json, "modelBox") ); \
config.hitbox = gf2d_collision_shape_load( sj_object_get_value(json, "hitbox") ); \
config.awareArea = gf2d_collision_shape_load( sj_object_get_value(json, "awareArea") ); \
sj_get_float_value( sj_object_get_value(json, "damage"), &config.damage ); \
sj_get_float_value( sj_object_get_value(json, "hitstun"), &config.hitstun ); \
sj_get_float_value( sj_object_get_value(json, "maxHealth"), &config.maxHealth ); \
sj_get_float_value( sj_object_get_value(json, "cooldown"), &config.cooldown ); \
sj_get_float_value( sj_object_get_value(json, "anticipation"), &config.anticipation ); \
config.player = gf2d_physics_entity_get_by_name("punti");

#define ENEMY_INIT_BEGIN(config, phys) GameObject *gobj = NULL; \
if(!phys) return; \
phys->modelBox = config.modelBox; \
phys->entity->anim->rend->sprite = config.sprite; \
phys->canCollide = 1; \
phys->useGravity = 1; \
phys->type = PET_KINETIC; \
gobj = game_object_new(); \
if(gobj) { \
    gobj->health = config.maxHealth; \
    gobj->awareArea = config.awareArea; \
    gobj->self = phys->entity; \
    gobj->selfPhys = phys; \
    gobj->hitbox = config.hitbox; \
    phys->entity->abstraction = gobj; \
}

#endif