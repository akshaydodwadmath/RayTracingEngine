#include <rt/materials/lambertian.h>
#include <core/scalar.h>
#include <core/point.h>

namespace rt {

LambertianMaterial::LambertianMaterial(Texture* emission, Texture* diffuse)
{
	textreE = emission;
	textreD = diffuse;
    /* TODO */
}

RGBColor LambertianMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
	float costheta = std::max(0.0f, (dot(normal, inDir)));
	return RGBColor(textreD->getColor(texPoint).r * 1 / pi * costheta,
		textreD->getColor(texPoint).g * 1 / pi * costheta,
		textreD->getColor(texPoint).b * 1 / pi * costheta);
}

RGBColor LambertianMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
	//float costheta = std::max(0.0f, (dot(normal, outDir)));
	if (textreE == nullptr) {
		return RGBColor::rep(0.0f);
	}
	return RGBColor(textreE->getColor(texPoint).r,
		textreE->getColor(texPoint).g,
		textreE->getColor(texPoint).b);
}

Material::SampleReflectance LambertianMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
	return SampleReflectance(Vector(0,0,0), RGBColor::rep(0));
}

Material::Sampling LambertianMaterial::useSampling() const {
	return SAMPLING_NOT_NEEDED;
}

}