#ifndef __JFPARTICLE_SYSTEM_EXTENDED_H__
#define __JFPARTICLE_SYSTEM_EXTENDED_H__

#include "cocos2d.h"
#include "2d/CCParticleSystemQuad.h"
#include "SphKernel.h"

class ParticlePhysicsData {
public:
    ParticlePhysicsData();

    cocos2d::ParticleData* basics;

    float* posx;
    float* posy;
    float* velocityX;
    float* velocityY;
    float* forceX;
    float* forceY;
    float* density;
    float* pressure;
    float* mass;
    float particleMass;
    float particleSpacing;
    float kernelRadius;
    float restDensity;

    SphKernel *sphKernel;

    unsigned int maxCount;
    unsigned int curCount;
    bool init(cocos2d::ParticleData* basicData, int count, float radius = 10.0f, float rdensity = 3.0, SphKernel *kernel = SphPoly6Kernel2::create());
    void release();
    unsigned int getMaxCount() { return maxCount; }

    void copyParticle(int p1, int p2)
    {
        basics->copyParticle(p1, p2);

        velocityX[p1] = velocityX[p2];
        velocityY[p1] = velocityY[p2];
        forceX[p1] = forceX[p2];
        forceY[p1] = forceY[p2];
        density[p1] = density[p2];
        pressure[p1] = pressure[p2];
        mass[p1] = mass[p2];
    }

private:
    bool initMass();
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

    bool addParticle(cocos2d::Vec2 p);
    unsigned int fillRect(cocos2d::Rect rect);

    virtual void update(float dt) override;

private:

    CC_DISALLOW_COPY_AND_ASSIGN(ParticleSystemExtended);

};

#endif