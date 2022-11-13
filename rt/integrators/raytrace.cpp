#include <rt/integrators/raytrace.h>
#include <rt/world.h>
#include <rt/solids/solid.h>
#include <rt/materials/material.h>
#include <rt/coordmappers/coordmapper.h>
namespace rt {

	RGBColor RayTracingIntegrator::getRadiance(const Ray& ray) const {

		Intersection intersect = world->scene->intersect(ray);
		RGBColor color = RGBColor(0.0f, 0.0f, 0.0f);
		const Solid* s = intersect.getSolid();
	
		if (intersect) {
			CoordMapper *cM = s->getCoordMapper();
			Point texP = cM->getCoords(intersect);
			for (Light* ls : world->light) {
				LightHit LocalHit = ls->getLightHit(intersect.hitPoint());
					
				// Tracing shadow ray
				Ray shadowRay;
				static bool flag = false;
	
				shadowRay.d = LocalHit.normal;
				shadowRay.o = intersect.hitPoint() +  0.001 * shadowRay.d;
				if (dot(LocalHit.normal, intersect.normal()) > 0) {
					//Some prob with shadows of directional light, avoiding for now
					if (LocalHit.distance == FLT_MAX)
						LocalHit.distance = 0;
					Intersection intersect2 = world->scene->intersect(shadowRay, LocalHit.distance);
					if (intersect2) {
						color = color + RGBColor(0,0,0);
					}
					else {
						RGBColor L = ls->getIntensity(LocalHit);
	
						RGBColor matDiffusion =  L * s->getMaterial()->getReflectance(texP,
							intersect.normal(), ray.d, LocalHit.normal);
						color = color  + matDiffusion;
					}
				}
			}
			color = color + s->getMaterial()->getEmission(texP,
				intersect.normal(), ray.d);
		}
		return color;
	}

}