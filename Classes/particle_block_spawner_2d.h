#ifndef __PARTICLE_BLOCK_SPAWNER_2D_H__
#define __PARTICLE_BLOCK_SPAWNER_2D_H__

#include "particle_spawner_2d_interface.h"

class ParticleBlockSpawner2D : public ParticleSpawner2DInterface {
public:
    ParticleBlockSpawner2D(
        float frequency,
        float spacing,
        cocos2d::Rect region) :
        frequency_(frequency),
        spacing_(spacing),
        region_(region),
        time_elapsed_(0.0f) {};

    std::vector<cocos2d::Vec2> Spawn(float time_interval) override;
private:
    std::vector<cocos2d::Vec2> FillRectWithPoints(cocos2d::Rect rect, double spacing);

    cocos2d::Rect region_;
    float frequency_;
    float spacing_;
    float time_elapsed_;
};

#endif