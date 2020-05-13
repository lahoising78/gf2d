#include "simple_logger.h"
#include "gf2d_particles.h"
#include "gf2d_scene.h"

typedef struct
{
    Particle *particles;
    uint32_t particleCount;

    ParticleEmitter *emitters;
    uint32_t emitterCount;
} ParticleManager;

static ParticleManager particle_manager = {0};

void gf2d_particle_manager_close();

Particle *gf2d_particle_reserve_particles(uint32_t count);

void gf2d_particle_init(Particle *p);

void gf2d_particle_free(Particle *p);
void gf2d_particle_emitter_free(ParticleEmitter *emitter);

void gf2d_particle_manager_init(uint32_t maxParticleCount, uint32_t maxEmitterCount)
{
    int i;
    particle_manager.particles = gfc_allocate_array(sizeof(Particle), maxParticleCount);
    particle_manager.particleCount = maxParticleCount;
    for(i = 0; i < maxParticleCount; i++)
    {
        gf2d_particle_init(&particle_manager.particles[i]);
    }

    particle_manager.emitters = gfc_allocate_array(sizeof(ParticleEmitter), maxEmitterCount);
    particle_manager.emitterCount = maxEmitterCount;

    atexit(gf2d_particle_manager_close);
}

void gf2d_particle_init(Particle *p)
{
    if(!p) return;

    p->rend = gf2d_render_ent_new(NULL);
}

void gf2d_particle_manager_close()
{
    Particle *p = NULL;
    ParticleEmitter *emitter = NULL;
    int i;
    if(particle_manager.particles)
    {
        for(i = 0; i < particle_manager.particleCount; i++)
        {
            p = &particle_manager.particles[i];
            gf2d_particle_free(p);
        }
    }

    if(particle_manager.emitters)
    {
        for(i = 0; i < particle_manager.emitterCount; i++)
        {
            emitter = &particle_manager.emitters[i];
            gf2d_particle_emitter_free(emitter);
        }
    }
}

void gf2d_particle_free(Particle *p)
{
    if(!p || !p->rend) return;

    gf2d_render_ent_free(p->rend);
    free(p->rend);
}

void gf2d_particle_emitter_free(ParticleEmitter *emitter)
{
    if(!emitter) return;

    gf2d_sprite_free(emitter->particleSprite);
}


ParticleEmitter *gf2d_particle_emitter_new(uint32_t particleCount)
{
    int i;
    ParticleEmitter *emitter = NULL;

    for(i = 0; i < particle_manager.emitterCount; i++)
    {
        emitter = &particle_manager.emitters[i];
        if(emitter->_inuse) continue;

        emitter->particles = gf2d_particle_reserve_particles(particleCount);
        if(emitter->particles) emitter->_particleCount = particleCount;

        gf2d_timer_start(&emitter->nextEmission);

        return emitter;
    }

    return NULL;
}

Particle *gf2d_particle_reserve_particles(uint32_t count)
{
    int i;
    uint32_t counter = 0;
    Particle *arr = NULL;
    Particle *p = NULL;

    for(i = 0; i < particle_manager.particleCount; i++)
    {
        p = &particle_manager.particles[i];
        if(p->_inuse) continue;
        if(!arr) arr = p;

        p->_inuse = 1;

        counter++;
        if(counter >= count) break;
    }

    return arr;
}

void gf2d_particle_emitter_general_update(ParticleEmitter *emitter)
{
    uint32_t count = 0;
    Particle *p = NULL;
    uint8_t hacked = 0;
    float angle = 0.0f;
    if(!emitter) return;

    if(!emitter->firstLiving) emitter->firstLiving = emitter->particles;
    if(emitter->livingCount == 0) 
    {
        emitter->livingCount = 1;
        hacked = 1;
    }

    for(p = emitter->firstLiving; count < emitter->livingCount; p++, count++)
    {
        if(p > emitter->particles + emitter->_particleCount) p = emitter->particles;

        if(!p->flying)
        {
            p->rend->sprite = emitter->particleSprite;
            p->ttf = emitter->particleTtf;
            p->update = emitter->particleUpdate;
            p->velocity = emitter->particleInitialVelocity;
            vector2d_add(p->rend->position, emitter->position, emitter->particleOffset);
            angle = gfc_crandom() * GFC_DEGTORAD * emitter->spread;
            p->velocity = vector2d_rotate(emitter->particleInitialVelocity, angle);
            p->visible = 1;
            p->flying = 1;
            emitter->livingCount++;
            p->rend->colorShift.w = 255.0f;
            gf2d_scene_add_to_drawables(p->rend, DET_REND);
            gf2d_timer_start(&p->timeLiving);
            break;
        }

        if(p->update) p->update(p);
        else gf2d_particle_general_update(p);

        if(!p->flying) 
        {
            emitter->livingCount--;
            emitter->firstLiving++;
            if(emitter->firstLiving > emitter->particles + emitter->_particleCount)
            {
                emitter->firstLiving = emitter->particles;
            }
        }
    }

    if(hacked) emitter->livingCount--;

    if( gf2d_timer_get_ticks(&emitter->nextEmission) / 1000.0f >= emitter->emissionRate )
    {
        p->rend->sprite = emitter->particleSprite;
        p->ttf = emitter->particleTtf;
        p->update = emitter->particleUpdate;
        angle = gfc_crandom() * GFC_DEGTORAD * emitter->spread;
        p->velocity = vector2d_rotate(emitter->particleInitialVelocity, angle);
        p->visible = 1;
        p->flying = 1;
        vector2d_add(p->rend->position, emitter->position, emitter->particleOffset);
        p->rend->colorShift.w = 255.0f;
        emitter->livingCount++;
        gf2d_scene_add_to_drawables(p->rend, DET_REND);
        gf2d_timer_start(&p->timeLiving);
        gf2d_timer_start(&emitter->nextEmission);
    }
}

extern float frameTime;
void gf2d_particle_general_update(Particle *particle)
{
    Vector2D buf = {0};
    if(!particle) return;

    vector2d_scale(buf, particle->velocity, frameTime * 100.0f);
    vector2d_add( particle->rend->position, particle->rend->position, buf );

    if(gf2d_timer_get_ticks(&particle->timeLiving)/1000.0f >= particle->ttf)
    {
        particle->flying = 0;
        particle->visible = 0;
        gf2d_scene_remove_from_drawables(particle);
        particle->rend->colorShift.w = 0.0f;
    }
}