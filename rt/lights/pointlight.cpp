#include <rt/lights/pointlight.h>
#include <core/scalar.h>
#include <core/vector.h>

namespace rt {

PointLight::PointLight(const Point& position, const RGBColor& intensity)
{
	this->position = position;
	this->intensity = intensity;
}

LightHit PointLight::getLightHit(const Point& p) const {
	LightHit P;
	P.direction = position - p;
	P.distance = P.direction.length();
	P.normal = P.direction.normalize();
	//TODO : normal?
	return P;
}

RGBColor PointLight::getIntensity(const LightHit& irr) const {

		return RGBColor(intensity* (1/sqr(irr.distance)));
}

}
