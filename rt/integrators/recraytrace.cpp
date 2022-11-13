#include <rt/integrators/recraytrace.h>
#include <rt/world.h>
#include <rt/solids/solid.h>
#include <rt/materials/material.h>
#include <rt/lights/light.h>
#include <core/random.h>
#include <rt/coordmappers/coordmapper.h>

const int setBounce = 6;

namespace rt {

RGBColor RecursiveRayTracingIntegrator::getRadiance(const Ray& ray) const {
	Intersection intersect = world->scene->intersect(ray);
	
	RGBColor color = RGBColor(0.0f, 0.0f, 0.0f);
	if (intersect) {
		const Solid* s = intersect.getSolid();
		Point texP = s->texMapper->getCoords(intersect);
		if (s->getMaterial()->useSampling() == Material::SAMPLING_NOT_NEEDED ||
			s->getMaterial()->useSampling() == Material::SAMPLING_SECONDARY) {
			for (Light* ls : world->light) {
				LightHit LocalHit = ls->getLightHit(intersect.hitPoint());

				// Tracing shadow ray
				Ray shadowRay;
				shadowRay.d = LocalHit.normal;
				shadowRay.o = intersect.hitPoint() + 0.001 * shadowRay.d;


				if (dot(LocalHit.normal, intersect.normal()) > 0) {
					Intersection intersect2 = world->scene->intersect(shadowRay, LocalHit.distance);
					if (intersect2) {
						color = color + RGBColor(0, 0, 0);
					}
					else {

					RGBColor L = ls->getIntensity(LocalHit);
					
					RGBColor matDiffusion = L * s->getMaterial()->getReflectance(texP,
						intersect.normal(), ray.d, LocalHit.normal);
					//if (s->getMaterial()->useSampling() == Material::SAMPLING_NOT_NEEDED)
					color = color + matDiffusion;
					}

				}
			}

			color = color + s->getMaterial()->getEmission(texP,
				intersect.normal(), ray.d);
		}
		if (s->getMaterial()->useSampling() == Material::SAMPLING_ALL ||
			s->getMaterial()->useSampling() == Material::SAMPLING_SECONDARY) {
			static int numBounces = setBounce;

			Material::SampleReflectance sR =
				s->getMaterial()->getSampleReflectance(intersect.local(), intersect.normal(), ray.d);
			//Trace secondary Ray
			Ray secRay;
			secRay.d = sR.direction.normalize();
			secRay.o = intersect.hitPoint() + 0.001 * secRay.d;
			//Intersection intersect3 = world->scene->intersect(secRay);
			numBounces--;
			if (numBounces > 0) {
				color = color + sR.reflectance *getRadiance(secRay);
			}
			else {
				color = color + RGBColor::rep(0.0f);
				
			}
			numBounces = setBounce;
			/*Ray refractRay;
			//Trace refraction ray
			RGBColor addcolor;
			float num = random();
			if (num <0.6) {
				refractRay.d = sR.rdirection.normalize();
				//refractRay.o = intersect.hitPoint() + 0.001 * refractRay.d;
				refractRay.o = outside ? intersect.hitPoint() - bias : intersect.hitPoint() + bias;
				//Intersection intersectrefract = world->scene->intersect(refractRay);
				float refra = 1 - sR.reflectance.r;
				addcolor = RGBColor::rep(refra)
					* getRadiance(refractRay);
			}
			else {
				//Trace reflection ray
				Ray reflectRay;
				reflectRay.d = sR.direction.normalize();
				reflectRay.o = outside ? intersect.hitPoint() + bias : intersect.hitPoint() - bias;
				//reflectRay.o = intersect.hitPoint() + 0.001 * reflectRay.d;
				//Intersection intersectreflect = world->scene->intersect(reflectRay);
				addcolor = sR.reflectance * getRadiance(reflectRay);
			}
			//if (sR.reflectance.r < 1)
			//	color = color + (sR.reflectance * getRadiance(reflectRay))
			//	+ (RGBColor(1 - sR.reflectance.r, 1 - sR.reflectance.g, 1 - sR.reflectance.b)
			//		* getRadiance(refractRay));
			//else
				
				color = color + addcolor;*/

		}

	}
	return color;
}

}