#ifndef __PARTICLE_SPAWNER_2D_INTERFACE_H__
#define __PARTICLE_SPAWNER_2D_INTERFACE_H__

#include "cocos2d.h"

#include <vector>

class ParticleSpawner2DInterface {
public:
    virtual std::vector<cocos2d::Vec2> Spawn(float time_interval) = 0;

};

#endif