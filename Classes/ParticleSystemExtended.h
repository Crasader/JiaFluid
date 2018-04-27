#ifndef __JFPARTICLE_SYSTEM_EXTENDED_H__
#define __JFPARTICLE_SYSTEM_EXTENDED_H__

#include "cocos2d.h"
#include "2d/CCParticleSystemQuad.h"
#include "SphKernel.h"

typedef std::vector<cocos2d::Vec2> PosArray;

class ParticlePhysicsData {
public:
    ParticlePhysicsData();

    float* posx;
    float* posy;
    float* velocityX;
    float* velocityY;
    float* forceX;
    float* forceY;
    float* density;
    float* pressure;
    float particleMass;
    float particleSpacing;
    float kernelRadius;
    float restDensity;

    SphKernel *sphKernel;

    unsigned int maxCount;
    unsigned int curCount;
    bool init(int count, float radius = 10.0f, float rdensity = 3.0, SphKernel *kernel = SphPoly6Kernel2::create());
    void release();
    unsigned int getMaxCount() { return maxCount; }

    bool addParticle(cocos2d::Vec2 p);
    unsigned int fillRect(cocos2d::Rect rect);

private:
    bool initMass();
    PosArray fillRectWithPoints(cocos2d::Rect rect, double spacing);
};

class ParticleSystemExtended : public cocos2d::ParticleSystemQuad {
protected:
    ParticlePhysicsData _particlePhysicsData;
    unsigned int _nFrames;


CC_CONSTRUCTOR_ACCESS:
    /**
    * @js ctor
    */
    ParticleSystemExtended();
    /**
    * @js NA
    * @lua NA
    */
    virtual ~ParticleSystemExtended() {}

public:
    static ParticleSystemExtended* create();
    static ParticleSystemExtended* createWithTotalParticles(int numberOfParticles);
    
    bool init() { return initWithTotalParticles(30000); }
    virtual bool initWithTotalParticles(int numberOfParticles);

    virtual void update(float dt) override;

private:
    bool syncData(unsigned int n);

    CC_DISALLOW_COPY_AND_ASSIGN(ParticleSystemExtended);

};

#endif