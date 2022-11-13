#include <rt/bbox.h>

namespace rt {

BBox BBox::empty() {
	BBox emptyBox;
	float boxmin = -FLT_MAX;
	float boxmax = FLT_MAX;
	Point pmin(boxmax, boxmax, boxmax);
	Point pmax(boxmin, boxmin, boxmin);
	emptyBox.min = pmin;
	emptyBox.max = pmax;
	return  emptyBox;
}

BBox BBox::full() {
	return BBox(Point(-FLT_MAX, -FLT_MAX, -FLT_MAX), Point(FLT_MAX, FLT_MAX, FLT_MAX));
}

void BBox::extend(const Point& point) {
	Point extendmin(std::min(this->min.x, point.x),
	std::min(this->min.y, point.y),
		std::min(this->min.z, point.z));
		Point extendmax(std::max(this->max.x, point.x),
			std::max(this->max.y, point.y),
			std::max(this->max.z, point.z));
		this->min = extendmin;
		this->max = extendmax;
}

void BBox::extend(const BBox& bbox) {
	Point extendmin(std::min(this->min.x, bbox.min.x),
		std::min(this->min.y, bbox.min.y),
		std::min(this->min.z, bbox.min.z));
	Point extendmax(std::max(this->max.x, bbox.max.x),
		std::max(this->max.y, bbox.max.y),
		std::max(this->max.z, bbox.max.z));
	this->min = extendmin;
	this->max = extendmax;
}

std::pair<float, float> BBox::intersect(const Ray& ray) const {

	float tmin = (min.x - ray.o.x) / ray.d.x;
	float tmax = (max.x - ray.o.x) / ray.d.x;

	if (tmin > tmax) std::swap(tmin, tmax);

	float tymin = (min.y - ray.o.y) / ray.d.y;
	float tymax = (max.y - ray.o.y) / ray.d.y;

	if (tymin > tymax) std::swap(tymin, tymax);

	if ((tmin > tymax) || (tymin > tmax)) {
		//std::cout << "missed at 1\n" << std::endl;
		return std::make_pair(1, 0); //missed
	}

	if (tymin > tmin)
		tmin = tymin;

	if (tymax < tmax)
		tmax = tymax;

	float tzmin = (min.z - ray.o.z) / ray.d.z;
	float tzmax = (max.z - ray.o.z) / ray.d.z;

	if (tzmin > tzmax) std::swap(tzmin, tzmax);

	if ((tmin > tzmax) || (tzmin > tmax)) {
		//std::cout << "missed at 2\n" << std::endl;
		return std::make_pair(1, 0); //missed
	}

	if (tzmin > tmin)
		tmin = tzmin;

	if (tzmax < tmax)
		tmax = tzmax;

	return std::make_pair(tmin, tmax);
}

bool BBox::isUnbound() {
	return ((this->min.x <= -FLT_MAX) ||
		(this->min.y <= -FLT_MAX) ||
		(this->min.z <= -FLT_MAX) ||
		(this->max.x >= FLT_MAX) ||
		(this->max.y >= FLT_MAX) ||
		(this->max.z >= FLT_MAX));

}

}