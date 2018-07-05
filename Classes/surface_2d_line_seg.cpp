#include "surface_2d_line_seg.h"

Vec2 Surface2DLineSeg::GetClosestPoint(const Vec2& point) const {
    Vec2 v0 = point - point_a_;
    Vec2 v1 = point_b_ - point_a_;
    double t = v0.dot(v1) / v1.dot(v1); // t: parameter of projected point on the line
    if (t > 1)
        return point_b_;
    else if (t < 0)
        return point_a_;

    return point_a_ + v1 * t;
}

bool Surface2DLineSeg::GetClosestIntersection(
    Vec2 point1,
    Vec2 point2,
    IntersectionData2* intersection) const {

    //fast rejection
    const Vec2 vA = point_a_ - point1;
    const Vec2 vB = point_b_ - point1;
    const Vec2 ray = point2 - point1;
    const float delta = vA.cross(ray) * vB.cross(ray);
    if (delta > 0)
        return false;

    float s; // s: parameter on the line
    float t; // t: parameter on the ray
    float A, B, C, D; // equation of ray: x=A+Bt, y=C+Dt
    float E, F, G, H; // equation of line: x=E+Fs, y=G+Hs

    E = point_a_.x;
    G = point_a_.y;
    F = point_b_.x - point_a_.x;
    H = point_b_.y - point_a_.y;

    A = point1.x;
    C = point1.y;
    B = ray.x;
    D = ray.y;

    float det = B * H - D * F;
    if (std::abs(det) < 1e-6) return false;
    t = (H * (E - A) - F * (G - C)) / det;
    s = (D * (E - A) - B * (G - C)) / det;

    constexpr float tmin = 0.0;
    constexpr float tmax = 1.0;

    if (t < tmin || t > tmax) return false;
    if (s < 0 || s > 1) return false;
    intersection->t_ = t;
    intersection->point_ = point1 + ray * t;
    //data.point = _pointA + (_pointB - _pointA) * s;
    intersection->normal_ = normal_;

    Vec2 vO = point1 - intersection->point_;
    if (vO.dot(normal_) < 0) //normal should be the same direction as origin of ray
        intersection->normal_ = -normal_;

    return true;
}