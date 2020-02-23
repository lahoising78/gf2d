#ifndef _PUNTI_JORDAN_H_
#define _PUNTI_JORDAN_H_

#include <stdint.h>

void punti_jordan_load(const char *filename);

uint32_t *punti_jordan_get_idle();
float punti_jordan_get_idle_speed();

uint32_t *punti_jordan_get_walking();
float punti_jordan_get_walking_speed();

#endif