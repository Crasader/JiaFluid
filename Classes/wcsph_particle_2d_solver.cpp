#include "wcsph_particle_2d_solver.h"

#include <algorithm>

int WcsphParicle2DSolver::NumSubFrames(
    float *force_x,
    float *force_y) {

    float max_force = 0.0;
    float cfl_interval = std::numeric_limits<float>::max();
    for (size_t i = 0; i < num_particles_; i++) {
        max_force = std::max(Vec2(force_x[i], force_y[i]).length(), max_force);
    }
    if (max_force > 0.0)
        cfl_interval = 0.25 * kernel_radius_ / max_force;
    cfl_interval = std::min(cfl_interval, 0.4f * kernel_radius_ / (speed_of_sound_ * (1 + 0.6f * viscosity_coeff_)));
    return std::ceil(time_interval_ / cfl_interval);
}

bool WcsphParicle2DSolver::Solve(
    float time_interval,
    SurfaceSetInterface* surface_set,
    ParticleDataInterface* particle_data) {
    
    time_interval_ = time_interval;
    Reset(particle_data);
    ParticleDataAccessors a = particle_data->GetAccessors();
    int n = NumSubFrames(a.force_x, a.force_y);
    float sub_interval = time_interval_ / n;
    
    for (int i = 0; i < n; i++) {
        InitForcesByGravity(a.force_x, a.force_y, a.mass);
        TimeIntegrate(sub_interval, 
            a.pos_x, a.pos_y, a.velocity_x, a.velocity_y, a.mass, a.force_x, a.force_y, 
            new_pos_x_.data(), new_pos_y_.data(), 
            new_velocity_x_.data(), new_velocity_y_.data());
        ResolveCollision(surface_set, a.pos_x, a.pos_y, new_pos_x_.data(), new_pos_y_.data(),
            new_velocity_x_.data(), new_velocity_y_.data());

        memcpy(a.pos_x, new_pos_x_.data(), sizeof(float) * num_particles_);
        memcpy(a.pos_y, new_pos_y_.data(), sizeof(float) * num_particles_);
        memcpy(a.velocity_x, new_velocity_x_.data(), sizeof(float) * num_particles_);
        memcpy(a.velocity_y, new_velocity_y_.data(), sizeof(float) * num_particles_);
    }

    return true;
}