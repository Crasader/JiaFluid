#ifndef __SURFACE_2D_INTERFACE_H__
#define __SURFACE_2D_INTERFACE_H__

#include "cocos2d.h"
#include "surface_set_interface.h"

using cocos2d::Vec2;

class Surface2DInterface {
public:

    virtual Vec2 GetClosestPoint(const Vec2& point) const = 0;
    virtual bool GetClosestIntersection(
        Vec2 point1,
        Vec2 point2,
        IntersectionData2* intersection) const = 0;
};

#endif