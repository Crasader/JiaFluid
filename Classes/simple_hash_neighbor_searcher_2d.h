#ifndef __SIMPLE_HASH_NEIGHBOR_SEARCHER_2D_SOLVER_INTERFACE_H__
#define __SIMPLE_HASH_NEIGHBOR_SEARCHER_2D_SOLVER_INTERFACE_H__

#include "neighbor_searcher_2d_interface.h"

class SimpleHashNeighborSearcher2D : public NeighborSearcher2DInterface {
public:
    SimpleHashNeighborSearcher2D(float cell_length, int n_buckets_per_axis, float particle_radius);

    void Build(
        int num_particles,
        float *pos_x,
        float *pos_y) override;

    IndexLists GetNeighborsOfAllParticles() override;

    IndexList GetNeighborsOfPoint(
        Vec2 point,
        int num_particles) override;

private:
    void BucketIndicesToLookup(float x, float y, std::vector<size_t>* indices);
    int BucketIndex(float x, float y);

    float cell_length_;
    int n_buckets_per_axis_;
    std::vector<std::vector<size_t>> bucket_;
    float particle_radius_;
    float *pos_x_;
    float *pos_y_;
    IndexLists neighbor_lists_;
};

#endif