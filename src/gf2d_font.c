#include "simple_logger.h"
#include "gfc_list.h"
#include "gf2d_font.h"

// typedef struct
// {
//     List *fonts;
// } FontsManager

static List *gf2d_fonts = NULL;

void gf2d_font_free()
{
    TTF_Font *font = NULL;
    int i;

    for(i = 0; i < gfc_list_get_count(gf2d_fonts); i++)
    {
        font = gfc_list_get_nth(gf2d_fonts, i);
        TTF_CloseFont(font);
    }

    gfc_list_delete(gf2d_fonts);
}

void gf2d_font_start()
{
    gf2d_fonts = gfc_list_new_size(8);
    atexit( gf2d_font_free );
}

void gf2d_font_add(const char *fontPath, int size)
{
    TTF_Font *font = NULL;

    slog("opening font %s", fontPath);
    font = TTF_OpenFont(fontPath, size);
    gfc_list_append(gf2d_fonts, font);
}

TTF_Font *gf2d_font_default()
{
    return (TTF_Font*)gfc_list_get_nth(gf2d_fonts, 0);
}