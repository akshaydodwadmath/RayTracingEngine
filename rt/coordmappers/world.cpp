#include <rt/coordmappers/world.h>

#include <rt/intersection.h>
namespace rt {

Point WorldMapper::getCoords(const Intersection& hit) const {
	return Point(hit.hitPoint().x * mScale.x,
		hit.hitPoint().y * mScale.y,
		hit.hitPoint().z * mScale.z) ;
}

WorldMapper::WorldMapper()
{
	Vector local(1.0, 1.0, 1.0);
	mScale = local;
    /* TODO */
}

WorldMapper::WorldMapper(const Vector& scale)
{
	mScale = scale;
    /* TODO */
}

}