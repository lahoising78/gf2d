#include "simple_logger.h"
#include "game_object.h"
#include "gf2d_scene.h"
#include "gf2d_input.h"
#include "scene_door.h"

typedef struct
{
    HAZARD_COMMON
} SceneDoorConfig;

static SceneDoorConfig config = {0};

void scene_door_touch(Entity *self, Entity *other);

void scene_door_config(const char *filepath)
{
    HAZARD_CONFIG_BEGIN("scene door")

    gf2d_physics_entity_free(phys);
    sj_free(json);
}

HAZARD_NEW(scene_door)

void scene_door_init(PhysicsEntity *self)
{
    HAZARD_INIT_BEGIN(self)

    self->entity->touch = scene_door_touch;
}

void scene_door_touch(Entity *self, Entity *other)
{
    GameObject *gobj = NULL;
    HAZARD_TOUCH_CHECK(self, other)
    if(config.player->entity != other) return;

    gobj = (GameObject*)self->abstraction;
    if(gobj && gf2d_input_key_just_pressed(SDL_SCANCODE_UP))
    {
        gf2d_scene_load_from_file( gobj->buf );
    }

}