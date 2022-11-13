#include <core/color.h>
#include <core/scalar.h>
#include <core/float4.h>

namespace rt {

RGBColor::RGBColor(const Float4& f4)
{
/* TODO */ NOT_IMPLEMENTED;
}

RGBColor RGBColor::operator + (const RGBColor& c) const {
	return RGBColor(this->r + c.r, this->g + c.g, this->b + c.b);
}

RGBColor RGBColor::operator - (const RGBColor& c) const {
	return RGBColor(this->r - c.r, this->g - c.g, this->b - c.b);
}

RGBColor RGBColor::operator * (const RGBColor& c) const {
	return RGBColor( this->r * c.r, this->g * c.g, this->b * c.b);
}

bool RGBColor::operator == (const RGBColor& c) const {
	return ((fabs(this->r - c.r) < epsilon) &&
		(fabs(this->g - c.g) < epsilon) &&
		(fabs(this->b - c.b) < epsilon));
}

bool RGBColor::operator != (const RGBColor& b) const {
	return !((fabs(this->r - b.r) < epsilon) &&
		(fabs(this->g - b.g) < epsilon) &&
		(fabs(this->b - b.b) < epsilon));
}

RGBColor RGBColor::clamp() const {
	return RGBColor(std::min(std::max(this->r, CLAMP_MIN_COLOR), CLAMP_MAX_COLOR),
					std::min(std::max(this->g, CLAMP_MIN_COLOR), CLAMP_MAX_COLOR),
					std::min(std::max(this->b, CLAMP_MIN_COLOR), CLAMP_MAX_COLOR));
}

RGBColor RGBColor::gamma(float gam) const {
    /* TODO */ NOT_IMPLEMENTED;
}

float RGBColor::luminance() const {
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor operator * (float scalar, const RGBColor& c) {
	return RGBColor(scalar*c.r, scalar*c.g, scalar*c.b);
}

RGBColor operator * (const RGBColor& c, float scalar) {
	return RGBColor(scalar*c.r, scalar*c.g, scalar*c.b);
}

RGBColor operator / (const RGBColor& c, float scalar) {
	assert(scalar != 0);
	float divScalar = 1 / scalar;
	return RGBColor(divScalar*c.r, divScalar*c.g, divScalar*c.b);
}

}
