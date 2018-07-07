#ifndef __WCSPH_PARICLE_2D_SOLVER_INTERFACE_H__
#define __WCSPH_PARICLE_2D_SOLVER_INTERFACE_H__

#include "particle_2d_solver.h"

class WcsphParicle2DSolver : public Paricle2DSolver {
public:
    WcsphParicle2DSolver(
        NeighborSearcher2DInterface *neighbor_searcher,
        SphKernel *sph_kernel,
        float kernel_radius,
        float rest_density,
        float eos_exponent,
        float speed_of_sound,
        float viscosity_coeff) :
        Paricle2DSolver(neighbor_searcher, sph_kernel, kernel_radius),
        rest_density_(rest_density),
        eos_exponent_(eos_exponent),
        speed_of_sound_(speed_of_sound),
        viscosity_coeff_(viscosity_coeff) {};


    bool Solve(
        float time_interval,
        SurfaceSetInterface* surface_set,
        ParticleDataInterface* particle_data) override;

private:
    int NumSubFrames(
        float *force_x,
        float *force_y);

    float rest_density_ = 3.0;
    float eos_exponent_ = 7.0;
    float speed_of_sound_ = 100.0;
    float viscosity_coeff_ = 1.0;
};

#endif