#include <rt/materials/mirror.h>
#include <core/scalar.h>

namespace rt {

MirrorMaterial::MirrorMaterial(float eta, float kappa)
{
	mEta = eta;
	mKappa = kappa;
    /* TODO */
}

RGBColor MirrorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
	return RGBColor(0, 0, 0);
}

RGBColor MirrorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
	return RGBColor(0, 0, 0);
}

float calparallel(float mEta,float mKappa,float ctheta) {
	float num = ((sqr(mEta) + sqr(mKappa)) * sqr(ctheta)) - (2 * mEta*ctheta) + 1;
	float den = ((sqr(mEta) + sqr(mKappa)) * sqr(ctheta)) + (2 * mEta*ctheta) + 1;
	return num / den;
}

float calperp(float mEta, float mKappa, float ctheta) {
	float num = (sqr(mEta) + sqr(mKappa)) - (2 * mEta*ctheta) + sqr(ctheta);
	float den = (sqr(mEta) + sqr(mKappa)) + (2 * mEta*ctheta) + sqr(ctheta);;
	return num / den;
}

Material::SampleReflectance MirrorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
	//Vector incDir = outDir + (2 * dot(normal, outDir) * outDir);
	Vector incDir = outDir - ((2 * dot(normal, outDir) * normal));
	//RGBColor reflc = getReflectance(texPoint, normal, outDir, incDir);
	//Amount of reflection

	float costheta = dot(normal.normalize(), -outDir.normalize());
	float rparallel = calparallel(mEta, mKappa, costheta);
	float rperp = calperp(mEta, mKappa, costheta);
	float Fr = 0.5f * (sqr(rparallel) + sqr(rperp));
	RGBColor reflc(Fr, Fr, Fr);
	return SampleReflectance(incDir, reflc);
}

Material::Sampling MirrorMaterial::useSampling() const {
	return SAMPLING_ALL;
}

}