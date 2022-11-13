#include <rt/coordmappers/tmapper.h>

#include <core/interpolate.h>
namespace rt {

TriangleMapper::TriangleMapper(Point ntv[3])
{
	b1 = ntv[0];
	b2 = ntv[1];
	b3 = ntv[2];
    /* TODO */
}

TriangleMapper::TriangleMapper(const Point& tv0, const Point& tv1, const Point& tv2)
{
	b1 = tv0 ;
	b2 = tv1;
	b3 = tv2;

    /* TODO */
}

Point TriangleMapper::getCoords(const Intersection& hit) const {
	return lerpbar(b2, b3, b1, hit.local().x, hit.local().y);
}

}