#include <rt/materials/dummy.h>
#include <core/assert.h>
#include <rt/intersection.h>
namespace rt {

DummyMaterial::DummyMaterial() {
    /* TODO */
}

RGBColor DummyMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
	//fr =1;
	float costheta = std::max(0.0f, (dot(normal, inDir)));
	return RGBColor(costheta, costheta, costheta);
}

RGBColor DummyMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
	return RGBColor(0.0, 0.0, 0.0);
}

Material::SampleReflectance DummyMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
	return SampleReflectance(Vector(0, 0, 0), RGBColor::rep(0));
}

}