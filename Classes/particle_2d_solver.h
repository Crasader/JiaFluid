#ifndef __PARICLE_2D_SOLVER_INTERFACE_H__
#define __PARICLE_2D_SOLVER_INTERFACE_H__

#include "solver_interface.h"
#include "surface_set_interface.h"
#include "neighbor_searcher_2d_interface.h"
#include "SphKernel.h"

class Paricle2DSolver : public SolverInterface {
protected:
    Paricle2DSolver(
        NeighborSearcher2DInterface *neighbor_searcher,
        SphKernel *sph_kernel,
        float kernel_radius) :
        kernel_radius_(kernel_radius),
        neighbor_searcher_(neighbor_searcher),
        kernel_(sph_kernel) {};

    void ResolveCollision(
        const SurfaceSetInterface *surface_set,
        float *old_pos_x,
        float *old_pos_y,
        float *new_pos_x, 
        float *new_pos_y, 
        float *new_velocity_x,
        float *new_velocity_y);

    void InitForcesByGravity(
        float *force_x,
        float *force_y,
        float *mass);

    void UpdateForcesByViscosity(
        float *pos_x,
        float *pos_y,
        float *velocity_x,
        float *velocity_y,
        float *density,
        float *mass,
        float *force_x,
        float *force_y
    );

    void UpdateNeighborSearcher(
        float *pos_x,
        float *pos_y
    );

    void TimeIntegrate(
        float interval,
        float *old_pos_x,
        float *old_pos_y,
        float *old_velocity_x,
        float *old_velocity_y,
        float *mass,
        float *force_x,
        float *force_y,
        float *new_pos_x,
        float *new_pos_y,
        float *new_velocity_x,
        float *new_velocity_y);

    void Reset(ParticleDataInterface* particle_data);

    int num_particles_;
    ParticleDataAccessors a_;
    NeighborSearcher2DInterface* neighbor_searcher_;
    SphKernel* kernel_;
    float kernel_radius_;
    IndexLists neighbor_lists_;
    float time_interval_;
    std::vector<float> new_pos_x_;
    std::vector<float> new_pos_y_;
    std::vector<float> new_velocity_x_;
    std::vector<float> new_velocity_y_;
};

#endif 