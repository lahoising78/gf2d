#include "gf2d_main.h"
#include "simple_logger.h"

#include "gf2d_scene.h"

#include "smh.h"

void gf2d_main()
{
    slog("MAIN");
    gf2d_scene_submit_awake(smh_awake);
    gf2d_scene_load_from_file("application/sample_scene.json");
}