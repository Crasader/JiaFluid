#ifndef __SOLVER_INTERFACE_H__
#define __SOLVER_INTERFACE_H__

#include "particle_data_interface.h"
#include "surface_set_interface.h"

class SolverInterface {
public:
    virtual bool Solve(
        SurfaceSetInterface* surface_set, 
        ParticleDataInterface* particle_data) = 0;
};

#endif