#include <core/point.h>
#include <core/float4.h>
#include <core/scalar.h>
#include <core/assert.h>
#include <core/vector.h>

namespace rt {

Point::Point(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
    /* TODO */
}

Point::Point(const Float4& f4)
{
	if ((f4.w == 0)) throw rt::Assert() << "Assertion failure at Vector Float";
	this->x = f4.x / f4.w;
	this->y = f4.y / f4.w;
	this->z = f4.z / f4.w;
}

Vector Point::operator - (const Point& b) const {
	return Vector(this->x - b.x, this->y - b.y, this->z - b.z);
}

bool Point::operator == (const Point& b) const {
	return ((fabs(this->x - b.x) < epsilon) &&
		(fabs(this->y - b.y) < epsilon) &&
		(fabs(this->z - b.z) < epsilon));
}

bool Point::operator != (const Point& b) const {
	return !((fabs(this->x - b.x) < epsilon) &&
		(fabs(this->y - b.y) < epsilon) &&
		(fabs(this->z - b.z) < epsilon));
}

Point operator * (float scalar, const Point& b) {
	return Point(scalar*b.x, scalar*b.y, scalar*b.z);
}

Point operator * (const Point& a, float scalar) {
	return Point(scalar*a.x, scalar*a.y, scalar*a.z);
}

Point min(const Point& a, const Point& b) {
	return Point(std::min(a.x, b.x), std::min(a.y, b.y),
		std::min(a.z, b.z));
}

Point max(const Point& a, const Point& b) {
	return Point(std::max(a.x, b.x), std::max(a.y, b.y),
		std::max(a.z, b.z));
}

}
