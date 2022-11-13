#include <rt/coordmappers/cylindrical.h>
#include <rt/coordmappers/spherical.h>
#include <core/matrix.h>
namespace rt {

CylindricalCoordMapper::CylindricalCoordMapper(const Point& origin, const Vector& longitudinalAxis, const Vector& polarAxis)
{
	mO = origin;
	mL = longitudinalAxis;
	mP = polarAxis;
}

Point CylindricalCoordMapper::getCoords(const Intersection& hit) const {

	Point p = hit.local();

	float r = (p - mO).length();



	Vector Sp = mL.normalize();
	Vector Se = mP.normalize();
	Vector Sn = (p - mO);
/*
	float phi = std::acosdot(Sn, Sp);
	float v = phi / 2*pi;
	float x = v / mP.length();

	float v = std::acos(dot(Sn, Sp));
	float y = v/ mL.length();

	float u = std::acos(dot(-Sn, Se)) / sin(v);
	if (dot(cross(Sp, Se), -Sn) > 0)
		u = u / (2 * pi);
	else
		u = (1 - u) / (2 * pi);
	float x = u / mP.length();
	//if (p.z < 0)
		//u = 1 - u;

	//return Point(x, v, r);


	float a1 = sqrt(sqr(Sn.x) + sqr(Sn.y));
	
	float a2;
	if ((Sn.x) == 0 && (Sn.y == 0))
		a2 = 0;
	else if (Sn.x > 0)
		a2 = std::atan(Sn.y/ Sn.x);
	else if(Sn.x < 0)
		a2 = -std::asin(Sn.y / a1); +pi;

	float a3 = a1/ mL.length();
	a3 = a3 / (2 * pi);
	float a4 = a2/ mP.length();
	//return Point(a3, a4, r);*/


	SphericalCoordMapper s(mO, mL, mP);

	Point spher = s.getCoords(hit);
	float x1;
	float x3;
	if (spher.y > 0) {
		x1 = spher.z * std::cos(spher.y);
		x3 = spher.z * std::sin(spher.y);
	}
	else {
		x1 = spher.z * std::sin(spher.y);
		x3 = spher.z * std::cos(spher.y);
	}
	float x2 = spher.x;
	
	x1 = x1 * pi	;
	return Point(x2, x1, x3);
}

}