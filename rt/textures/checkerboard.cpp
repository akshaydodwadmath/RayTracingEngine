#include <rt/textures/checkerboard.h>
#include <core/point.h>
#include <cmath>

namespace rt {

CheckerboardTexture::CheckerboardTexture(const RGBColor& white, const RGBColor& black)
{
	w = white;
	b = black;
    /* TODO */
}

inline float modulo(const float &x)
{
	return x - std::floor(x);
}

RGBColor CheckerboardTexture::getColor(const Point& coord) {
	float pattern = (modulo(coord.x) < 0.5) ^ (modulo(coord.y) < 0.5) ^
		(modulo(coord.z) < 0.5);
	if (pattern)
		return w;
	else
		return b;
}

RGBColor CheckerboardTexture::getColorDX(const Point& coord) {
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor CheckerboardTexture::getColorDY(const Point& coord) {
    /* TODO */ NOT_IMPLEMENTED;
}

}