#include "particle_2d_data.h"

int Particle2DData::GetNumParticles() {
    return current_count_;
}

ParticleDataAccessors Particle2DData::GetAccessors() {
    ParticleDataAccessors p;
    p.pos_x = posx_;
    p.pos_y = posy_;
    p.velocity_x = velocity_x_;
    p.velocity_y = velocity_y_;
    p.force_x = force_x_;
    p.force_y = force_y_;
    p.density = density_;
    p.pressure = pressure_;
    p.mass = mass_;
    return p;
}