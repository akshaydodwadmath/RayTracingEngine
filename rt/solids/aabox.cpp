#include <rt/solids/aabox.h>

namespace rt {
	void compute(float ray_d, float ray_o, float lower, float upper, float& r1, float& r2);
	AABox::AABox(const Point& corner1, const Point& corner2, CoordMapper* texMapper, Material* material)
	{
		this->corner1 = rt::min(corner1, corner2);
		this->corner2 = rt::max(corner1, corner2);
		this->material = material;
		if (texMapper == nullptr) {
			this->texMapper = new WorldMapper(Vector::rep(1.0));
		}
		else {
			this->texMapper = texMapper;
		}
	}

	BBox AABox::getBounds() const {
		return BBox(corner1, corner2);
	}

	void AABox::setMaterial(Material* m) {
		material = m;
	}

	Solid::Sample AABox::sample() const {
		NOT_IMPLEMENTED;
	}

	float AABox::getArea() const {
		const Vector d = this->corner2 - this->corner1;
		return 2 * (d.x*d.y + d.x*d.z + d.y*d.z);
	}

	Intersection AABox::intersect(const Ray& ray, float previousBestDistance) const {
		Vector t1, t2;
		Point c1 = this->corner1;
		Point c2 = this->corner2;
		compute(ray.d.x, ray.o.x, c1.x, c2.x, t1.x, t2.x);
		compute(ray.d.y, ray.o.y, c1.y, c2.y, t1.y, t2.y);
		compute(ray.d.z, ray.o.z, c1.z, c2.z, t1.z, t2.z);

		const Vector tMin = rt::min(t1, t2);
		const Vector tMax = rt::max(t1, t2);

		const float fmin = rt::max(tMin.x, tMin.y, tMin.z);
		const float fmax = rt::min(tMax.x, tMax.y, tMax.z);

		if (fmin > fmax)
			return Intersection::failure();

		const float dist = fmin > 0 ? fmin : fmax;

		if (dist < 0 || dist > previousBestDistance)
			return Intersection::failure();

		Point hit = ray.getPoint(dist);
		Vector normal;
		if (fabsf(hit.x - corner1.x) < epsilon) normal = Vector(-1, 0, 0);
		else if (fabsf(hit.x - corner2.x) < epsilon) normal = Vector(1, 0, 0);
		else if (fabsf(hit.y - corner1.y) < epsilon) normal = Vector(0, -1, 0);
		else if (fabsf(hit.y - corner2.y) < epsilon) normal = Vector(0, 1, 0);
		else  if (fabsf(hit.z - corner1.z) < epsilon) normal = Vector(0, 0, -1);
		else if (fabsf(hit.z - corner2.z) < epsilon) normal = Vector(0, 0, 1);
		else UNREACHABLE;

		return Intersection(fmin, ray, this, normal, hit);
	}



	void compute(float ray_d, float ray_o, float lower, float upper, float& r1, float& r2) {
		const float inv = 1.0f / ray_d;
		r1 = (lower - ray_o) * inv;
		r2 = (upper - ray_o) * inv;
	}
}