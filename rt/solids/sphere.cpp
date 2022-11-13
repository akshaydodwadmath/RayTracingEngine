#include <rt/solids/sphere.h>

namespace rt {

Sphere::Sphere(const Point& center, float radius, CoordMapper* texMapper, Material* material)
{
    this->center = center;
    this->radius = radius;
	
	if (texMapper == nullptr)
		this->texMapper = new WorldMapper(Vector::rep(1.0));
	else
		this->texMapper = texMapper;
    this->material = material;
}

BBox Sphere::getBounds() const {
	Point bBoxMin(
		this->center.x - this->radius,
		this->center.y - this->radius,
		this->center.z - this->radius
	);
	Point bBoxMax(
		this->center.x + this->radius,
		this->center.y + this->radius,
		this->center.z + this->radius
	);
	return BBox(bBoxMin, bBoxMax);
}

void Sphere::setMaterial(Material* m) {
	material = m;
}

Intersection Sphere::intersect(const Ray& ray, float previousBestDistance) const {
	//std::cout << "Intersecting::\n" << previousBestDistance << std::endl;
    rt::Vector p = ray.o - this->center;
    float x0, x1;
    float a = dot(ray.d, ray.d);
    float b = 2 * dot(ray.d, p);
    float c = dot(p, p) - radius * radius;
    float discr = b * b - 4 * a * c;
    if (discr < 0) return Intersection::failure();
    else if (discr == 0) x0 = x1 = -0.5 * b / a;
    else {
        float q = (b > 0) ?
            -0.5 * (b + sqrt(discr)) :
            -0.5 * (b - sqrt(discr));
        x0 = q / a;
        x1 = c / q;
    }
    if (x0 > x1) std::swap(x0, x1);
    if (x0 < 0) {
        x0 = x1; 
        if (x0 < 0) return Intersection::failure(); 
    }
    rt::Point hitPoint = ray.getPoint(x0);
    rt::Vector normalVec = hitPoint - this->center;
    normalVec = normalVec.normalize();
    if (x0 >= previousBestDistance) {
        return Intersection::failure();
    }
    return Intersection(x0, ray, this, normalVec, hitPoint);
}

Solid::Sample Sphere::sample() const {
	NOT_IMPLEMENTED;
}

float Sphere::getArea() const {
    return 4 * pi * this->radius * this->radius;
}

}
