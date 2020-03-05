#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "gf2d_physics_entity.h"

typedef struct
{
    float health;
    float coolDown;
    int state;
    uint8_t _inuse;
} GameObject;

void game_object_manager_init(uint32_t count);
GameObject *game_object_new();

#endif