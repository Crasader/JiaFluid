#include "simple_hash_neighbor_searcher_2d.h"

SimpleHashNeighborSearcher2D::SimpleHashNeighborSearcher2D(
    float cell_length,
    int n_buckets_per_axis,
    float particle_radius)
    : cell_length_(cell_length), 
    n_buckets_per_axis_(n_buckets_per_axis),
    particle_radius_(particle_radius) {}

void SimpleHashNeighborSearcher2D::BucketIndicesToLookup(float x, float y, std::vector<size_t>* indices) {
    size_t bucket_index = BucketIndex(x, y);
    indices->clear();
    indices->push_back(bucket_index);
    float cell_center_x = static_cast<int>(std::floor(x / cell_length_)) * cell_length_ + 0.5 * cell_length_;
    float cell_center_y = static_cast<int>(std::floor(y / cell_length_)) * cell_length_ + 0.5 * cell_length_;

    if (x < cell_center_x && y < cell_center_y) {
        indices->push_back(BucketIndex(x - cell_length_, y));
        indices->push_back(BucketIndex(x, y - cell_length_));
        indices->push_back(BucketIndex(x - cell_length_, y - cell_length_));
    }
    else if (x >= cell_center_x && y < cell_center_y) {
        indices->push_back(BucketIndex(x + cell_length_, y));
        indices->push_back(BucketIndex(x, y - cell_length_));
        indices->push_back(BucketIndex(x + cell_length_, y - cell_length_));
    }
    else if (x < cell_center_x && y >= cell_center_y) {
        indices->push_back(BucketIndex(x - cell_length_, y));
        indices->push_back(BucketIndex(x, y + cell_length_));
        indices->push_back(BucketIndex(x - cell_length_, y + cell_length_));
    }
    else if (x >= cell_center_x && y >= cell_center_y) {
        indices->push_back(BucketIndex(x + cell_length_, y));
        indices->push_back(BucketIndex(x, y + cell_length_));
        indices->push_back(BucketIndex(x + cell_length_, y + cell_length_));
    }
}

int SimpleHashNeighborSearcher2D::BucketIndex(float x, float y) {
    int bucketX = static_cast<int>(std::floor(x / cell_length_)) % n_buckets_per_axis_;
    int bucketY = static_cast<int>(std::floor(y / cell_length_)) % n_buckets_per_axis_;
    bucketX = (bucketX + n_buckets_per_axis_) % n_buckets_per_axis_;
    bucketY = (bucketY + n_buckets_per_axis_) % n_buckets_per_axis_;
    int bucketIndex = bucketY * n_buckets_per_axis_ + bucketX;
    return bucketIndex;
}

void SimpleHashNeighborSearcher2D::Build(
    int num_particles,
    float *pos_x,
    float *pos_y) {

    assert(particle_radius_ * 2 <= cell_length_);

    pos_x_ = pos_x;
    pos_y_ = pos_y;

    bucket_.clear();
    bucket_.resize(n_buckets_per_axis_ * n_buckets_per_axis_);

    neighbor_lists_.clear();
    neighbor_lists_.resize(num_particles);

    // put all particles into buckets
    for (size_t i = 0; i < num_particles; i++) {
        const int bucket_index = BucketIndex(pos_x[i], pos_y[i]);
        bucket_[bucket_index].push_back(i);
    }

    // get neighbors
    for (size_t i = 0; i < num_particles; i++) {
        std::vector<size_t> bucket_lookup;
        BucketIndicesToLookup(pos_x[i], pos_y[i], &bucket_lookup);
        assert(bucket_lookup.size() == 4);
        for (size_t j = 0; j < 4; j++) {
            for (size_t k : bucket_[bucket_lookup[j]]) {
                if (k == i) continue;
                Vec2 v(pos_x[i] - pos_x[k], pos_y[i] - pos_y[k]);
                if (v.dot(v) < particle_radius_ * particle_radius_) {
                    neighbor_lists_[i].push_back(k);
                }
            }
        }
    }
}

IndexLists SimpleHashNeighborSearcher2D::GetNeighborsOfAllParticles() {
    return neighbor_lists_;
}

IndexList SimpleHashNeighborSearcher2D::GetNeighborsOfPoint(
    Vec2 point,
    int num_particles) {

    std::vector<size_t> bucket_lookup;
    IndexList neighbor_list;
    BucketIndicesToLookup(point.x, point.y, &bucket_lookup);
    for (size_t idx : bucket_lookup) {
        for (size_t k : bucket_[idx]) {
            Vec2 v(point.x - pos_x_[k], point.y - pos_y_[k]);
            if (v.dot(v) < particle_radius_ * particle_radius_) {
                neighbor_list.push_back(k);
            }
        }
    }
    return neighbor_list;
}