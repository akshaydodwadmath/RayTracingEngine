#include <rt/textures/constant.h>

namespace rt {

ConstantTexture::ConstantTexture()
{
    /* TODO */
}

ConstantTexture::ConstantTexture(const RGBColor& color)
{
	mColor = color;
}

RGBColor ConstantTexture::getColor(const Point& coord) {
	return mColor;
}

RGBColor ConstantTexture::getColorDX(const Point& coord) {
	return mColor;
}

RGBColor ConstantTexture::getColorDY(const Point& coord) {
	return mColor;
}

}