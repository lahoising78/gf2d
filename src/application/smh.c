#include "smh.h"
#include "simple_logger.h"

#include "gf2d_scene.h"
#include "player.h"
#include "punti_jordan.h"

void load_next_level(Entity *self, Entity *other)
{
    gf2d_scene_load_from_file("application/scenes/second_scene.json");
}

void smh_awake()
{
    PhysicsEntity *transition = NULL;
    player_create( gf2d_physics_entity_get_by_name("punti") );

    transition = gf2d_physics_entity_new(NULL);
    transition->modelBox = gf2d_collision_shape(
        vector2d(0.0f, 0.0f),
        vector2d(64.0f, 64.0f),
        CST_BOX
    );
    transition->entity->touch = load_next_level;
    transition->entity->position = vector2d(360.0f, 128.0f);
}