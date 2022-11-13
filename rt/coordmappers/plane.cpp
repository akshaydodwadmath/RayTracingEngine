#include <rt/coordmappers/plane.h>
#include <rt/intersection.h>
#include <core/matrix.h>
namespace rt {

PlaneCoordMapper::PlaneCoordMapper(const Vector& e1, const Vector& e2) {
	p1 = e1;
	p2 = e2;
}
/*
void translatePoint(Matrix tl, Point &p) {
	float xp = tl[0][0] * p.x + tl[0][1] * p.y + tl[0][2] * p.z + tl[0][3];
	float yp = tl[1][0] * p.x + tl[1][1] * p.y + tl[1][2] * p.z + tl[1][3];
	float zp = tl[2][0] * p.x + tl[2][1] * p.y + tl[2][2] * p.z + tl[2][3];
	float wp = tl[3][0] * p.x + tl[3][1] * p.y + tl[3][2] * p.z + tl[3][3];

	Point tr(xp, yp, zp);
	Point tr2(xp / wp, yp / wp, zp / wp);

	if (wp == 1)
		p = tr;
	else
		p = tr2;
}*/
Point PlaneCoordMapper::getCoords(const Intersection& hit) const {
	
	/*Vector normal = cross(p1, p2).normalize();
	//Point Ipoint(normal.x, normal.y, normal.z);
	Vector v(hit.local().x , 
		hit.local().y ,hit.local().z );

	float dist = dot(v, normal);

	Point ret = hit.local() - dist * normal;*/
	
	/*std::cout << "p1 1 x\n" << p1.x << std::endl;
	std::cout << "p1 2 y\n" << p1.y << std::endl;
	std::cout << "p1 3 z\n" << p1.z << std::endl;
	std::cout << "p2 1 x\n" << p2.x << std::endl;
	std::cout << "p2 2 y\n" << p2.y << std::endl;
	std::cout << "p2 3 z\n" << p2.z << std::endl;
	std::cout << "dist1" << dist << std::endl;
	std::cout << "dist2" << dot(v, normal) << std::endl;
	std::cout << "normal 1 x\n" << normal.x << std::endl;
	std::cout << "normal 2 y\n" << normal.y << std::endl;
	std::cout << "normal 3 z\n" << normal.z << std::endl;
	std::cout << "local 1 x\n" << hit.local().x << std::endl;
	std::cout << "local 2 y\n" << hit.local().y << std::endl;
	std::cout << "local 3 z\n" << hit.local().z << std::endl;*/


	//return ret;
	
	/*
	Float4 f1(hit.local());
	Float4 f2(p1);
	Float4 f3(p2);
	Float4 f4(cross(p1, p2));
	return Point(dot(f1, f2), dot(f1, f3), dot(f1, f4));*/
	/*Vector v2(hit.local().x, hit.local().y, hit.local().z);
	return Point(dot(p1, v2),
		dot(p2, v2),
		dot(v2, cross(p1,p2)));*/
		/*
	Matrix k1(Float4(1.0f, 0.0f, 0.0f, p1.x),
		Float4(0, 1, 0, p1.y),
		Float4(0, 0, 1, p1.z),
		Float4(0, 0, 0, 1));*/
	//Point x1 = hit.local();

	Vector normal = cross(p1, p2);
	Matrix transform = Matrix::system(p1, p2, normal);

	Point ret = transform.invert() * hit.local();

	return ret;
}

}