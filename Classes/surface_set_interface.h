#ifndef __SURFACE_SET_INTERFACE_SOLVER_INTERFACE_H__
#define __SURFACE_SET_INTERFACE_SOLVER_INTERFACE_H__

#include "cocos2d.h"

using cocos2d::Vec2;

struct IntersectionData2 {
    float t_;
    Vec2 point_;
    Vec2 normal_;
};

class SurfaceSetInterface {
public:
    virtual bool GetClosestIntersection(
        Vec2 point1,
        Vec2 point2,
        IntersectionData2* intersection) const = 0;
};

#endif 