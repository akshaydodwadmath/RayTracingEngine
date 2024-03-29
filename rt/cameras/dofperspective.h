#ifndef CG1RAYTRACER_CAMERAS_DOFPERSPECTIVE_HEADER
#define CG1RAYTRACER_CAMERAS_DOFPERSPECTIVE_HEADER

#include <rt/cameras/camera.h>

namespace rt {

class Point;
class Vector;

class DOFPerspectiveCamera : public Camera {
public:
    DOFPerspectiveCamera(
        const Point& center,
        const Vector& forward,
        const Vector& up,
        float verticalOpeningAngle,
        float horizonalOpeningAngle,
        float focalDistance,
        float apertureRadius
    );

	Point mCenter ;
	float mFocalDistance ;
	float mApertureRadius ;
	float mHorizontalOpeningAngle;
	float mVerticalOpeningAngle;

	Vector mForward ;
	Vector mRight ;
	Vector mUp ;



    virtual Ray getPrimaryRay(float x, float y) const;
};

}

#endif