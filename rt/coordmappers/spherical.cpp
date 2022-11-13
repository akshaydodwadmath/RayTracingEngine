#include <rt/coordmappers/spherical.h>
#include <rt/intersection.h>
namespace rt {

SphericalCoordMapper::SphericalCoordMapper(const Point& origin, const Vector& zenith, const Vector& azimuthRef)
{

	 mO = origin;
	 mZ = zenith;
	 mAR = azimuthRef;
}

Point SphericalCoordMapper::getCoords(const Intersection& hit) const {
	Point p = hit.local();
	
	float r = (p - mO).length();

	/*float piV = mZ.length();

	float twopiu = mAR.length();*/


	Vector Sp = mZ.normalize();
	Vector Se = mAR.normalize();
	Vector Sn = (p - mO).normalize();

	float phi = std::acos(dot(Sn, Sp));
	float v = phi / pi;
	float y = v / mZ.length();

	float theta = std::acos(dot(-Sn, Se) / sin(phi));
	float u; 
	
	if(dot(cross(Sp,Se), -Sn) > 0)
		u = theta / (2 * pi);
	else 
		u = (1 - theta) / (2 * pi);
	
	float x = u / mAR.length();

	float z = r;
//	std::sin(mZ);
	/*
	float ret1 = r.length();

	float pAngle = dot(r.normalize(), mZ.normalize());

	float ret2 = std::acos(pAngle);
	*/
	//return(Point(p.x * mAR.length(), p.y * mZ.length(),0.0f));
	//return Point(ret1, ret2, 0.0);

	/*float x = r * std::sin(piV) * std::cos(twopiu);
	float y = r * std::sin(piV) * std::sin(twopiu);
	float z = r * std::cos(piV);*/

	//float x = dot((p - mO).normalize(), mZ.normalize());
	//float y = dot((p - mO).normalize(), mAR.normalize());
	//float z = r;


	/*float x = (p.x/ twopiu - mO.x)  +r;
	float y = (p.y/piV - mO.y) + r;;
	float z = mO.z + r;
	*/

	/*std::cout << "point x\n" << p.x << std::endl;
	std::cout << "point y\n" << p.y << std::endl;
	std::cout << "point z\n" << p.z << std::endl;
	std::cout << "center x\n" << mO.x << std::endl;
	std::cout << "center y\n" << mO.y << std::endl;
	std::cout << "center z\n" << mO.z << std::endl;
	std::cout << "radius length\n" << r << std::endl;
	std::cout << "zenith\n" << piV << std::endl;
	std::cout << "zenithx\n" << mZ.x << std::endl;
	std::cout << "zenithy\n" << mZ.y << std::endl;
	std::cout << "zenithz\n" << mZ.	z << std::endl;
	std::cout << "azim\n" << twopiu << std::endl;
	std::cout << "azimx\n" << mAR.x << std::endl;
	std::cout << "azimy\n" << mAR.y << std::endl;
	std::cout << "azimz\n" << mAR.z << std::endl;*/

	return Point(x, y, z);
}

}