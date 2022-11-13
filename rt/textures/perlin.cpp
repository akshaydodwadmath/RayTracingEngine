#include <rt/textures/perlin.h>
#include <core/point.h>
#include <core/vector.h>
#include <core/interpolate.h>
#include <cmath>

namespace rt {
static const unsigned tableSize = 256;
static const unsigned tableSizeMask = tableSize - 1;
float calNoise(float x, float y, float z);

// returns a value in range -1 to 1
static inline float noise(int x, int y, int z) {
    int n = x + y * 57 + z * 997;
    n = (n << 13) ^ n;
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

PerlinTexture::PerlinTexture(const RGBColor& white, const RGBColor& black)
{
	w = white;
	b = black;
}

rt::RGBColor PerlinTexture::getColor(const Point& coord) {
	float noisefunc = 0.0f;
	for (octave oct : mOctave) {
		noisefunc = noisefunc + oct.amp * 
			std::abs(calNoise(oct.freq* coord.x, oct.freq*coord.y, oct.freq*coord.z));
	}
	RGBColor ret = lerp(w, b, noisefunc);
	return ret;
}

rt::RGBColor PerlinTexture::getColorDX(const Point& coord) {
    /* TODO */ NOT_IMPLEMENTED;
}

rt::RGBColor PerlinTexture::getColorDY(const Point& coord) {
    /* TODO */ NOT_IMPLEMENTED;
}

void PerlinTexture::addOctave(float amplitude, float frequency) {
	octave localOct = { amplitude, frequency };
	mOctave.push_back(localOct);
}

float smoothstep(const float &t) { return t * t * (3 - 2 * t); }

float calNoise(float x, float y, float z) {
	int xi0 = ((int)std::floor(x)) & tableSizeMask;
	int yi0 = ((int)std::floor(y)) & tableSizeMask;
	int zi0 = ((int)std::floor(z)) & tableSizeMask;

	int xi1 = (xi0 + 1) & tableSizeMask;
	int yi1 = (yi0 + 1) & tableSizeMask;
	int zi1 = (zi0 + 1) & tableSizeMask;

	float tx = x - ((int)std::floor(x));
	float ty = y - ((int)std::floor(y));
	float tz = z - ((int)std::floor(z));

	float u = smoothstep(tx);
	float v = smoothstep(ty);
	float w = smoothstep(tz);

	Vector c000 = Vector(noise(xi0, yi0, zi0), noise(xi0, yi0, zi0), noise(xi0, yi0, zi0));
	Vector c100 = Vector(noise(xi1, yi0, zi0), noise(xi1, yi0, zi0), noise(xi1, yi0, zi0));
	Vector c010 = Vector(noise(xi0, yi1, zi0), noise(xi0, yi1, zi0), noise(xi0, yi1, zi0));
	Vector c110 = Vector(noise(xi1, yi1, zi0), noise(xi1, yi1, zi0), noise(xi1, yi1, zi0));

	Vector c001 = Vector(noise(xi0, yi0, zi1), noise(xi0, yi0, zi1), noise(xi0, yi0, zi1));
	Vector c101 = Vector(noise(xi1, yi0, zi1), noise(xi1, yi0, zi1), noise(xi1, yi0, zi1));
	Vector c011 = Vector(noise(xi0, yi1, zi1), noise(xi0, yi1, zi1), noise(xi0, yi1, zi1));
	Vector c111 = Vector(noise(xi1, yi1, zi1), noise(xi1, yi1, zi1), noise(xi1, yi1, zi1));

	// generate vectors going from the grid points to p
	float x0 = tx, x1 = tx - 1;
	float y0 = ty, y1 = ty - 1;
	float z0 = tz, z1 = tz - 1;

	Vector p000 = Vector(x0, y0, z0);
	Vector p100 = Vector(x1, y0, z0);
	Vector p010 = Vector(x0, y1, z0);
	Vector p110 = Vector(x1, y1, z0);

	Vector p001 = Vector(x0, y0, z1);
	Vector p101 = Vector(x1, y0, z1);
	Vector p011 = Vector(x0, y1, z1);
	Vector p111 = Vector(x1, y1, z1);

	// linear interpolation
	return lerp3d(dot(c000, p000), dot(c100, p100),
			dot(c010, p010), dot(c110, p110),
			dot(c001, p001), dot(c101, p101),
			dot(c011, p011), dot(c111, p111), u, v, w);
}

}