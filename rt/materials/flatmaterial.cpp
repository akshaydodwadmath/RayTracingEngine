#include <rt/materials/flatmaterial.h>

namespace rt {

FlatMaterial::FlatMaterial(Texture* texture)
{
	textre = texture;
	
    /* TODO */
}

RGBColor FlatMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
	return RGBColor::rep(dot(-outDir, normal));
	//return RGBColor(0.0, 0.0, 0.0);
}

RGBColor FlatMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
	//return(textre->getColor(texPoint) * dot(-outDir, normal));
	//return RGBColor::rep(dot(-outDir, normal));
	return RGBColor(0.0, 0.0, 0.0);
}

Material::SampleReflectance FlatMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    UNREACHABLE;
}

Material::Sampling FlatMaterial::useSampling() const {
	return SAMPLING_NOT_NEEDED;
}

}