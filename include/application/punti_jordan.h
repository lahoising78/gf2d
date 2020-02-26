#ifndef _PUNTI_JORDAN_H_
#define _PUNTI_JORDAN_H_

#include <stdint.h>

void punti_jordan_load(const char *filename);

uint32_t *pj_anim_idle();
float pj_anim_idle_speed();

uint32_t *pj_anim_walking();
float pj_anim_walking_speed();

uint32_t *pj_anim_jumping();
float pj_anim_jumping_speed();

#endif