#include <rt/solids/striangle.h>
#include <rt/intersection.h>
#include <core/interpolate.h>
namespace rt {

SmoothTriangle::SmoothTriangle(Point vertices[3], Vector normals[3], CoordMapper* texMapper, Material* material)
		: Triangle(vertices, texMapper, material)
{
		this->mNormals[0] = normals[0];
		this->mNormals[1] = normals[1];
		this->mNormals[2] = normals[2];
}

SmoothTriangle::SmoothTriangle(const Point& v1, const Point& v2, const Point& v3, const Vector& n1, const Vector& n2, const Vector& n3, CoordMapper* texMapper, Material* material)
	: Triangle(v1, v2, v3, texMapper, material)
{
	this->mNormals[0] = n1;
	this->mNormals[1] = n2;
	this->mNormals[2] = n3;
}

Intersection SmoothTriangle::intersect(const Ray& ray, float previousBestDistance) const {
	Intersection interTemp = Triangle::intersect(ray, previousBestDistance);
	
	if (interTemp) {
		Point localPoint = interTemp.local();
		Vector newNormal = lerpbar(mNormals[0], mNormals[1], mNormals[2], localPoint.x, localPoint.y);
		return Intersection(interTemp.distance, ray, this, newNormal, localPoint);
	}
	return interTemp;
    /* TODO */ NOT_IMPLEMENTED;
}

}