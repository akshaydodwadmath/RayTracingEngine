#ifndef CG1RAYTRACER_INTEGRATORS_CASTINGDIST_HEADER
#define CG1RAYTRACER_INTEGRATORS_CASTINGDIST_HEADER

#include <rt/integrators/integrator.h>

namespace rt {

class RayCastingDistIntegrator : public Integrator {
public:

    World* world;
    RGBColor nearColor;
    float nearDist;
    RGBColor farColor;
    float farDist;

    RayCastingDistIntegrator(World* world, const RGBColor& nearColor, float nearDist, const RGBColor& farColor, float farDist);
    virtual RGBColor getRadiance(const Ray& ray) const;
};

}

#endif