#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "gf2d_physics_entity.h"
#include <inttypes.h>

typedef struct game_object_s
{
    float health;
    float maxHealth;
    float coolDown;
    int state;
    float shotsFired;
    float hitstun;
    uint8_t _inuse;

    char buf[GFCLINELEN];

    PhysicsEntity *selfPhys;
    Entity *self;

    void (*damage)(struct game_object_s *self, struct game_object_s *other);
    CollisionShape hitbox;
    Entity *target;
    uint8_t isProtected;

    CollisionShape awareArea;
} GameObject;

void game_object_manager_init(uint32_t count);
void game_object_manager_clean();
void game_object_manager_save(SJson *dst);

GameObject *game_object_new();
void game_object_update(GameObject *gobj);
uint8_t game_object_player_in_area(GameObject *self);
void game_object_free(GameObject *obj);
void game_object_save(GameObject *obj, SJson *dst);

#endif