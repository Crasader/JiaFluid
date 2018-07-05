#ifndef __NEIGHBOR_SEARCHER_2D_SOLVER_INTERFACE_H__
#define __NEIGHBOR_SEARCHER_2D_SOLVER_INTERFACE_H__

#include "cocos2d.h"

#include <vector>

using cocos2d::Vec2;

using IndexList = std::vector<size_t>;
using IndexLists = std::vector<IndexList>;

class NeighborSearcher2DInterface {
public:
    virtual void Build(
        int num_particles,
        float *pos_x,
        float *pos_y) = 0;

    virtual IndexLists GetNeighborsOfAllParticles() = 0;

    virtual IndexList GetNeighborsOfPoint(
        Vec2 point, 
        int num_particles) = 0;
};

#endif