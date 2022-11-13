#include <core/scalar.h>
#include <core/vector.h>
#include <core/point.h>
#include <core/float4.h>
#include <core/assert.h>
#include <algorithm>
#include <cmath>

namespace rt {

Vector::Vector(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector::Vector(const Float4& f4)
{
	if ((f4.w ==1)) throw rt::Assert() << "Assertion failure at Vector Float";
	this->x = f4.x ;
	this->y = f4.y;
	this->z = f4.z;
}

Vector Vector::operator + (const Vector& b) const {
	return Vector(this->x + b.x, this->y + b.y, this->z + b.z);
}

Vector Vector::operator - (const Vector& b) const {
	return Vector( this->x - b.x, this->y - b.y, this->z - b.z);
}

Vector Vector::operator - () const {
	return Vector(-(this->x), -(this->y), -(this->z));
}

Vector Vector::normalize() const {
	Vector normVector(0.0,0.0,0.0);
	float len = this->length();
	if (len > 0) {
		float invLen = 1 / len;
		normVector.x = this->x * invLen;
		normVector.y = this->y * invLen;
		normVector.z = this->z * invLen;
	}

	return normVector;
}

Vector operator * (float scalar, const Vector& b) {
	return Vector(scalar*b.x, scalar*b.y, scalar*b.z);
}

Vector operator * (const Vector& a, float scalar) {
	return Vector(scalar*a.x, scalar*a.y, scalar*a.z);
}

Vector operator / (const Vector& a, float scalar) {
	assert(scalar != 0);
	float divScalar = 1 / scalar;
	return Vector(divScalar*a.x, divScalar*a.y, divScalar*a.z);
}

Vector cross(const Vector& a, const Vector& b) {
	return Vector(
		(a.y * b.z) - (a.z * b.y),
		(a.z * b.x) - (a.x * b.z),
		(a.x * b.y) - (a.y * b.x));
}

float dot(const Vector& a, const Vector& b) {
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

float Vector::lensqr() const {
	return (sqr(this->x) + sqr(this->y) + sqr(this->z));
}

float Vector::length() const {
	return sqrt(lensqr());
}


bool Vector::operator == (const Vector& b) const {
	return ((fabs(this->x - b.x)< epsilon) && 
			(fabs(this->y - b.y)< epsilon) &&
			(fabs(this->z - b.z)< epsilon));
}

bool Vector::operator != (const Vector& b) const {
	return !((fabs(this->x - b.x) < epsilon) &&
		(fabs(this->y - b.y) < epsilon) &&
		(fabs(this->z - b.z) < epsilon));
}

Vector min(const Vector& a, const Vector& b) {
	return Vector(std::min(a.x, b.x), std::min(a.y, b.y),
		std::min(a.z, b.z));
}

Vector max(const Vector& a, const Vector& b) {
	return Vector(std::max(a.x, b.x), std::max(a.y, b.y),
		std::max(a.z, b.z));
}

Point operator + (const Point& a, const Vector& b) {
	return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}

Point operator + (const Vector& a, const Point& b) {
	return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}

Point operator - (const Point& a, const Vector& b) {
	return Point(a.x - b.x, a.y - b.y, a.z - b.z);
}

Point operator * (const Float4& scale, const Point& p) {
    /* TODO */ NOT_IMPLEMENTED;
}

}
