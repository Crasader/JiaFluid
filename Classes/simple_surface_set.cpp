#include "simple_surface_set.h"

bool SimpleSurfaceSet::GetClosestIntersection(
    Vec2 point1,
    Vec2 point2,
    IntersectionData2* intersection) const {

    float closest_distance = std::numeric_limits<float>::max();
    Vec2 surface_normal;
    Vec2 point_on_surface;
    int surface_index = -1;
    float t = 0;

    for (int i = 0; i < surfaces_.size(); i++) {
        IntersectionData2 intersection;
        if (surfaces_[i]->GetClosestIntersection(point1, point2, &intersection)) {
            const float distance = (intersection.point_ - point1).length();
            if (distance < closest_distance) {
                closest_distance = distance;
                point_on_surface = intersection.point_;
                surface_normal = intersection.normal_;
                t = intersection.t_;
                surface_index = i;
            }
        }
    }

    if (surface_index != -1) {
        intersection->point_ = point_on_surface;
        intersection->normal_ = surface_normal;
        intersection->t_ = t;
        return true;
    }

    return false;
}