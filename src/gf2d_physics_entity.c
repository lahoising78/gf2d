#include "gf2d_physics_entity.h"
#include "simple_logger.h"

#include "gf2d_tilemap.h"
#include "gf2d_trie.h"

#define GRAVITY                     0.5f
#define FLOOR_ANGLE_THRESHOLD       0.5f

#define TIME_MULTIPLIER             1000.0f
extern float frameTime;

typedef struct
{
    PhysicsEntity           *entity_list;
    uint32_t                count;
    Trie                    entity_trie;
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

PhysicsEntityType gf2d_physics_entity_type_from_string(const char *str);

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
    gf2d_physics_entity_manager.entity_trie = gf2d_trie_new();

    atexit(gf2d_physics_entity_manager_close);
}

void gf2d_physics_entity_manager_initialize_entities()
{
    int i;
    PhysicsEntity *ent = NULL;

    for(i = 0; i < gf2d_physics_entity_manager.count; i++)
    {
        ent = &gf2d_physics_entity_manager.entity_list[i];
        ent->entity = gf2d_entity_new(NULL);
    }
}

void gf2d_physics_entity_manager_clean( uint32_t freeEntity )
{
    int i;
    PhysicsEntity *ent = NULL;
    slog("clean physics entity manager");

    for(i = 0; i < gf2d_physics_entity_manager.count; i++)
    {
        ent = &gf2d_physics_entity_manager.entity_list[i];
        gf2d_physics_entity_free(ent);
    }
    
    if(freeEntity)
    {
        memset(gf2d_physics_entity_manager.entity_list, 0, gf2d_physics_entity_manager.count*sizeof(PhysicsEntity));
        return;
    }

    gf2d_trie_free(&gf2d_physics_entity_manager.entity_trie);
    gf2d_physics_entity_manager.entity_trie = gf2d_trie_new();
}

void gf2d_physics_entity_manager_close()
{
    slog("close physics entity manager");

    gf2d_physics_entity_manager_clean(1);
    free(gf2d_physics_entity_manager.entity_list);
    gf2d_trie_free(&gf2d_physics_entity_manager.entity_trie);
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

void gf2d_physics_entity_manager_save(SJson *dst)
{
    PhysicsEntity *phys = NULL;
    int i;
    if(!dst) return;

    for(i = 0; i < gf2d_physics_entity_manager.count; i++)
    {
        phys = &gf2d_physics_entity_manager.entity_list[i];
        if(phys->name[0] == 0) continue;

        gf2d_physics_entity_save(phys);
    }
}

PhysicsEntity *gf2d_physics_entity_new(const char *name)
{
    int i;
    PhysicsEntity *ent = NULL;

    for(i = 0; i < gf2d_physics_entity_manager.count; i++)
    {
        ent = &gf2d_physics_entity_manager.entity_list[i];
        if(ent->_inuse) continue;

        ent->_inuse = 1;
        if(name && gfc_line_cmp(name, "") != 0)
        {
            gfc_line_cpy(ent->name, name);
            if(!gf2d_trie_insert(&gf2d_physics_entity_manager.entity_trie, ent->name, ent))
            {
                gf2d_physics_entity_free(ent);
                return NULL;
            }
        }
        
        return ent;
    }

    return NULL;
}

PhysicsEntityType gf2d_physics_entity_type_from_string(const char *str)
{
    if(strcmp(str, "PET_KINETIC") == 0) return PET_KINETIC;
    return PET_STATIC;
}

PhysicsEntity *gf2d_physics_entity_load(SJson *json)
{
    SJson *obj = NULL;
    PhysicsEntity *phys = NULL;

    if( sj_is_string(json) )
    {
        obj = sj_load( sj_get_string_value(json) );
            phys = gf2d_physics_entity_load(obj);
        sj_free(obj);
        return phys;
    }


    phys = gf2d_physics_entity_new(NULL);
    gf2d_physics_entity_load_to_entity(phys, json);

    return phys;
}

void gf2d_physics_entity_load_to_entity(PhysicsEntity *phys, SJson *json)
{
    SJson *obj = NULL;
    if(!phys || !json) return;
    // slog("phys ent load");

    obj = sj_object_get_value(json, "entity");
    if( sj_is_string(obj) )
    {
        obj = sj_load( sj_get_string_value(obj) );
        gf2d_entity_load_to_entity(phys->entity, obj);
        sj_free(obj);
    }
    else
    {
        gf2d_entity_load_to_entity(phys->entity, obj);
    }
    phys->modelBox = gf2d_collision_shape_load( sj_object_get_value(json, "modelBox") );
    phys->useGravity = gf2d_json_uint8( sj_object_get_value(json, "useGravity") );
    phys->canCollide = gf2d_json_uint8( sj_object_get_value(json, "canCollide") );
    phys->type = gf2d_physics_entity_type_from_string( sj_get_string_value( sj_object_get_value(json, "type") ) );

    obj = sj_object_get_value(json, "name");
    if(obj && sj_is_string(obj))
    {
        gf2d_physics_entity_set_name(phys, sj_get_string_value(obj));
    }
}

void gf2d_physics_entity_free( PhysicsEntity *ent )
{
    Entity *e = NULL;

    if(!ent) return;

    if( ent->name[0] != 0 )
    {
        gf2d_trie_remove(&gf2d_physics_entity_manager.entity_trie, ent->name);
    }

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
        info.originalPosition = ent->entity->position;
        
        if( ent->useGravity )
        {
            ent->entity->velocity.y += frameTime * GRAVITY * TIME_MULTIPLIER;
            // slog("%.2f %.2f", ent->entity->velocity.x, ent->entity->velocity.y);
        }
        ent->_onFloor = 0;

        /* df = di + vt */
        vector2d_scale( buf, ent->entity->velocity, frameTime * TIME_MULTIPLIER );                                      // vt
        vector2d_add( ent->entity->position, ent->entity->position, buf );
        
        while( (o = gf2d_physics_entity_check_collision(ent, o, &info)) != NULL )
        {
            if(ent->canCollide && o->canCollide )
            {
                gf2d_physics_entity_handle_collision(ent, o, info);
            }

            if( ent->entity->touch )    ent->entity->touch( ent->entity, o->entity );
            if( o->entity->touch )      o->entity->touch( o->entity, ent->entity );
        }

        if( ent->canCollide )
        {
            gf2d_physics_entity_check_tilemap_collision(ent, &info);
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
    Tile *tile = NULL;
    Vector2D worldToMap = {0};
    Vector2D entMax = {0};
    Vector2D initial = {0};

    if(!e || !tilemaps) return 0;

    // vector2d_add(e->modelBox.position, e->modelBox.position, e->entity->position);
    vector2d_add(initial, e->modelBox.position, e->entity->position);
    for(i = 0; i < count; i++)
    {
        vector2d_add(entMax, initial, e->modelBox.dimensions.wh);
        entMax = gf2d_tilemap_world_to_map(&tilemaps[i], entMax);
        
        if(entMax.x > 0.0f)
            entMax.x += 1.0f; 
        if(entMax.y > 0.0f) 
            entMax.y += 1.0f;
        
        if( entMax.x == -1.0f ) continue;
        
        worldToMap = gf2d_tilemap_world_to_map(&tilemaps[i], e->modelBox.position);
        
        if( worldToMap.x > 0.0f )
            worldToMap.x -= 1.0f; 
        if( worldToMap.y > 0.0f) 
            worldToMap.y -= 1.0f;

        if(worldToMap.x == -1.0f ) continue;

        if( worldToMap.x == -3.0f ) worldToMap.x = 0.0f;
        else if ( worldToMap.x == -2.0f ) worldToMap.x = (float)tilemaps[i].w - 1.0f;
        if( worldToMap.y == -3.0f ) worldToMap.y = 0.0f;
        else if ( worldToMap.y == -2.0f ) worldToMap.y = (float)tilemaps[i].h - 1.0f;
        
        if( entMax.x == -3.0f ) entMax.x = 0.0f;
        else if ( entMax.x == -2.0f ) entMax.x = (float)tilemaps[i].w - 1.0f;
        if( entMax.y == -3.0f ) entMax.y = 0.0f;
        else if ( entMax.y == -2.0f ) entMax.y = (float)tilemaps[i].h - 1.0f;

        for(y = (int)worldToMap.y; y <= entMax.y + 1; y++)
        {
            for(x = (int)worldToMap.x; x <= entMax.x + 1; x++)
            {
                tile = &tilemaps->tiles[y * tilemaps->w + x];
                if( tile->body.dimensions.wh.x <= 0.0f && tile->body.dimensions.wh.y <= 0.0f ) continue;

                vector2d_add(e->modelBox.position, e->modelBox.position, e->entity->position);
                vector2d_add(tile->body.position, tile->body.position, tile->_pos);
                if( gf2d_collision_check(&e->modelBox, &tile->body, info) ) 
                {
                    vector2d_sub(e->modelBox.position, e->modelBox.position, e->entity->position);
                    vector2d_sub(tile->body.position, tile->body.position, tile->_pos);
                    gf2d_physics_entity_collision_resolution(e, *info);
                    continue;
                }
                vector2d_sub(tile->body.position, tile->body.position, tile->_pos);
                vector2d_sub(e->modelBox.position, e->modelBox.position, e->entity->position);
            }
        }
    }

    return 0;
}

void gf2d_physics_entity_collision_resolution(PhysicsEntity *e, CollisionInfo info)
{
    Vector2D dir = {0};
    Vector2D up = {0.0f, -1.0f};
    if(!e) return;

    // dot = vector2d_dot_product(info.normal, e->entity->velocity);
    // vector2d_scale(dir, info.normal, dot);
    // vector2d_sub(e->entity->velocity, e->entity->velocity, dir);
    // vector2d_scale(dir, e->entity->velocity, frameTime * TIME_MULTIPLIER);
    // vector2d_add(e->entity->position, info.originalPosition, dir);

    // vector2d_sub(dir, info.b.position, info.a.position);
    // mag = vector2d_magnitude(dir);
    // vector2d_scale(dir, dir, info.overlap / mag);
    // vector2d_sub(e->entity->position, e->entity->position, dir);
    // slog("resolution");

    vector2d_scale(dir, info.normal, info.overlap);
    vector2d_add(e->entity->position, e->entity->position, dir);

    /* ngl, took strong inspiration from godot engine */
    e->_onFloor = acosf(vector2d_dot_product(up, info.normal)) <= FLOOR_ANGLE_THRESHOLD;
    if(e->_onFloor) e->entity->velocity.y = 0.0f;
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

PhysicsEntity *gf2d_physics_entity_get_by_name(const char *name)
{
    if(!name) return NULL;

    return (PhysicsEntity*)gf2d_trie_get(&gf2d_physics_entity_manager.entity_trie, name);
}

void gf2d_physics_entity_set_name(PhysicsEntity *ent, const char *name)
{
    if(!ent || !name) return;

    gf2d_trie_remove(&gf2d_physics_entity_manager.entity_trie, name);
    gfc_line_cpy(ent->name, name);
    gf2d_trie_insert(&gf2d_physics_entity_manager.entity_trie, name, ent);
}

SJson *gf2d_physics_entity_save(PhysicsEntity *phys)
{
    SJson *obj = NULL;
    SJson *data = NULL;
    
    if(!phys) return NULL;

    obj = sj_object_new();
    if(!obj) return NULL;

    /* position */
    data = sj_array_new();
    if(data)
    {
        gf2d_json_vec2d_save(data, phys->entity->position);
        sj_object_insert(obj, "position", data);
    }

    /* name */
    if(phys->name)
    {
        data = sj_new_str(phys->name);
        if(data)
        {
            sj_object_insert(obj, "name", data);
        }
    }

    return obj;
}