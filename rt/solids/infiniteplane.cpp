#include <rt/solids/infiniteplane.h>

namespace rt {

InfinitePlane::InfinitePlane(const Point& origin, const Vector& normal, CoordMapper* texMapper, Material* material)
{
    this->origin = origin;
    this->normal = normal.normalize();
    this->material = material;
	if (texMapper == nullptr) {
		this->texMapper = new WorldMapper(Vector::rep(1.0));
	}
	else {
		this->texMapper = texMapper;
	}
}

BBox InfinitePlane::getBounds() const {
	BBox infBox;
	infBox = infBox.full();
	return infBox;
    /* TODO */ NOT_IMPLEMENTED;
}

void InfinitePlane::setMaterial(Material* m) {
	material = m;
}

Intersection InfinitePlane::intersect(const Ray& ray, float previousBestDistance) const {
    Vector v = this->origin - ray.o;
    v.normalize();
    float denom = dot(ray.d, normal);
    if (denom == 0.0f) {
        return Intersection::failure();
    }
    float t = dot(v,normal) / denom;

    if (t >= previousBestDistance || t <  0.0f) {
        return Intersection::failure();
    }

    return Intersection(t, ray, this, normal, ray.getPoint(t));
}

Solid::Sample InfinitePlane::sample() const {
    /* TODO */ NOT_IMPLEMENTED;
}

float InfinitePlane::getArea() const {
    return INFINITY;
}

}
