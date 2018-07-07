#ifndef __JFPARTICLE_SYSTEM_MANAGER_H__
#define __JFPARTICLE_SYSTEM_MANAGER_H__

#include "cocos2d.h"
#include "2d/CCParticleSystemQuad.h"
#include "SphKernel.h"
#include "particle_data_interface.h"
#include "particle_2d_data.h"
#include "solver_interface.h"
#include "wcsph_particle_2d_solver.h"
#include "particle_block_spawner_2d.h"
#include "simple_surface_set.h"
#include "simple_hash_neighbor_searcher_2d.h"

#include <memory>

class ParticleSystemManager : public cocos2d::ParticleSystemQuad {
public:

protected:
    unsigned int _nFrames;


CC_CONSTRUCTOR_ACCESS:
    /**
    * @js ctor
    */
    ParticleSystemManager();
    /**
    * @js NA
    * @lua NA
    */
    virtual ~ParticleSystemManager() {}

public:
    static ParticleSystemManager* create();
    static ParticleSystemManager* createWithTotalParticles(int numberOfParticles);
    
    bool init() { return initWithTotalParticles(30000); }
    virtual bool initWithTotalParticles(int numberOfParticles);

    bool addParticle(cocos2d::Vec2 p);
    unsigned int fillRect(cocos2d::Rect rect);
    void SpwanParticles(float dt);
    void SolveParticleDynamics(float dt);

    virtual void update(float dt) override;

private:

    void copyParticle(int p1, int p2)
    {
        _particleData.copyParticle(p1, p2);

        velocity_x_[p1] = velocity_x_[p2];
        velocity_y_[p1] = velocity_y_[p2];
        force_x_[p1] = force_x_[p2];
        force_y_[p1] = force_y_[p2];
        density_[p1] = density_[p2];
        pressure_[p1] = pressure_[p2];
        mass_[p1] = mass_[p2];
    }

    std::unique_ptr<float[]> velocity_x_;
    std::unique_ptr<float[]> velocity_y_;
    std::unique_ptr<float[]> force_x_;
    std::unique_ptr<float[]> force_y_;
    std::unique_ptr<float[]> density_;
    std::unique_ptr<float[]> pressure_;
    std::unique_ptr<float[]> mass_;
    std::unique_ptr<SphKernel> sph_kernel_;
    std::unique_ptr<Particle2DData> particle_data_extended_;
    std::unique_ptr<SolverInterface> solver_;
    std::vector<std::unique_ptr<ParticleSpawner2DInterface>> one_time_spawners_;
    std::unique_ptr<SurfaceSetInterface> surface_set_;
    std::unique_ptr<NeighborSearcher2DInterface> neighbor_searcher_;

    float particle_mass_;
    float particle_spacing_;
    float kernel_radius_;
    float rest_density_;

    CC_DISALLOW_COPY_AND_ASSIGN(ParticleSystemManager);

};

#endif