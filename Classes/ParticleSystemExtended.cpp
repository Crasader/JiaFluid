#include "ParticleSystemExtended.h"
#include "Utils/utils.h"
#include <string>

using namespace std;
USING_NS_CC;

ParticlePhysicsData::ParticlePhysicsData()
{
    memset(this, 0, sizeof(ParticlePhysicsData));
}

bool ParticlePhysicsData::init(ParticleData* basicData, int count, float radius, float rdensity, SphKernel *kernel)
{
    curCount = 0;
    maxCount = count;
    particleSpacing = radius / 2.f;
    kernelRadius = radius;
    restDensity = rdensity;
    sphKernel = kernel;
    sphKernel->setRadius(radius);
    basics = basicData;
    posx = basics->posx;
    posy = basics->posy;
    velocityX = (float*)malloc(count * sizeof(float));
    velocityY = (float*)malloc(count * sizeof(float));
    forceX = (float*)malloc(count * sizeof(float));
    forceY = (float*)malloc(count * sizeof(float));
    density = (float*)malloc(count * sizeof(float));
    pressure = (float*)malloc(count * sizeof(float));
    mass = (float*)malloc(count * sizeof(float));

    return velocityX && velocityY && forceX && forceY && density && pressure && mass && initMass();
}

bool ParticlePhysicsData::initMass() {
    Rect rect(-1.5 *kernelRadius, -1.5 * kernelRadius, kernelRadius * 3, kernelRadius * 3);
    PosArray points = fillRectWithPoints(rect, particleSpacing);
    double maxNumParticlesPerUnitArea = 0.0;
    for (size_t i = 0; i < points.size(); i++) {
        double numParticlesPerUnitArea = 0.0;
        for (size_t j = 0; j < points.size(); j++) {
            Vec2 v = points[i] - points[j];
            numParticlesPerUnitArea += sphKernel->value(v.length());
        }
        maxNumParticlesPerUnitArea = std::max(maxNumParticlesPerUnitArea, numParticlesPerUnitArea);
    }

    particleMass = restDensity / maxNumParticlesPerUnitArea;
    return true;
}

void ParticlePhysicsData::release()
{
    CC_SAFE_FREE(velocityX);
    CC_SAFE_FREE(velocityY);
    CC_SAFE_FREE(forceX);
    CC_SAFE_FREE(forceY);
    CC_SAFE_FREE(density);
    CC_SAFE_FREE(pressure);
    CC_SAFE_FREE(sphKernel);
}

ParticleSystemExtended::ParticleSystemExtended() {

}


int ParticleSystemExtended::GetNumParticles() {
    return _particleCount;
}

ParticleDataAccessors ParticleSystemExtended::GetAccessors() {
    ParticleDataAccessors p;
    p.pos_x = _particlePhysicsData.posx;
    p.pos_y = _particlePhysicsData.posy;
    p.velocity_x = _particlePhysicsData.velocityX;
    p.velocity_y = _particlePhysicsData.velocityY;
    p.force_x = _particlePhysicsData.forceX;
    p.force_y = _particlePhysicsData.forceY;
    p.density = _particlePhysicsData.density;
    p.pressure = _particlePhysicsData.pressure;
    p.mass = _particlePhysicsData.mass;
    return p;
}

ParticleSystemExtended* ParticleSystemExtended::create()
{
    ParticleSystemExtended* ret = new (std::nothrow) ParticleSystemExtended();
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

ParticleSystemExtended* ParticleSystemExtended::createWithTotalParticles(int numberOfParticles)
{
    ParticleSystemExtended* ret = new (std::nothrow) ParticleSystemExtended();
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

bool ParticleSystemExtended::initWithTotalParticles(int numberOfParticles)
{
    if (ParticleSystemQuad::initWithTotalParticles(numberOfParticles))
    {
        _particlePhysicsData.init(&_particleData, numberOfParticles);

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

bool ParticleSystemExtended::addParticle(Vec2 p) {
    if (_particlePhysicsData.curCount >= _particlePhysicsData.maxCount) {
        return false;
    }

    addParticles(1);
    _particleData.posx[_particleCount - 1] = p.x;
    _particleData.posy[_particleCount - 1] = p.y;
    
    _particlePhysicsData.velocityX[_particleCount - 1] = 0;
    _particlePhysicsData.velocityY[_particleCount - 1] = 0;
    _particlePhysicsData.forceX[_particleCount - 1] = 0;
    _particlePhysicsData.forceY[_particleCount - 1] = 0;
    _particlePhysicsData.density[_particleCount - 1] = 0;
    _particlePhysicsData.pressure[_particleCount - 1] = 0;
    _particlePhysicsData.mass[_particleCount - 1] = _particlePhysicsData.particleMass;
    _particlePhysicsData.curCount = _particleCount;
    return true;
}


unsigned int ParticleSystemExtended::fillRect(Rect rect) {
    auto points = fillRectWithPoints(rect, _particlePhysicsData.particleSpacing);
    unsigned int total = 0;
    for (size_t i = 0, j = _particlePhysicsData.curCount; i < points.size(); i++) {
        if (addParticle(points[i])) {
            total++;
        }
    }
    return total;
}

void ParticleSystemExtended::update(float dt)
{
    CC_PROFILER_START_CATEGORY(kProfilerCategoryParticles, "ParticleSystemExtended - update");

    if (_nFrames == 0) {
        unsigned int nAdded = fillRect(Rect(-100, -100, 240, 240));
    }
    
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
                _particlePhysicsData.copyParticle(i, _particleCount - 1);
                _particlePhysicsData.curCount = _particleCount;
                if (_batchNode)
                {
                    //disable the switched particle
                    int currentIndex = _particleData.atlasIndex[i];
                    _batchNode->disableParticle(_atlasIndex + currentIndex);
                    _particlePhysicsData.mass[_atlasIndex + currentIndex] = 0.0;
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

    CC_PROFILER_STOP_CATEGORY(kProfilerCategoryParticles, "ParticleSystemExtended - update");

}