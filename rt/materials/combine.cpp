#include <rt/materials/combine.h>
#include <rt/solids/solid.h>

namespace rt {

CombineMaterial::CombineMaterial()
{
	mTotalWeight = 0.0f;
}

void CombineMaterial::add(Material* m, float w) {
	Combine c = { m, w };
	combMat.push_back(c);
	mTotalWeight = mTotalWeight + w;
}

RGBColor CombineMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
	RGBColor color(0, 0, 0);
	for (Combine cM : combMat) {
		color = color + ((cM.w / mTotalWeight) * cM.m->getReflectance(texPoint, normal, outDir, inDir));
	}
	color.clamp();
	return color;
}

RGBColor CombineMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
	RGBColor color(0, 0, 0);
	for (Combine cM : combMat) {
		color = color + ((cM.w/ mTotalWeight) * cM.m->getEmission(texPoint, normal, outDir));
	}
	color.clamp();
	return color;
}

Material::SampleReflectance CombineMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
	SampleReflectance ret(Vector(0, 0, 0), RGBColor::rep(0));
	for (Combine cM : combMat) {
		if (cM.m->useSampling() == SAMPLING_ALL) {
			ret =  cM.m->getSampleReflectance(texPoint, normal, outDir);
		}
	}
	return ret;
}

Material::Sampling CombineMaterial::useSampling() const {
	return SAMPLING_SECONDARY;
}

}