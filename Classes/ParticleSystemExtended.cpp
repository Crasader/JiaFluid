#include "ParticleSystemExtended.h"
#include "Utils/utils.h"
#include <string>

using namespace std;
USING_NS_CC;

ParticlePhysicsData::ParticlePhysicsData()
{
    memset(this, 0, sizeof(ParticlePhysicsData));
}

bool ParticlePhysicsData::init(int count, float radius, float rdensity, SphKernel *kernel)
{
    curCount = 0;
    maxCount = count;
    particleSpacing = radius / 2.f;
    kernelRadius = radius;
    restDensity = rdensity;
    sphKernel = kernel;
    sphKernel->setRadius(radius);
    posx = (float*)malloc(count * sizeof(float));
    posy = (float*)malloc(count * sizeof(float));
    velocityX = (float*)malloc(count * sizeof(float));
    velocityY = (float*)malloc(count * sizeof(float));
    forceX = (float*)malloc(count * sizeof(float));
    forceY = (float*)malloc(count * sizeof(float));
    density = (float*)malloc(count * sizeof(float));
    pressure = (float*)malloc(count * sizeof(float));

    return velocityX && velocityY && forceX && forceY && density && pressure && initMass();
}

PosArray ParticlePhysicsData::fillRectWithPoints(cocos2d::Rect rect, double spacing) {
    PosArray points;
    double dx = spacing;
    double dy = spacing / 2.0 * std::sqrt(3);
    bool flag = false;
    for (double y = rect.getMinY(); y < rect.getMaxY(); y += dy) {
        double startingX = rect.getMinX();
        if (flag)
            startingX += spacing / 2.0;
        for (double x = startingX; x < rect.getMaxX(); x += dx) {
            points.push_back(Vec2(x, y));
        }
    }
    return points;
}

unsigned int ParticlePhysicsData::fillRect(Rect rect) {
    auto points = fillRectWithPoints(rect, particleSpacing);
    unsigned int total = 0;
    for (size_t i = 0, j = curCount; i < points.size() ; i++) {
        if (addParticle(points[i])) {
            total++;
        }
    }
    return total;
}

bool ParticlePhysicsData::addParticle(Vec2 p) {
    if (curCount >= maxCount) {
        return false;
    }

    posx[curCount] = p.x;
    posy[curCount] = p.y;
    velocityX[curCount] = 0;
    velocityY[curCount] = 0;
    forceX[curCount] = 0;
    forceY[curCount] = 0;
    density[curCount] = 0;
    pressure[curCount] = 0;
    curCount++;
    return true;
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
        _particlePhysicsData.init(numberOfParticles);

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

bool ParticleSystemExtended::syncData(unsigned int n) {
    addParticles(n);
    for (int i = n ; i >= 1; i--) {
        _particleData.posx[_particleCount - i] = _particlePhysicsData.posx[_particlePhysicsData.curCount - i];
        _particleData.posy[_particleCount - i] = _particlePhysicsData.posy[_particlePhysicsData.curCount - i];

    }
    return true;
}

void ParticleSystemExtended::update(float dt)
{
    if (_nFrames == 0) {
        unsigned int nAdded = _particlePhysicsData.fillRect(Rect(-100, -100, 240, 240));
        syncData(nAdded);
    }
    ParticleSystemQuad::update(dt);
    _nFrames++;
}