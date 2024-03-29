#include <core/assert.h>
#include <core/macros.h>
#include <rt/ray.h>

namespace rt {

Ray::Ray(const Point& o, const Vector& d)
{
    assert(d != Vector::rep(0.0f));
    this->d = d;
    this->o = o;
}

Point Ray::getPoint(float distance) const {
    return o + distance * d;
}

}
