#ifndef __WCSPH_PARICLE_2D_SOLVER_INTERFACE_H__
#define __WCSPH_PARICLE_2D_SOLVER_INTERFACE_H__

#include "particle_2d_solver.h"

class WcsphParicle2DSolver : public Paricle2DSolver {
public:
    bool Solve(
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