#include <rt/materials/phong.h>
#include <math.h>
#include <core/assert.h>
#include <core/assert.h>
#include <rt/intersection.h>
namespace rt {

PhongMaterial::PhongMaterial(Texture* specular, float exponent)
{
	mSpecular = specular;
	mExponent = exponent;
}

RGBColor PhongMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
	Vector reflectDir = (2 * (dot(normal, inDir) * normal)) - inDir;
	//Vector reflectDir = inDir - (2 * dot(normal, inDir) * normal);
	float spec = std::max(0.0f, dot(-outDir, reflectDir));
	float spec_pow = pow(spec, mExponent);
	return RGBColor(mSpecular->getColor(texPoint).r * spec_pow,
		mSpecular->getColor(texPoint).g * spec_pow,
		mSpecular->getColor(texPoint).b * spec_pow);
}

RGBColor PhongMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
	return RGBColor(0.0, 0.0, 0.0);
}

Material::SampleReflectance PhongMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
	UNREACHABLE;
}

Material::Sampling PhongMaterial::useSampling() const {
	return SAMPLING_NOT_NEEDED;
}

}