#ifndef _GF2D_FONT_H_
#define _GF2D_FONT_H_

#include "SDL2/SDL_ttf.h"
#include <SDL_ttf.h>
#include "gfc_types.h"

void gf2d_font_start();

void gf2d_font_add(const char *fontPath, int size);

TTF_Font *gf2d_font_default();

#endif