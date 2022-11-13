#include <rt/solids/disc.h>
#include <rt/solids/infiniteplane.h>
#include <core/random.h>
#include <core/float4.h>
#include <core/matrix.h>
namespace rt {

Disc::Disc(const Point& center, const Vector& normal, float radius, CoordMapper* texMapper, Material* material)
{
    this->center = center;
    this->material = material;
    this->normal = normal.normalize();
    this->radius = radius;
	if (texMapper == nullptr) {
		this->texMapper = new WorldMapper(Vector::rep(1.0));
	}
	else {
		this->texMapper = texMapper;
	}
	minfPlane = new InfinitePlane(center, normal, this->texMapper, this->material);
}

BBox Disc::getBounds() const {
	Point bBoxMin(
		center.x - radius,
		center.y - radius,
		center.z - radius
	);
	Point bBoxMax(
		center.x + radius,
		center.y + radius,
		center.z + radius
	);
	return BBox(bBoxMin, bBoxMax);
}

void Disc::setMaterial(Material* m) {
	material = m;
}

Intersection Disc::intersect(const Ray& ray, float previousBestDistance) const {
    
    Intersection intersect = minfPlane->intersect(ray, previousBestDistance);
    
    if (intersect) {
        Vector v1 = ray.getPoint(intersect.distance) - this->center;
        if (v1.length() <= radius) {
            return Intersection(intersect.distance, ray, this, normal, ray.getPoint(intersect.distance));
        }
        return Intersection::failure();
    }
    return Intersection::failure();
}

Solid::Sample Disc::sample() const {
	
	float u = ( random());
	float v = 2 * pi * (random());
	float x = sqrt(u) * cos(v);
	float y = sqrt(u) * sin(v);
	Point randP( x,  y, 1);

	//creating a coordinate system
	float w =std::max(std::max(normal.x, normal.y), normal.z);
	Vector h;
	Vector i;
	if (w == normal.x) {
		h = Vector(0, -normal.z, normal.y);
		i = Vector(sqr(normal.y) + sqr(normal.z), -normal.x *normal.y, -normal.x *normal.z);
	}
	else if (w == normal.y) {
		h = Vector(0, -normal.x, normal.z);
		i = Vector(sqr(normal.z) + sqr(normal.x), -normal.y *normal.z, -normal.y *normal.x);
	}
	else {
		h = Vector(0, -normal.y, normal.x);
		i = Vector(sqr(normal.x) + sqr(normal.y), -normal.z *normal.x, -normal.z *normal.y);
	}

	Matrix transform = Matrix::system(h, i, normal);

	Point randP_tr = transform * randP;
	Point ret (center.x + randP_tr.x, center.y + randP_tr.y, center.z + randP_tr.z);
	
	return Sample{ ret, normal };

}

float Disc::getArea() const {
    return pi * radius * radius;
}

}
