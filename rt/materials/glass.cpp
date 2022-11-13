#include <rt/materials/glass.h>
#include <core/random.h>
#include <core/float4.h>
#include <rt/solids/quad.h>
#include <rt/solids/triangle.h>
#include <rt/solids/infiniteplane.h>
namespace rt {

GlassMaterial::GlassMaterial(float eta)
{
	mEta = eta;
    /* TODO */
}

RGBColor GlassMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
	return RGBColor(0, 0, 0);
}

RGBColor GlassMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
	return RGBColor(0, 0, 0);
}

void calFresnel(const Vector &I, const Vector &N, const float &ior, float &kr)
{
	Vector Nrefr = N;
	float NdotI = dot(Nrefr,I);
	float etai = 1, etat = ior; //refractice indices
	if (NdotI < 0) {
		// cos(theta) should always be positive
		NdotI = -NdotI;
	}

	if (dot(Nrefr, I) > 0) { std::swap(etai, etat); }
	// Compute sini using Snell's law
	float sint = etai / etat * sqrtf(std::max(0.f, 1 - NdotI * NdotI));
	// Check for Total internal reflection
	if (sint >= 1) {
		kr = 1;
	}
	else {
		float cost = sqrtf(std::max(0.f, 1 - sint * sint));
		NdotI = fabsf(NdotI);
		float Rs = ((etat * NdotI) - (etai * cost)) / ((etat * NdotI) + (etai * cost));
		float Rp = ((etai * NdotI) - (etat * cost)) / ((etai * NdotI) + (etat * cost));
		kr = (Rs * Rs + Rp * Rp) / 2;
	}
}

Vector calRefract(const Vector &I, const Vector &N, const float &ior)
{
	Vector Nrefr = N;
	float NdotI = dot(Nrefr, I);
	float etai = 1, etat = ior; 

	if (NdotI < 0) { NdotI = -NdotI; }
	else { std::swap(etai, etat); Nrefr = -N; }
	float eta = etai / etat;
	float k = 1 - eta * eta * (1 - NdotI * NdotI);
	if (k < 0)
		return Vector::rep(0.0f);
	else
		return (eta * I) + (eta * NdotI - sqrtf(k)) * Nrefr;
}


Material::SampleReflectance GlassMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
	
	float kr = 1;
	// compute fresnel
	calFresnel(outDir, normal, mEta, kr);
	Vector refractionDirection;
	// compute refraction if it is not a case of total internal reflection
	if (kr < 1) {
		refractionDirection = calRefract(outDir, normal, mEta);
	}
	Vector reflectionDirection = outDir - ((2 * dot(normal, outDir) * normal));
	
	Vector retDirection;
	
	RGBColor reflectColor;
	float num = random();
	if (num <=0.5 && kr<1) {
		retDirection = refractionDirection.normalize();
		float refra = 1 - kr;
		reflectColor = RGBColor::rep(refra/0.5f);
	}
	else {
		Ray reflectRay;
		retDirection = reflectionDirection.normalize();
		reflectColor = RGBColor::rep(kr / 0.5f);
	}
	return SampleReflectance(retDirection, reflectColor);
}

Material::Sampling GlassMaterial::useSampling() const {
	return SAMPLING_ALL;
}

}