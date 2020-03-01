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

uint32_t *pj_anim_slash_down();
float pj_anim_slash_down_speed();

uint32_t *pj_anim_slash_up();
float pj_anim_slash_up_speed();

uint32_t *pj_anim_slash_side();
float pj_anim_slash_side_speed();

float *pj_dash();

uint32_t *pj_anim_sword_throw();
float pj_anim_sword_throw_speed();
void pj_spin_sword(Sprite **sprite, uint32_t **anim, float *animSpeed, float *fwd, float *distance);

#endif