#include "gf2d_physics_entity.h"
#include "simple_logger.h"

#include "gf2d_tilemap.h"

#define GRAVITY                     98.0f

#define TIME_MULTIPLIER             1000.0f
extern float frameTime;

typedef struct
{
    PhysicsEntity           *entity_list;
    uint32_t                count;
} PhysicsEntityManager;

static PhysicsEntityManager gf2d_physics_entity_manager = {0};

// void gf2d_physics_entity_manager_initialize_entities();
void gf2d_physics_entity_manager_close();

void gf2d_physics_entity_think( struct physics_entity_s *ent );
void gf2d_physics_entity_update( struct physics_entity_s *ent );
void gf2d_physics_entity_touch( struct physics_entity_s *ent, struct physics_entity_s *other );

PhysicsEntity *gf2d_physics_entity_check_collision( PhysicsEntity *e, CollisionInfo *info );
uint8_t gf2d_physics_entity_check_tilemap_collision(PhysicsEntity *e, CollisionInfo *info);
void gf2d_physics_entity_handle_collision( PhysicsEntity *e, PhysicsEntity *o, CollisionInfo info );
void gf2d_physics_entity_collision_resolution(PhysicsEntity *e, CollisionInfo info);

void gf2d_physics_entity_manager_init(uint32_t count)
{
    slog("initializing physics entity manager with %d entities", (int)count);
    gf2d_physics_entity_manager.entity_list = (PhysicsEntity*)gfc_allocate_array(sizeof(PhysicsEntity), count);
    if(!gf2d_physics_entity_manager.entity_list)
    {
        slog("unable to allocate enough physics entities");
        return;
    }
    memset(gf2d_physics_entity_manager.entity_list, 0, sizeof(PhysicsEntity) * count);
    gf2d_physics_entity_manager.count = count;

    gf2d_physics_entity_manager_initialize_entities();

    atexit(gf2d_physics_entity_manager_close);
}

void gf2d_physics_entity_manager_initialize_entities()
{
    int i;
    PhysicsEntity *ent = NULL;

    for(i = 0; i < gf2d_physics_entity_manager.count; i++)
    {
        ent = &gf2d_physics_entity_manager.entity_list[i];
        ent->entity = gf2d_entity_new();
    }
}

void gf2d_physics_entity_manager_clean( uint32_t freeEntity )
{
    int i;
    PhysicsEntity *ent = NULL;

    if(freeEntity)
    {
        memset(gf2d_physics_entity_manager.entity_list, 0, gf2d_physics_entity_manager.count*sizeof(PhysicsEntity));
        return;
    }

    for(i = 0; i < gf2d_physics_entity_manager.count; i++)
    {
        ent = &gf2d_physics_entity_manager.entity_list[i];
        gf2d_physics_entity_free(ent);
    }
}

void gf2d_physics_entity_manager_close()
{
    slog("close physics entity manager");

    gf2d_physics_entity_manager_clean(1);
    free(gf2d_physics_entity_manager.entity_list);
}

void gf2d_physics_entity_manager_update()
{
    int i;
    PhysicsEntity *ent = NULL;

    for(i = 0; i < gf2d_physics_entity_manager.count; i++)
    {
        ent = &gf2d_physics_entity_manager.entity_list[i];
        if(!ent->_inuse) continue;
        gf2d_physics_entity_think(ent);
        gf2d_physics_entity_update(ent);
    }
}

PhysicsEntity *gf2d_physics_entity_new()
{
    int i;
    PhysicsEntity *ent = NULL;

    for(i = 0; i < gf2d_physics_entity_manager.count; i++)
    {
        ent = &gf2d_physics_entity_manager.entity_list[i];
        if(ent->_inuse) continue;
        ent->_inuse = 1;
        return ent;
    }

    return NULL;
}

void gf2d_physics_entity_free( PhysicsEntity *ent )
{
    Entity *e = NULL;

    if(!ent) return;

    e = ent->entity;
    gf2d_entity_free(e);
    memset(ent, 0, sizeof(PhysicsEntity));
    ent->entity = e;    
    gf2d_entity_init(e);
}

void gf2d_physics_entity_think( struct physics_entity_s *ent )
{
    if(!ent) return;


}

/** @note we multiply time by TIME_MULTIPLIER to make numbers work better */
void gf2d_physics_entity_update( struct physics_entity_s *ent )
{
    Vector2D buf = {0};
    PhysicsEntity *o = NULL;
    CollisionInfo info = {0};

    if(!ent) return;
    
    if( ent->type == PET_KINETIC )
    {
        
        if( ent->useGravity )
        {
            ent->entity->velocity.y += GRAVITY * frameTime;
        }

        /* df = di + vt */
        vector2d_scale( buf, ent->entity->velocity, frameTime * TIME_MULTIPLIER );                                      // vt
        vector2d_add( ent->entity->position, ent->entity->position, buf );                                              // df = di + vt
        
        if( (o = gf2d_physics_entity_check_collision(ent, &info)) )
        {
            if( ent->canCollide && o->canCollide )
            {
                gf2d_physics_entity_handle_collision(ent, o, info);
            }

            if( ent->entity->touch )    ent->entity->touch( ent->entity, o->entity );
            if( o->entity->touch )      o->entity->touch( o->entity, ent->entity );
        }

        if( gf2d_physics_entity_check_tilemap_collision(ent, &info) )
        {
            if( ent->canCollide )
            {
                slog("colliding with tile");
                gf2d_physics_entity_collision_resolution(ent, info);
            }
        }
    }
    else
    {
        vector2d_clear(ent->entity->acceleration);
        vector2d_clear(ent->entity->velocity);
    }

    // slog( "position: %.2f, %.2f", ent->entity->position.x, ent->entity->position.y );
    // slog( "velocity: %.2f, %.2f", ent->entity->velocity.x, ent->entity->velocity.y );
    // slog( "acceleration: %.2f, %.2f", ent->entity->acceleration.x, ent->entity->acceleration.y );
}

void gf2d_physics_entity_touch( struct physics_entity_s *ent, struct physics_entity_s *other )
{
    if(!ent || !other) return;

    
}

PhysicsEntity *gf2d_physics_entity_check_collision( PhysicsEntity *e, CollisionInfo *info )
{
    PhysicsEntity *o = NULL;

    if(!e) return NULL;

    vector2d_add(e->modelBox.position, e->modelBox.position, e->entity->position);
    for( o = e + 1; o <= &gf2d_physics_entity_manager.entity_list[ gf2d_physics_entity_manager.count - 1 ]; o++)
    {
        // o = &gf2d_physics_entity_manager.entity_list[i];
        if(!o->_inuse || o == e) continue;

        vector2d_add(o->modelBox.position, o->modelBox.position, o->entity->position);
        if( gf2d_collision_check(&e->modelBox, &o->modelBox, info) )
        {   
            vector2d_sub(e->modelBox.position, e->modelBox.position, e->entity->position);
            vector2d_sub(o->modelBox.position, o->modelBox.position, o->entity->position);
            return o;
        }
        vector2d_sub(o->modelBox.position, o->modelBox.position, o->entity->position);
    }
    vector2d_sub(e->modelBox.position, e->modelBox.position, e->entity->position);

    return NULL;
}

uint8_t gf2d_physics_entity_check_tilemap_collision(PhysicsEntity *e, CollisionInfo *info)
{
    int i, x, y;
    const uint32_t count = gf2d_tilemap_get_count();
    const Tilemap *tilemaps = gf2d_tilemap_get_all();
    CollisionShape shape = {0};
    Vector2D worldToMap = {0};
    Vector2D entMax = {0};
    shape.shapeType = CST_BOX;

    if(!e || !tilemaps) return 0;

    vector2d_add(e->modelBox.position, e->modelBox.position, e->entity->position);
    for(i = 0; i < count; i++)
    {
        vector2d_add(entMax, e->modelBox.position, e->modelBox.dimensions.wh);
        entMax = gf2d_tilemap_world_to_map(&tilemaps[i], entMax);
        worldToMap = gf2d_tilemap_world_to_map(&tilemaps[i], e->modelBox.position);
        if(worldToMap.x < 0) continue;
        
        shape.dimensions.wh.x = tilemaps[i].spriteSheet->frame_w;
        shape.dimensions.wh.y = tilemaps[i].spriteSheet->frame_h;

        if( entMax.x < 0 ) entMax.x = (float)tilemaps[i].w - 1.0f;
        if( entMax.y < 0 ) entMax.y = (float)tilemaps[i].h - 1.0f;

        for(y = (int)worldToMap.y; y <= entMax.y; y++)
        {
            for(x = (int)worldToMap.x; x <= entMax.x; x++)
            {
                if( !tilemaps[i].tiles[y * tilemaps[i].w + x].solid ) continue;

                shape.position.x = x;
                shape.position.y = y;
                shape.position = gf2d_tilemap_map_to_world(&tilemaps[i], shape.position);

                if( gf2d_collision_check(&shape, &e->modelBox, info) ) 
                {
                    vector2d_sub(e->modelBox.position, e->modelBox.position, e->entity->position);
                    return 1;
                }
            }
        }
    }
    vector2d_sub(e->modelBox.position, e->modelBox.position, e->entity->position);

    return 0;
}

void gf2d_physics_entity_collision_resolution(PhysicsEntity *e, CollisionInfo info)
{
    float theta = 0.0f;
    float speed = 0.0f;
    Vector2D buf = {0};
    Vector2D normalizedVel = {0};
    if(!e) return;

    vector2d_copy(normalizedVel, e->entity->velocity);
    vector2d_normalize(&normalizedVel);
    while( gf2d_collision_check(&info.a, &info.b, &info) )
    {
        vector2d_sub(buf, buf, normalizedVel);
        vector2d_sub(info.a.position, info.a.position, normalizedVel);
    }
    vector2d_add(e->entity->position, e->entity->position, buf);

    speed = vector2d_magnitude(e->entity->velocity);
    if( speed != 0.0f )
    {
        theta = acosf( (vector2d_dot_product(e->entity->velocity, info.normal)) / speed );
        vector2d_scale(e->entity->velocity, e->entity->velocity, sinf(theta));
    }

}

void gf2d_physics_entity_handle_collision( PhysicsEntity *e, PhysicsEntity *o, CollisionInfo info )
{
    float ve, vo, vmax; /* velocity e, velocity other */
    Vector2D buf;

    if(!e || !o) return;
    if(!e->canCollide || !o->canCollide) return;

    ve = vector2d_magnitude(e->entity->velocity);
    vo = vector2d_magnitude(o->entity->velocity);
    vmax = (ve > vo)? ve : vo;

    switch( e->type )
    {
    case PET_STATIC:
        
        if( o->type == PET_KINETIC )
        {
            
        }

        break;
    
    case PET_KINETIC:

        if( o->type == PET_KINETIC )
        {
            if( ve == vo )
            {
                vector2d_clear(e->entity->velocity);
                vector2d_clear(o->entity->velocity);
            }
            else
            {
                vector2d_sub( buf, e->entity->position, o->entity->position );
                vector2d_set_magnitude(&buf, vmax);
                if( vo < ve ) vector2d_scale(buf, buf, -1);
                vector2d_copy( e->entity->velocity, buf );
                vector2d_copy( o->entity->velocity, buf );
            }
        }
        else /* static */
        {
        }

        break;

    default:
        break;
    }

    gf2d_physics_entity_collision_resolution(e, info);
}