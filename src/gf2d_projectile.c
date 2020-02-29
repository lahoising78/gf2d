#include "simple_logger.h"
#include "gfc_list.h"
#include "gf2d_scene.h"
#include "gf2d_projectile.h"

typedef struct
{
    Projectile              *projectiles;
    uint32_t                count;
} ProjectileManager;

static ProjectileManager projectiles = {0};

void gf2d_projectile_manager_close()
{
    if( projectiles.projectiles ) free(projectiles.projectiles);
}

void gf2d_projectile_manager_init(uint32_t count)
{
    projectiles.projectiles = gfc_allocate_array(sizeof(Projectile), count);
    if(!projectiles.projectiles)
    {
        slog("couldn't allocate projectiles");
        return;
    }
    projectiles.count = count;
    atexit( gf2d_projectile_manager_close );
}

Projectile *gf2d_projectile_new()
{
    Projectile *proj = NULL;
    int i;

    for(i = 0; i < projectiles.count; i++)
    {
        proj = &projectiles.projectiles[i];
        if(proj->_inuse) continue;

        proj->_inuse = 1;
        return proj;
    }

    slog("no projectiles available");
    return NULL;
}

PhysicsEntity *gf2d_projectile_create( Vector2D start, Vector2D dir, float speed, float distance, Entity *owner )
{
    PhysicsEntity *phys = NULL;
    Projectile *proj = NULL;

    phys = gf2d_physics_entity_new(NULL);
    if(!phys)
    {
        slog("not enough physics entities for projectiles");
        return NULL;
    }

    vector2d_copy(phys->entity->position, start);
    vector2d_normalize(&dir);
    vector2d_scale(phys->entity->velocity, dir, speed);

    phys->type = PET_KINETIC;

    proj = gf2d_projectile_new();
    if(!proj)
    {
        gf2d_physics_entity_free(phys);
        return NULL;
    }

    vector2d_copy(proj->start, start);
    vector2d_copy(proj->dir, dir);
    proj->speed = speed;

    vector2d_copy(proj->dst, dir);
    vector2d_set_magnitude(&proj->dst, distance);
    vector2d_add(proj->dst, proj->dst, proj->start);
    
    proj->owner = owner;
    proj->self = phys;

    phys->entity->abstraction = proj;
    phys->entity->update = gf2d_projectile_general_update;

    return phys;
}

void gf2d_projectile_general_update( Entity *proj )
{
    Projectile *p = NULL;
    Vector2D distance = {0};
    if(!proj) return;

    p = (Projectile*)proj->abstraction;

    vector2d_sub(distance, p->dst, proj->position);
    if( p->dir.x * distance.x < 0.0f || p->dir.y * distance.y < 0.0f )
    {
        gf2d_projectile_free(proj);
        return;
    }
}

void gf2d_projectile_free( Entity *proj )
{
    Projectile *p = NULL;
    if(!proj) return;

    p = (Projectile*)proj->abstraction;
    gf2d_scene_remove_from_drawables(p->self);
    gf2d_physics_entity_free(p->self);
    memset(p, 0, sizeof(Projectile));
}