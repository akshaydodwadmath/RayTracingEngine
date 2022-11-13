#include <rt/cameras/perspective.h>
#include <cmath>

namespace rt {

PerspectiveCamera::PerspectiveCamera(const Point& center, const Vector& forward, const Vector& up, float verticalOpeningAngle, float horizontalOpeningAngle)
{
    this->center = center;
    this->forward = forward.normalize();
    this->up = up.normalize();
    this->verticalOpeningAngle = verticalOpeningAngle;
    this->horizontalOpeningAngle = horizontalOpeningAngle;
}

Ray PerspectiveCamera::getPrimaryRay(float x, float y) const {
    float pcx = x * tanf(this->horizontalOpeningAngle/ 2.f);
    float pcy = y * tanf(this->verticalOpeningAngle / 2.f);
    rt::Vector u = cross(this->forward, up);
    u = u.normalize();
    rt::Vector v = cross(u, this->forward);
    v = v.normalize();
    rt::Vector d = this->forward + (pcx * u) + (pcy * v);
    d = d.normalize();
    Ray primaryRay = Ray(this->center, d);
    return primaryRay;
}

}
