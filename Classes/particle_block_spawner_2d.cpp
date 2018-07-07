#include "particle_block_spawner_2d.h"

std::vector<cocos2d::Vec2> ParticleBlockSpawner2D::Spawn(float time_interval) {
    return FillRectWithPoints(region_, spacing_);
}

std::vector<cocos2d::Vec2> ParticleBlockSpawner2D::FillRectWithPoints(cocos2d::Rect rect, double spacing) {
    std::vector<cocos2d::Vec2> points;
    double dx = spacing;
    double dy = spacing / 2.0 * std::sqrt(3);
    bool flag = false;
    for (double y = rect.getMinY(); y < rect.getMaxY(); y += dy) {
        double startingX = rect.getMinX();
        if (flag)
            startingX += spacing / 2.0;
        for (double x = startingX; x < rect.getMaxX(); x += dx) {
            points.push_back(cocos2d::Vec2(x, y));
        }
    }
    return points;
}