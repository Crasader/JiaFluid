#ifndef __PARICLE_2D_DATA_H__
#define __PARICLE_2D_DATA_H__

#include "particle_data_interface.h"
#include "SphKernel.h"
#include "Utils/utils.h"

class Particle2DData : public ParticleDataInterface {
public:
    Particle2DData(
        float* posx,
        float* posy,
        float* velocity_x,
        float* velocity_y,
        float* force_x,
        float* force_y,
        float* density,
        float* pressure,
        float* mass) :
        posx_(posx),
        posy_(posy),
        velocity_x_(velocity_x),
        velocity_y_(velocity_y),
        force_x_(force_x),
        force_y_(force_y),
        density_(density),
        pressure_(pressure),
        mass_(mass) {}

    int GetNumParticles() override;
    ParticleDataAccessors GetAccessors() override;

    SphKernel *sph_kernel_;

    unsigned int max_count_;
    unsigned int current_count_;


private:


    float* posx_;
    float* posy_;
    float* velocity_x_;
    float* velocity_y_;
    float* force_x_;
    float* force_y_;
    float* density_;
    float* pressure_;
    float* mass_;
};

#endif