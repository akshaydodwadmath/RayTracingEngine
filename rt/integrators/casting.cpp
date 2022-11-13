#include <rt/integrators/casting.h>
#include <rt/world.h>

namespace rt {

RGBColor RayCastingIntegrator::getRadiance(const Ray& ray) const {
    Intersection intersec = world->scene->intersect(ray);
    float color = 0.0f;

    if (intersec) {
        color = dot(-intersec.ray.d, intersec.normal());
    }

    rt::RGBColor result = RGBColor(color,color,color);
    result.clamp();
    return result;
}

}
