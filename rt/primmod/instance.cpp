#include <rt/primmod/instance.h>

namespace rt {

Instance::Instance(Primitive* content)
{
	pContent = content;
	trM = trM.identity();
	trIM = trIM.identity();
	roM = roM.identity();
	roIM = roIM.identity();
	scM = roM.identity();
	scIM = roIM.identity();
}

Primitive* Instance::content() {
	return pContent;
}

void Instance::reset() {
	trM = trM.identity();
	trIM = trIM.identity();
	roM = roM.identity();
	roIM = roIM.identity();
	scM = roM.identity();
	scIM = roIM.identity();
}

void Instance::translate(const Vector& t) {
	Matrix k1(Float4(1.0f, 0.0f, 0.0f, t.x),
	Float4(0, 1, 0, t.y),
	Float4(0, 0, 1, t.z),
	Float4(0, 0, 0, 1));
	
	Matrix k2(Float4(1, 0, 0, -t.x),
	Float4(0, 1, 0, -t.y),
	Float4(0, 0, 1, -t.z),
	Float4(0, 0, 0, 1));

	trM = product(trM,k1);
	trIM = product(trIM,k2);
}

void Instance::rotate(const Vector& nnaxis, float angle) {
	Vector a = nnaxis.normalize();
	float sinTheta = std::sin( (angle));
	float cosTheta = std::cos( (angle));
	Matrix newM;
	newM = newM.identity();

	newM.r1.x = newM[0][0] = a.x * a.x + (1 - a.x * a.x) * cosTheta;
	newM.r1.y = newM[0][1] = a.x * a.y * (1 - cosTheta) - a.z * sinTheta;
	newM.r1.z = newM[0][2] = a.x * a.z * (1 - cosTheta) + a.y * sinTheta;
	newM.r1.w = newM[0][3] = 0;

	newM.r2.x = newM[1][0] = a.x * a.y * (1 - cosTheta) + a.z * sinTheta;
	newM.r2.y = newM[1][1] = a.y * a.y + (1 - a.y * a.y) * cosTheta;
	newM.r2.z = newM[1][2] = a.y * a.z * (1 - cosTheta) - a.x * sinTheta;
	newM.r2.w = newM[1][3] = 0;

	newM.r3.x = newM[2][0] = a.x * a.z * (1 - cosTheta) - a.y * sinTheta;
	newM.r3.y = newM[2][1] = a.y * a.z * (1 - cosTheta) + a.x * sinTheta;
	newM.r3.z = newM[2][2] = a.z * a.z + (1 - a.z * a.z) * cosTheta;
	newM.r3.w = newM[2][3] = 0;

	newM.r4.x = newM[3][0] = 0;
	newM.r4.y = newM[3][1] = 0;
	newM.r4.z = newM[3][2] = 0;
	newM.r4.w = newM[3][3] = 1;

	roM = product(roM,(newM));
	roIM = product(roIM,roM.transpose());
}

void Instance::scale(float f) {
	Matrix sm(Float4(f, 0, 0, 0),
		Float4(0, f, 0, 0),
		Float4(0, 0, f, 0),
		Float4(0, 0, 0, 1));
	Matrix sminv(Float4(1 / f, 0, 0, 0),
		Float4(0, 1 / f, 0, 0),
		Float4(0, 0, 1 / f, 0),
		Float4(0, 0, 0, 1));
	scM = product(scM,sm);
	scIM = product(scIM,sminv);
}

void Instance::scale(const Vector& s) {
	Matrix sm(Float4(s.x, 0, 0, 0),
		Float4(0, s.y, 0, 0),
		Float4(0, 0, s.z, 0),
		Float4(0, 0, 0, 1));
	Matrix sminv(Float4(1 / s.x, 0, 0, 0),
		Float4(0, 1 / s.y, 0, 0),
		Float4(0, 0, 1 / s.z, 0),
		Float4(0, 0, 0, 1));

	scM = product(scM, sm);
	scIM = product(scIM, sminv);
}

void Instance::setMaterial(Material* m) {
    /* TODO */ NOT_IMPLEMENTED;
}

void Instance::setCoordMapper(CoordMapper* cm) {
    /* TODO */ NOT_IMPLEMENTED;
}

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
}

void translateVector(Matrix t1, Vector &v) {
	Vector tv (t1[0][0] * v.x + t1[0][1] * v.y + t1[0][2] * v.z,
		t1[1][0] * v.x + t1[1][1] * v.y + t1[1][2] * v.z,
		t1[2][0] * v.x + t1[2][1] * v.y + t1[2][2] * v.z);
	v = tv;
}

Intersection Instance::intersect(const Ray& ray, float previousBestDistance) const {
	static bool flag = true;
	Ray newRay(ray.o, ray.d);
	
	translatePoint(trIM, newRay.o);
	translatePoint(roIM, newRay.o);
	translatePoint(scIM, newRay.o);
	
	translateVector(roIM, newRay.d);
	translateVector(scIM, newRay.d);
	newRay.d = newRay.d.normalize();

	if (flag) {
		std::cout << "previousBestDistance s\n" << previousBestDistance << std::endl;
		std::cout << "transfered origin " << newRay.d.x << std::endl;
		std::cout << "transfered origin " << newRay.d.y << std::endl;
		std::cout << "scIM.det() " << scIM[0][0] * scIM[1][1]
			* scIM[2][2] * 50 << std::endl;
	}

	previousBestDistance = (scIM * (ray.getPoint(previousBestDistance) - ray.o)).length();
	Intersection intersectTemp = pContent->intersect(newRay, previousBestDistance);
	if (flag) {
		std::cout << "previousBestDistance 2\n" << previousBestDistance << std::endl;
		flag = false;
	}
	translatePoint(trM, intersectTemp.ray.o);
	translatePoint(roM, intersectTemp.ray.o);
	translatePoint(scM, intersectTemp.ray.o);

	translateVector(roM, intersectTemp.ray.d);
	translateVector(scM, intersectTemp.ray.d);
	intersectTemp.ray.d = intersectTemp.ray.d.normalize();

	translateVector(roM, intersectTemp.normalVec);
	translateVector(scM, intersectTemp.normalVec);
	intersectTemp.normalVec = intersectTemp.normalVec.normalize();

	intersectTemp.distance = (scM * (newRay.getPoint(intersectTemp.distance) - newRay.o)).length();

	return intersectTemp;
}

BBox Instance::getBounds() const {
	BBox fullGroupBox;
	fullGroupBox = fullGroupBox.empty();
		BBox primBox = pContent->getBounds();
		translatePoint(product( trM, scM), primBox.min);
		translatePoint(product( trM, scM), primBox.max);
		fullGroupBox.extend(primBox);
	return fullGroupBox;

}

}