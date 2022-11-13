#include <rt/lights/spotlight.h>
#include <core/julia.h>

namespace rt {

SpotLight::SpotLight(const Point& position, const Vector& direction, float angle, float power, const RGBColor& intensity)
{
	this->position = position;
	this->direction = direction;
	this->angle = angle;
	this->power = power;
	this->intensity = intensity;
}

LightHit SpotLight::getLightHit(const Point& p) const {
	LightHit P;
	P.direction = position - p;
	P.distance = P.direction.length();
	P.normal = P.direction.normalize();
	//TODO : normal?
	return P;
}

RGBColor SpotLight::getIntensity(const LightHit& irr) const {
	float costotalAngle = std::cos((angle));
	Vector PointD = -irr.normal;
	Vector SpotD = direction.normalize();
	float pointAngle = dot(PointD, SpotD);

	if (pointAngle >= costotalAngle)
		return RGBColor(intensity * std::pow(pointAngle, power) * (1 / sqr(irr.distance)));
		
	else
	{
		return RGBColor(0, 0, 0);
	};
}

JuliaLight::JuliaLight(const Point& position, const RGBColor& intensity)
{
	this->position = position;
	this->direction = direction;
	this->angle = angle;
	this->power = power;
	this->intensity = intensity;
}

LightHit JuliaLight::getLightHit(const Point& p) const {
	LightHit P;
	P.direction = position - p;
	P.distance = P.direction.length();
	P.normal = P.direction.normalize();
	return P;
}

float a1computeWeight(float fx, float fy, float fz, const Point& c, float div) {
	Point v(fx, fy, 0.5f);
	v = v - Vector::rep(0.5f);
	v = v * 2.0f;
	int numIter = julia(v, c);
	return numIter / (numIter + div);
}


RGBColor a1computeColor(uint x, uint y, uint z,uint width, uint height) {
	float fx = float(x) / float(width);
	float fy = float(y) / float(height);
	float fz = float(z) / float(height);
	RGBColor color = RGBColor::rep(0.0f);
	color = color + a1computeWeight(fx, fy, fz, Point(-0.8f, 0.156f, 0.0f), 64.0f) * RGBColor(0.8f, 0.8f, 1.0f);
	color = color + a1computeWeight(fx, fy, fz, Point(-0.6f, 0.2f, 0.0f), 64.0f)*0.2f * RGBColor(0.5f, 0.5f, -0.2f);
	color = color + a1computeWeight(fy, fx, fz, Point(0.285f, 0.0f, 0.0f), 64.0f) * RGBColor(0.2f, 0.3f, 0.4f);
	return RGBColor::rep(1.0f) - color;
}

RGBColor JuliaLight::getIntensity(const LightHit& irr) const {
	Vector direction = irr.normal;
	return RGBColor(intensity *a1computeColor(direction.x, direction.y, 
		 direction.z, 1, 1));
}



}
