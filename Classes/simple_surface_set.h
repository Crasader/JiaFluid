#ifndef __SIMPLE_SURFACE_SET_INTERFACE_SOLVER_INTERFACE_H__
#define __SIMPLE_SURFACE_SET_INTERFACE_SOLVER_INTERFACE_H__

#include "surface_set_interface.h"
#include "surface_2d_interface.h"

#include <vector>

class SimpleSurfaceSet : public SurfaceSetInterface {
public:
    bool GetClosestIntersection(
        Vec2 point1,
        Vec2 point2,
        IntersectionData2* intersection) const override;

private:
    std::vector<std::unique_ptr<Surface2DInterface>> surfaces_;
};

#endif