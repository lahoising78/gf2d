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

PhysicsEntity *gf2d_physics_entity_check_collision( PhysicsEntity *e, PhysicsEntity *o, CollisionInfo *info );
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
        
        if( ent->useGravity && !ent->_onFloor )
        {
            ent->entity->velocity.y += GRAVITY * frameTime;
        }

        if( ent->entity->velocity.y < 1.0f ) ent->_onFloor = 0;

        /* df = di + vt */
        vector2d_scale( buf, ent->entity->velocity, frameTime * TIME_MULTIPLIER );                                      // vt
        vector2d_add( ent->entity->position, ent->entity->position, buf );                                              // df = di + vt
        
        while( (o = gf2d_physics_entity_check_collision(ent, o, &info)) != NULL )
        {
            if( ent->canCollide && o->canCollide )
            {
                gf2d_physics_entity_handle_collision(ent, o, info);
            }

            if( ent->entity->touch )    ent->entity->touch( ent->entity, o->entity );
            if( o->entity->touch )      o->entity->touch( o->entity, ent->entity );
        }
        // else 
        // {
        //     ent->_onFloor = 0;
        // }

        if( gf2d_physics_entity_check_tilemap_collision(ent, &info) )
        {
            if( ent->canCollide )
            {
                slog("colliding with tile");
                gf2d_physics_entity_collision_resolution(ent, info);
            }
        }
        // else
        // {
        //     ent->_onFloor = 0;
        // }
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

PhysicsEntity *gf2d_physics_entity_check_collision( PhysicsEntity *e, PhysicsEntity *o, CollisionInfo *info )
{
    if(!e) return NULL;
    
    if(!o) o = &gf2d_physics_entity_manager.entity_list[0];
    else o++;
    vector2d_add(e->modelBox.position, e->modelBox.position, e->entity->position);
    for( ; o <= &gf2d_physics_entity_manager.entity_list[ gf2d_physics_entity_manager.count - 1 ]; o++)
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
    if(!e) return;

    /* go back to where we were not colliding */
    if( info.poc.x != 0.0f )
    {
        if( info.normal.x > 0.0f )
        {
            e->entity->position.x = info.poc.x - 1.0f;
        }
        else if( info.normal.x < 0.0f )
        {
            e->entity->position.x = info.poc.x - info.a.dimensions.wh.x - 1.0f;
        }
    }
    if( info.poc.y != 0.0f )
    {
        /* the thing you collided with is below you */
        if( info.normal.y > 0.0f )
        {
            e->entity->position.y = info.poc.y - e->modelBox.dimensions.wh.y;
            // slog("y %.2f", e->entity->position.y);
        }
        else if ( info.normal.y < 0.0f )
        {
            e->entity->position.y = info.poc.y;
        }
    }

    /* if the normal.y is positive, that means we are on top of a surface */
    if( info.normal.y > 0 )
        e->_onFloor = 1;

    speed = vector2d_magnitude(e->entity->velocity);
    if( speed != 0.0f )
    {
        /* A . B = ||A|| * ||B|| * cos(theta) where theta is the angle between the vectors */
        theta = acosf( (vector2d_dot_product(e->entity->velocity, info.normal)) / speed );
        
        /* A * sin(theta) will give us the line perpendicular to the normal, which would be parallel to the surface
           we are colliding with. The magnitude of the resulting vector would be proportional to the component of the
           original vector that is parallel to the surface */
        vector2d_scale(e->entity->velocity, e->entity->velocity, sinf(theta));

        /* do this to deal with floating point precision */
        if( fabs(e->entity->velocity.y) <= 1.0f ) e->entity->velocity.y = 0.0f;
        if( fabs(e->entity->velocity.x) <= 1.0f ) e->entity->velocity.x = 0.0f;
        // slog("vel %.2f %.2f", e->entity->velocity.x, e->entity->velocity.y);
    }

}

void gf2d_physics_entity_handle_collision( PhysicsEntity *e, PhysicsEntity *o, CollisionInfo info )
{
    // float ve, vo, vmax; /* velocity e, velocity other */
    // Vector2D buf;

    if(!e || !o) return;
    if(!e->canCollide || !o->canCollide) return;

    // ve = vector2d_magnitude(e->entity->velocity);
    // vo = vector2d_magnitude(o->entity->velocity);
    // vmax = (ve > vo)? ve : vo;

    switch( e->type )
    {
    case PET_STATIC:
        
        if( o->type == PET_KINETIC )
        {
            // gf2d_collision_check(&o->modelBox, &e->modelBox, &info);
            // e = o;
        }

        break;
    
    case PET_KINETIC:

        if( o->type == PET_KINETIC )
        {
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