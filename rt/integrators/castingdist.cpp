#include <rt/integrators/castingdist.h>
#include <rt/world.h>

namespace rt {

RayCastingDistIntegrator::RayCastingDistIntegrator(World* world, const RGBColor& nearColor, float nearDist, const RGBColor& farColor, float farDist)
    : Integrator(world)
{
    this->farColor = farColor;
    this->farDist = farDist;
    this->nearColor = nearColor;
    this->nearDist = nearDist;
    this->world = world;

}

RGBColor RayCastingDistIntegrator::getRadiance(const Ray& ray) const {
    Intersection intersect = world->scene->intersect(ray);
    RGBColor color = RGBColor(0.0f, 0.0f, 0.0f);
    if (intersect) {

        float x = (intersect.distance - nearDist) / (farDist - nearDist);
        if (x < 0) {
            color = nearColor;
        }
        else if (x > 1) {
            color = farColor;
        }
        else {
            color = ((1 - x) * nearColor) + (x * farColor);
        }
        color = color * (dot(-ray.d, intersect.normal()));
    }
    return color.clamp();
}

}
