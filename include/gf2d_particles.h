#ifndef _GF2D_PARTICLES_H_
#define _GF2D_PARTICLES_H_

#include "gf2d_render_ent.h"
#include "gf2d_timer.h"

typedef struct particle_s
{
    Vector2D velocity;
    float ttf;
    Timer timeLiving;
    RenderEntity *rend;

    void (*update)(struct particle_s *self);

    uint8_t visible;
    uint8_t flying;
    uint8_t _inuse;
} Particle;

typedef struct emitter_s
{
    Vector2D position;
    float emissionRate;
    Timer nextEmission;
    float direction;

    uint8_t emitting;

    Particle *particles;
    uint32_t _particleCount;

    Particle *firstLiving;
    uint32_t livingCount;
    void (*update)(struct emitter_s *self);

    Sprite *particleSprite;
    Vector2D particleOffset;
    float particleTtf;
    Vector2D particleInitialVelocity;
    void (*particleUpdate)(Particle *particle);
    float spread;

    uint8_t _inuse;
    uint8_t loop;
} ParticleEmitter;

void gf2d_particle_manager_init(uint32_t maxParticleCount, uint32_t maxEmitterCount);

ParticleEmitter *gf2d_particle_emitter_new(uint32_t particleCount);
void gf2d_particle_emitter_general_update(ParticleEmitter *emitter);
void gf2d_particle_general_update(Particle *particle);

#endif