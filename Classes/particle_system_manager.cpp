#include "particle_system_manager.h"
#include "Utils/utils.h"
#include <string>

using namespace std;
USING_NS_CC;

ParticleSystemManager::ParticleSystemManager() {

}

ParticleSystemManager* ParticleSystemManager::create()
{
    ParticleSystemManager* ret = new (std::nothrow) ParticleSystemManager();
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

ParticleSystemManager* ParticleSystemManager::createWithTotalParticles(int numberOfParticles)
{
    ParticleSystemManager* ret = new (std::nothrow) ParticleSystemManager();
    if (ret && ret->initWithTotalParticles(numberOfParticles))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ParticleSystemManager::initWithTotalParticles(int numberOfParticles)
{
    if (ParticleSystemQuad::initWithTotalParticles(numberOfParticles))
    {
        kernel_radius_ = 10.f;
        rest_density_ = 3.f;
        particle_spacing_ = kernel_radius_ * 0.5f;
        sph_kernel_ = std::make_unique<SphPoly6Kernel2>(kernel_radius_);
        particle_mass_ = InitMass(kernel_radius_, particle_spacing_, rest_density_, sph_kernel_.get());

        velocity_x_ = std::make_unique<float[]>(numberOfParticles);
        velocity_y_ = std::make_unique<float[]>(numberOfParticles);
        force_x_ = std::make_unique<float[]>(numberOfParticles);
        force_y_ = std::make_unique<float[]>(numberOfParticles);
        density_ = std::make_unique<float[]>(numberOfParticles);
        pressure_ = std::make_unique<float[]>(numberOfParticles);
        mass_ = std::make_unique<float[]>(numberOfParticles);

        neighbor_searcher_ = std::make_unique<SimpleHashNeighborSearcher2D>(100.0, 10.0, kernel_radius_);

        solver_ = std::make_unique<WcsphParicle2DSolver>(neighbor_searcher_.get(), sph_kernel_.get(), kernel_radius_, rest_density_, 7.0, 100.0, 1.0);

        surface_set_ = std::make_unique<SimpleSurfaceSet>();

        particle_data_extended_ = std::make_unique<Particle2DData>(
            _particleData.posx,
            _particleData.posy,
            velocity_x_.get(),
            velocity_y_.get(),
            force_x_.get(),
            force_y_.get(),
            density_.get(),
            pressure_.get(),
            mass_.get());
        particle_data_extended_->current_count_ = 0;
        particle_data_extended_->max_count_ = numberOfParticles;

        one_time_spawners_.push_back(std::make_unique<ParticleBlockSpawner2D>(
            0.0, particle_spacing_, Rect(-100, -100, 240, 240)));

        // duration
        _duration = DURATION_INFINITY;

        setEmitterMode(Mode::GRAVITY);

        // Gravity Mode: gravity
        setGravity(Vec2(0, 0));

        // Gravity Mode: radial
        setRadialAccel(0);
        setRadialAccelVar(0);

        // Gravity Mode: tangential
        setTangentialAccel(0);
        setTangentialAccelVar(0);

        // Gravity Mode: speed of particles
        setSpeed(0);
        setSpeedVar(0);

        // angle
        _angle = -90;
        _angleVar = 5;


        // emitter position
        Size winSize = Director::getInstance()->getWinSize();
        this->setPosition(winSize.width / 2, winSize.height);
        setPosVar(Vec2(winSize.width / 2, 0));



        // life of particles
        _life = 14.5f;
        _lifeVar = 0;

        // size, in pixels
        _startSize = 4.0f;
        _startSizeVar = 2.0f;
        _endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per second
        _emissionRate = 20;

        // color of particles
        _startColor.r = 0.7f;
        _startColor.g = 0.8f;
        _startColor.b = 1.0f;
        _startColor.a = 1.0f;
        _startColorVar.r = 0.0f;
        _startColorVar.g = 0.0f;
        _startColorVar.b = 0.0f;
        _startColorVar.a = 0.0f;
        _endColor.r = 0.7f;
        _endColor.g = 0.8f;
        _endColor.b = 1.0f;
        _endColor.a = 0.5f;
        _endColorVar.r = 0.0f;
        _endColorVar.g = 0.0f;
        _endColorVar.b = 0.0f;
        _endColorVar.a = 0.0f;

        Texture2D* texture = getDefaultTexture();
        if (texture != nullptr)
        {
            setTexture(texture);
        }

        // additive
        this->setBlendAdditive(false);

        _nFrames = 0;

        return true;
    }
    return false;
}

bool ParticleSystemManager::addParticle(Vec2 p) {
    if (particle_data_extended_->current_count_ >= particle_data_extended_->max_count_) {
        return false;
    }

    addParticles(1);
    _particleData.posx[_particleCount - 1] = p.x;
    _particleData.posy[_particleCount - 1] = p.y;
    
    velocity_x_[_particleCount - 1] = 0;
    velocity_y_[_particleCount - 1] = 0;
    force_x_[_particleCount - 1] = 0;
    force_y_[_particleCount - 1] = 0;
    density_[_particleCount - 1] = 0;
    pressure_[_particleCount - 1] = 0;
    mass_[_particleCount - 1] = particle_mass_;
    particle_data_extended_->current_count_ = _particleCount;
    return true;
}


unsigned int ParticleSystemManager::fillRect(Rect rect) {
    auto points = fillRectWithPoints(rect, particle_spacing_);
    unsigned int total = 0;
    for (size_t i = 0; i < points.size(); i++) {
        if (addParticle(points[i])) {
            total++;
        }
    }
    return total;
}

void ParticleSystemManager::SpwanParticles(float dt) {
    for (auto& spwaner : one_time_spawners_) {
        std::vector<cocos2d::Vec2> points = spwaner->Spawn(dt);
        for (auto& point : points) {
            addParticle(point);
        }
    }
    one_time_spawners_.clear();
}

void ParticleSystemManager::SolveParticleDynamics(float dt) {
    solver_->Solve(dt, surface_set_.get(), particle_data_extended_.get());
}

void ParticleSystemManager::update(float dt)
{
    CC_PROFILER_START_CATEGORY(kProfilerCategoryParticles, "ParticleSystemManager - update");

    SpwanParticles(dt);

    SolveParticleDynamics(dt);
    
    {
        for (int i = 0; i < _particleCount; ++i)
        {
            _particleData.timeToLive[i] -= dt;
        }

        for (int i = 0; i < _particleCount; ++i)
        {
            if (_particleData.timeToLive[i] <= 0.0f)
            {
                int j = _particleCount - 1;
                while (j > 0 && _particleData.timeToLive[j] <= 0)
                {
                    _particleCount--;
                    j--;
                }
                copyParticle(i, _particleCount - 1);
                particle_data_extended_->current_count_ = _particleCount;
                if (_batchNode)
                {
                    //disable the switched particle
                    int currentIndex = _particleData.atlasIndex[i];
                    _batchNode->disableParticle(_atlasIndex + currentIndex);
                    mass_[_atlasIndex + currentIndex] = 0.0;
                    //switch indexes
                    _particleData.atlasIndex[_particleCount - 1] = currentIndex;
                }
                --_particleCount;
                if (_particleCount == 0 && _isAutoRemoveOnFinish)
                {
                    this->unscheduleUpdate();
                    _parent->removeChild(this, true);
                    return;
                }
            }
        }

        //color r,g,b,a
        for (int i = 0; i < _particleCount; ++i)
        {
            _particleData.colorR[i] += _particleData.deltaColorR[i] * dt;
        }

        for (int i = 0; i < _particleCount; ++i)
        {
            _particleData.colorG[i] += _particleData.deltaColorG[i] * dt;
        }

        for (int i = 0; i < _particleCount; ++i)
        {
            _particleData.colorB[i] += _particleData.deltaColorB[i] * dt;
        }

        for (int i = 0; i < _particleCount; ++i)
        {
            _particleData.colorA[i] += _particleData.deltaColorA[i] * dt;
        }

        updateParticleQuads();
        _transformSystemDirty = false;
    }

    // only update gl buffer when visible
    if (_visible && !_batchNode)
    {
        postStep();
    }

    _nFrames++;

    CC_PROFILER_STOP_CATEGORY(kProfilerCategoryParticles, "ParticleSystemManager - update");

}