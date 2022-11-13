#include <rt/materials/fuzzymirror.h>
#include <rt/solids/disc.h>
#include <rt/solids/solid.h>
namespace rt {

FuzzyMirrorMaterial::FuzzyMirrorMaterial(float eta, float kappa, float fuzzyangle)
{
	float mEta = eta; 
	float mKappa = kappa; 
	float mFuzzyangle = fuzzyangle;
}

RGBColor FuzzyMirrorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
	return RGBColor(0, 0, 0);
}

RGBColor FuzzyMirrorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
	return RGBColor(0, 0, 0);
}

float calparallel2(float mEta, float mKappa, float ctheta) {
	float num = ((sqr(mEta) + sqr(mKappa)) * sqr(ctheta)) - (2 * mEta*ctheta) + 1;
	float den = ((sqr(mEta) + sqr(mKappa)) * sqr(ctheta)) + (2 * mEta*ctheta) + 1;
	return num / den;
}

float calperp2(float mEta, float mKappa, float ctheta) {
	float num = (sqr(mEta) + sqr(mKappa)) - (2 * mEta*ctheta) + sqr(ctheta);
	float den = (sqr(mEta) + sqr(mKappa)) + (2 * mEta*ctheta) + sqr(ctheta);;
	return num / den;
}

Material::SampleReflectance FuzzyMirrorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
	Vector reflectionDirection = outDir - ((2 * dot(normal, outDir) * normal));
	Disc d(texPoint + reflectionDirection.normalize(), reflectionDirection.normalize(), 1.0f, nullptr, nullptr);
	Vector newDir;
	/*static int count = 1000;
	do {
		rt::Solid::Sample s = d.sample();

		newDir = s.point - texPoint;
		count--;
	}
	while ((dot(newDir.normalize(), reflectionDirection.normalize()) < mFuzzyangle )
			|| (count==0));
	if (count == 0)// no direction within angle limit is found
		newDir = reflectionDirection;*/
	static int count = 0;
	while (count < 10) {
		count++;
		rt::Solid::Sample s = d.sample();

		newDir = s.point - texPoint;
		if (!(dot(newDir.normalize(), reflectionDirection.normalize()) < mFuzzyangle))
			count = 20;
	}
	if(count == 10)
		newDir = reflectionDirection;
	count = 0;
	float costheta = dot(normal.normalize(), -outDir.normalize());
	float rparallel = calparallel2(mEta, mKappa, costheta);
	float rperp = calperp2(mEta, mKappa, costheta);
	float Fr = 0.5f * (sqr(rparallel) + sqr(rperp));
	RGBColor reflc(Fr, Fr, Fr);
	
	return SampleReflectance(newDir, reflc);
}

Material::Sampling FuzzyMirrorMaterial::useSampling() const {
	return SAMPLING_ALL;
}

}
