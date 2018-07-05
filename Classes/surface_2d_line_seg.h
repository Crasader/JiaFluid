#ifndef __SURFACE_2D_LINE_SEG_H__
#define __SURFACE_2D_LINE_SEG_H__

#include "surface_2d_interface.h"

class Surface2DLineSeg : public Surface2DInterface {
public:
    Vec2 GetClosestPoint(const Vec2& point) const override;
    virtual bool GetClosestIntersection(
        Vec2 point1,
        Vec2 point2,
        IntersectionData2* intersection) const override;

private:
    Vec2 point_a_;
    Vec2 point_b_;
    Vec2 normal_; // always normalized
};

#endif