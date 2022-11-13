#include <rt/cameras/orthographic.h>
#include <rt/ray.h>


namespace rt {

OrthographicCamera::OrthographicCamera(const Point& center, const Vector& forward, const Vector& up, float scaleX, float scaleY)
{
	this->forward = forward.normalize();
	this->up = up.normalize();
	this->center = center;
	this->scaleX = scaleX;
	this->scaleY = scaleY;
}

Ray OrthographicCamera::getPrimaryRay(float x, float y) const {
	rt::Vector u = cross(this->forward, up);
	u = u.normalize();
	rt::Vector v = cross(u, this->forward);
	v = v.normalize();
	Ray primaryRay;
	primaryRay.d = this->forward;
	primaryRay.o = this->center + (x * (u * (scaleX/2.0f)) + (y * (scaleY / 2.0f) * v));
	return primaryRay;

}

}
