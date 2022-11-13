#include <rt/intersection.h>

namespace rt {

    Intersection::Intersection(float distance, const Ray& ray, const Solid* solid, const Vector& normal, const Point& local)
    {
        this->distance = distance;
        this->ray = ray;
        this->solid = solid;
        this->normalVec = normal;
        this->localPoint = local;
        this->boolFailure = true;
    }

    Intersection::Intersection(bool failure)
    {
        this->boolFailure = failure;
    }

    Intersection::operator bool() {
        return this->boolFailure;
    }

    Intersection Intersection::failure() {
        return Intersection(false);
    }

    Point Intersection::hitPoint() const {
        return ray.getPoint(distance);
    }

    Vector Intersection::normal() const {
        return this->normalVec;
    }

    Point Intersection::local() const {
        return this->localPoint;
    }
	const Solid* Intersection::getSolid() const  { 
		return this->solid;
	};
}