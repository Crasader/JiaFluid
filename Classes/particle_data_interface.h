#ifndef __PARTICLE_DATA_INTERFACE_H__
#define __PARTICLE_DATA_INTERFACE_H__

struct ParticleDataAccessors {
    float* pos_x;
    float* pos_y;
    float* velocity_x;
    float* velocity_y;
    float* force_x;
    float* force_y;
    float* density;
    float* pressure;
    float* mass;
};

class ParticleDataInterface {
public:
    virtual int GetNumParticles() = 0;
    virtual ParticleDataAccessors GetAccessors() = 0;
};

#endif
