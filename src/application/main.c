#include "gf2d_main.h"
#include "simple_logger.h"

#include "gf2d_scene.h"

#include "smh.h"

void gf2d_config()
{
    slog("CONFIG");

    gf2d_main_set_window_name( "Punti Jordan - El Sayayin de la Patria" );
}

void gf2d_main()
{
    slog("MAIN");
    gf2d_scene_load(8, 8, 8, smh_awake );
}