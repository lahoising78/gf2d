#include "gf2d_main.h"
#include "simple_logger.h"

#include "gf2d_scene.h"

#include "game_object.h"
#include "punti_jordan.h"
#include "sentry.h"
#include "smh.h"
#include "two.h"

void gf2d_main()
{
    slog("MAIN");

    game_object_manager_init(1024);
    gf2d_scene_submit_awake(smh_awake);
    gf2d_scene_submit_awake(two_awake);
    punti_jordan_load("application/punti_jordan_anim_config.json");
    sentry_load_config("application/sentry_config.json");
    gf2d_scene_load_from_file("application/scenes/first_scene.json");
}