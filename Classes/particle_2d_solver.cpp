#include "particle_2d_solver.h"

#include <cmath>
#include <algorithm>

void Paricle2DSolver::ResolveCollision(
    const SurfaceSetInterface* surface_set,
    float *old_pos_x,
    float *old_pos_y,
    float *new_pos_x,
    float *new_pos_y,
    float *new_velocity_x,
    float *new_velocity_y) {

    constexpr float resistitution_coefficient = 0.2;
    constexpr float friction = 0.5;
    constexpr float radius = 0.01;

    for (int i = 0; i < num_particles_; i++) {
        IntersectionData2 intersection;
        Vec2 point1(old_pos_x[i], old_pos_y[i]);
        Vec2 point2(new_pos_x[i], new_pos_y[i]);

        if (surface_set->GetClosestIntersection(point1, point2, &intersection)) {
            Vec2 relative_velocity(new_velocity_x[i], new_velocity_y[i]);
            Vec2 relative_velocity_n = intersection.normal_ * intersection.normal_.dot(relative_velocity);
            Vec2 relative_velocity_t = relative_velocity - relative_velocity_n;
            Vec2 delta_n = relative_velocity_n * (-resistitution_coefficient - 1);
            relative_velocity_n *= -resistitution_coefficient;
            relative_velocity_t *= std::max(1.0 - friction * delta_n.length() / relative_velocity_t.length(), 0.0);
            Vec2 new_v = relative_velocity_n + relative_velocity_t;
            Vec2 new_p = intersection.point_ + intersection.normal_ * radius;

            new_pos_x[i] = new_p.x;
            new_pos_y[i] = new_p.y;
            new_velocity_x[i] = new_v.x;
            new_velocity_y[i] = new_v.y;
        }
    }
}

void Paricle2DSolver::InitForcesByGravity(
    float *force_x,
    float *force_y,
    float *mass) {

    const Vec2 gravity(0, -9.8);
    for (int i = 0; i < num_particles_; i++) {
        force_x[i] = gravity.x * mass[i];
        force_y[i] = gravity.y * mass[i];
    }
}

void Paricle2DSolver::UpdateForcesByViscosity(
    float *pos_x,
    float *pos_y,
    float *velocity_x,
    float *velocity_y,
    float *density,
    float *mass,
    float *force_x,
    float *force_y
) {
    constexpr float viscosity_coeff = 1.0;
    for (int i = 0; i < num_particles_; i++) {
        for (size_t j : neighbor_lists_[i]) {
            const Vec2 v(pos_x[j] - pos_x[i], pos_y[j] - pos_y[i]); 
            const float distance = v.length();
            const float t = mass[i] * mass[j] * kernel_->secondDerivative(distance) * viscosity_coeff / density[j];
            force_x[i] += (velocity_x[j] - velocity_x[i]) * t;
            force_y[i] += (velocity_y[j] - velocity_y[i]) * t;
        }
    }
}

void Paricle2DSolver::UpdateNeighborSearcher(
    float *pos_x,
    float *pos_y) {
    neighbor_searcher_->Build(num_particles_, pos_x, pos_y);
    neighbor_lists_ = neighbor_searcher_->GetNeighborsOfAllParticles();
}

void Paricle2DSolver::Reset(ParticleDataInterface* particle_data) {
    num_particles_ = particle_data->GetNumParticles();
    a_ = particle_data->GetAccessors();
    new_pos_x_.resize(num_particles_);
    new_pos_y_.resize(num_particles_);
    new_velocity_x_.resize(num_particles_);
    new_velocity_y_.resize(num_particles_);
    UpdateNeighborSearcher(a_.pos_x, a_.pos_y);
}

void Paricle2DSolver::TimeIntegrate(
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
    float *new_velocity_y
) {
    for (int i = 0; i < num_particles_; i++) {
        new_velocity_x[i] = old_velocity_x[i] + force_x[i] * interval / mass[i];
        new_velocity_y[i] = old_velocity_y[i] + force_y[i] * interval / mass[i];
        assert(!isnan(new_velocity_x[i]));
        assert(!isnan(new_velocity_y[i]));
    }

    for (int i = 0; i < num_particles_; i++) {
        new_pos_x[i] = old_pos_x[i] + new_velocity_x[i] * interval;
        new_pos_y[i] = old_pos_y[i] + new_velocity_y[i] * interval;
        assert(!isnan(new_pos_x[i]));
        assert(!isnan(new_pos_y[i]));
    }
}