#include <rt/lights/directional.h>
#include <core/scalar.h>
#include <core/vector.h>

namespace rt {

DirectionalLight::DirectionalLight(const Vector& direction, const RGBColor& color)
{
	this->direction = direction;
	this->color = color;
}

LightHit DirectionalLight::getLightHit(const Point& p) const {
	LightHit dHit;
	dHit.distance = FLT_MAX;
	dHit.direction = -direction;
	dHit.normal = dHit.direction.normalize();
	return dHit;
}

RGBColor DirectionalLight::getIntensity(const LightHit& irr) const {
	return color;
}

}
