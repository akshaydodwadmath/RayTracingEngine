#include <rt/cameras/dofperspective.h>
#include <rt/ray.h>
#include <core/random.h>
#include <core/scalar.h>
namespace rt {

DOFPerspectiveCamera::DOFPerspectiveCamera(const Point& center, const Vector& forward, const Vector& up, float verticalOpeningAngle, float horizontalOpeningAngle, float focalDistance, float apertureRadius)
{

	this->mCenter = center;
	this->mForward = forward.normalize();
	this->mRight = cross(mForward, up).normalize();
	this->mUp = cross(mRight, mForward);

	this->mFocalDistance = focalDistance;
	this->mApertureRadius = apertureRadius;
	
	this->mHorizontalOpeningAngle = horizontalOpeningAngle;
	this->mVerticalOpeningAngle = verticalOpeningAngle;
    /* TODO */
}

Ray DOFPerspectiveCamera::getPrimaryRay(float x, float y) const {

	Vector pcx = mRight * std::tan(mHorizontalOpeningAngle / 2.f);
	Vector pcy = mUp * std::tan(mVerticalOpeningAngle / 2.f);
	
	Point newCenter = mCenter + (x*pcx + y * pcy + mForward) * mFocalDistance;
	float rad1 = sqrt(random()) * mApertureRadius;
	float rad2 = random(2 * pi);
	
	float x1 = rad1 * cos(rad2);
	float x2 = rad1 * sin(rad2);

	Point origin = mCenter + x1 * mRight + x2 * mUp;

	return Ray(origin, (newCenter - origin).normalize());
}

}