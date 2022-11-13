#include <rt/textures/imagetex.h>
#include <core/point.h>
#include <core/vector.h>
#include <core/interpolate.h>
#include <cmath>

namespace rt {

ImageTexture::ImageTexture()
{
    /* TODO */
}

ImageTexture::ImageTexture(const std::string& filename, BorderHandlingType bh, InterpolationType i)
{
	mImage.readPNG(filename);
	mResU = mImage.width();
	mResV = mImage.height();
	mBH = bh;
	mIT = i;
	std::cout << "image1 h1\n" << mImage.height() << std::endl;
	std::cout << "image1 w1\n" << mImage.width() << std::endl;
    /* TODO */
}

ImageTexture::ImageTexture(const Image& image, BorderHandlingType bh, InterpolationType i)
{
	mImage = image;
	mResU = mImage.width();
	mResV = mImage.height();
	mBH = bh;
	mIT = i;
	std::cout << "image1 h2\n" << mImage.height() << std::endl;
	std::cout << "image1 w2\n" << mImage.width() << std::endl;
    /* TODO */
}

RGBColor ImageTexture::getColor(const Point& coord) {
	float local_u;
	float local_v;
	float tu = 0.0f;
	float tv = 0.0f;

	switch (mBH) {
	case ImageTexture::MIRROR:
	{
		tu = (coord.x - ((int)std::floor(coord.x)));
		tv = (coord.y - ((int)std::floor(coord.y)));
	
		if ((int)std::floor(coord.x) % 2 != 0)
			tu = 1 - tu;
		if ((int)std::floor(coord.y) % 2 != 0)
			tv = 1 - tv;
	}
	break;
	case ImageTexture::CLAMP:
	{
		tu = coord.x;
		tv = coord.y;

		if (tu > 1)
			tu = 1;
		if (tu < 0)
			tu = 0;

		if (tv > 1)
			tv = 1;
		if (tv < 0)
			tv = 0;
	}
	break;
	case ImageTexture::REPEAT:
	{
		tu = (coord.x - ((int)std::floor(coord.x)));
		tv = (coord.y - ((int)std::floor(coord.y)));
	}
	default:
		//do nothing
		break;
	}

	switch (mIT) {
	case ImageTexture::BILINEAR:
	{
		local_u = tu * (mResU - 1);
		local_v = tv * (mResV - 1);
		float fu = local_u - (int)std::floor(local_u);
		float fv = local_v - (int)std::floor(local_v);

		int lu = std::min((int)std::floor(local_u), (mResU - 2));
		int lv = std::min((int)std::floor(local_v), (mResV - 2));
		lu = std::max(lu, 0);
		lv = std::max(lv, 0);
		return lerp2d(mImage(std::floor(lu), std::floor(lv)),
			mImage(std::floor(lu), std::floor(lv) + 1),
			mImage(std::floor(lu) + 1, std::floor(lv)),
			mImage(std::floor(lu) + 1, std::floor(lv) + 1),
			fu, fv);
		
	}
	break;

	case ImageTexture::NEAREST:
	{
		local_u = tu * (mResU -1);
		local_v = tv * (mResV -1);
		int lu = std::min((int)std::floor(local_u), (mResU - 1));
		int lv = std::min((int)std::floor(local_v), (mResV - 1));
		lu = std::max(lu,0);
		lv = std::max(lv, 0);
		return mImage(lu, lv);
	}
	break;

	default:
		//do nothing
		break;
	}
}


RGBColor lerpprocess(Image &mImage, int temp1, int temp2, float fu, float fv) {
	return lerp2d(mImage(std::floor(temp1), std::floor(temp2)),
		mImage(std::floor(temp1), std::floor(temp2) + 1),
		mImage(std::floor(temp1) + 1, std::floor(temp2)),
		mImage(std::floor(temp1) + 1, std::floor(temp2) + 1),
		fu, fv);
}

RGBColor ImageTexture::getColorDX(const Point& coord) {

	float local_u;
	float local_v;
	float tu = 0.0f;
	float tv = 0.0f;
	switch (mBH) {
		case ImageTexture::REPEAT:
		{
			tu = (coord.x - ((int)std::floor(coord.x)));
			tv = (coord.y - ((int)std::floor(coord.y)));
		}
		break;
		default:
			//do nothing
			break;
	}
	switch (mIT) {
	case ImageTexture::BILINEAR:
	{
		local_u = tu * (mResU - 1);
		local_v = tv * (mResV - 1);
		float fu = local_u - (int)std::floor(local_u);
		float fv = local_v - (int)std::floor(local_v);

		int lu = std::min((int)std::floor(local_u), (mResU - 3));
		int lv = std::min((int)std::floor(local_v), (mResV - 3));
		lu = std::max(lu, 2);
		lv = std::max(lv, 2);

		return (lerpprocess(mImage,lu - 1, lv - 1, fu,fv) * 1
			+ lerpprocess(mImage, lu, lv - 1, fu, fv) * 2
			+ lerpprocess(mImage, lu + 1, lv - 1, fu, fv) * 1
			+ lerpprocess(mImage, lu - 1, lv, fu, fv) * 0
			+ lerpprocess(mImage, lu, lv, fu, fv) * 0
			+ lerpprocess(mImage, lu + 1, lv, fu, fv) * 0
			+ lerpprocess(mImage, lu - 1, lv + 1, fu, fv) * -1
			+ lerpprocess(mImage, lu, lv + 1, fu, fv) * -2 
				+ lerpprocess(mImage, lu + 1, lv + 1, fu, fv) * -1);
	
	}
	break;

	case ImageTexture::NEAREST:
	{
		local_u = tu * (mResU - 1);
		local_v = tv * (mResV - 1);
		int lu = std::min((int)std::floor(local_u), (mResU - 2));
		int lv = std::min((int)std::floor(local_v), (mResV - 2));
		lu = std::max(lu, 1);
		lv = std::max(lv, 1);

		//std::cout << "nearestX" << std::endl;
		return (mImage(lu - 1, lv - 1) * 1 + mImage(lu, lv - 1) * 2 + mImage(lu + 1, lv - 1) * 1
			+ mImage(lu - 1, lv) * 0 + mImage(lu, lv) * 0 + mImage(lu + 1, lv) * 0
			+ mImage(lu - 1, lv + 1) * -1 + mImage(lu, lv + 1) * -2 + mImage(lu + 1, lv + 1) * -1);
	}
	break;

	default:
		//do nothing
		break;
	}
}

RGBColor ImageTexture::getColorDY(const Point& coord) {
	float local_u;
	float local_v;
	float tu = 0.0f;
	float tv = 0.0f;
	switch (mBH) {
	case ImageTexture::REPEAT:
	{
		tu = (coord.x - ((int)std::floor(coord.x)));
		tv = (coord.y - ((int)std::floor(coord.y)));
	}
	break;
	default:
		//do nothing
		break;
	}
	switch (mIT) {
		case ImageTexture::BILINEAR:
		{
			local_u = tu * (mResU - 1);
			local_v = tv * (mResV - 1);
			float fu = local_u - (int)std::floor(local_u);
			float fv = local_v - (int)std::floor(local_v);

			int lu = std::min((int)std::floor(local_u), (mResU - 3));
			int lv = std::min((int)std::floor(local_v), (mResV - 3));

			lu = std::max(lu, 2);
			lv = std::max(lv, 2);

			return (lerpprocess(mImage, lu - 1, lv - 1, fu, fv) * -1
				+ lerpprocess(mImage, lu -1, lv , fu, fv) * -2
				+ lerpprocess(mImage, lu - 1, lv + 1, fu, fv) * -1
				+ lerpprocess(mImage, lu, lv -1, fu, fv) * 0
				+ lerpprocess(mImage, lu, lv, fu, fv) * 0
				+ lerpprocess(mImage, lu, lv + 1, fu, fv) * 0
				+ lerpprocess(mImage, lu + 1, lv - 1, fu, fv) * 1
				+ lerpprocess(mImage, lu + 1, lv, fu, fv) * 2
				+ lerpprocess(mImage, lu + 1, lv + 1, fu, fv) * 1);
		}
		break;

	case ImageTexture::NEAREST:
	{
		local_u = tu * (mResU - 1);
		local_v = tv * (mResV - 1);
		int lu = std::min((int)std::floor(local_u), (mResU - 2));
		int lv = std::min((int)std::floor(local_v), (mResV - 2));
		lu = std::max(lu, 1);
		lv = std::max(lv, 1);

		//std::cout << "nearestY" << std::endl;
		return (mImage(lu - 1, lv - 1) * -1 + mImage(lu - 1, lv) * -2 + mImage(lu - 1, lv + 1) * -1
			+ mImage(lu, lv-1) * 0 + mImage(lu, lv) * 0 + mImage(lu, lv+1) * 0
			+ mImage(lu + 1, lv - 1) * 1 + mImage(lu +1, lv) * 2 + mImage(lu + 1, lv + 1) * 1);
	}
	break;

	default:
		//do nothing
		break;
	}
}

}