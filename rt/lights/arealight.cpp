#include <rt/lights/arealight.h>

#include <core/color.h>
#include <rt/materials/material.h>
namespace rt {

LightHit AreaLight::getLightHit(const Point& p) const {
	LightHit P;
	Point position = mSolid->sample().point + 0.001 * mSolid->sample().normal;

	P.direction = position - p;
	P.distance = P.direction.length();
	P.normal = P.direction.normalize();
	return P;
}

RGBColor AreaLight::getIntensity(const LightHit& irr) const {
	float costotalAngle = dot(irr.normal, -mSolid->sample().normal);

	RGBColor intensity = mSolid->getMaterial()->getEmission(Point::rep(0.0f), Vector::rep(0.0f), Vector::rep(0.0f));
	return intensity * (1 / sqr(irr.distance)) * mSolid->getArea()
		* std::abs(costotalAngle);
}

AreaLight::AreaLight(Solid* source)
{
	mSolid = source;
    /* TODO */
}

}