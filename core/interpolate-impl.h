#include "interpolate.h"

namespace rt {

template <typename T>
T lerp(const T& px0, const T& px1, float xPoint) {
	return T(px0 * (1 - xPoint) + px1 * xPoint);
}

template <typename T>
T lerpbar(const T& a, const T& b, const T& c, float aWeight, float bWeight) {
	return T(a * aWeight + b * bWeight + c * (1 - aWeight - bWeight));
}

template <typename T>
T lerp2d(const T& px0y0, const T& px1y0, const T& px0y1, const T& px1y1, float xWeight, float yWeight) {
	T interpolateX1 = lerp(px0y0, px1y0, xWeight);
	T interpolateX2 = lerp(px0y1, px1y1, xWeight);
	T interpolateY = lerp(interpolateX1, interpolateX2, yWeight);
	return interpolateY;
}

template <typename T>
T lerp3d(const T& px0y0z0, const T& px1y0z0, const T& px0y1z0, const T& px1y1z0,
    const T& px0y0z1, const T& px1y0z1, const T& px0y1z1, const T& px1y1z1,
    float xPoint, float yPoint, float zPoint)
{
	T interpolateX1 = lerp(px0y0z0, px1y0z0, xPoint);
	T interpolateX2 = lerp(px0y1z0, px1y1z0, xPoint);
	T interpolateX3 = lerp(px0y0z1, px1y0z1, xPoint);
	T interpolateX4 = lerp(px0y1z1, px1y1z1, xPoint);

	T interpolateY1 = lerp(interpolateX1, interpolateX2, yPoint);
	T interpolateY2 = lerp(interpolateX3, interpolateX4, yPoint);

	T interpolateZ = lerp(interpolateY1, interpolateY2, zPoint);

	return interpolateZ;
}

}