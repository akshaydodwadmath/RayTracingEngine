#ifndef CG1RAYTRACER_SOLIDS_INFINITEPLANE_HEADER
#define CG1RAYTRACER_SOLIDS_INFINITEPLANE_HEADER

#include <rt/solids/solid.h>

namespace rt {

class InfinitePlane : public Solid {
public:
    Point origin;
    Vector normal;
    CoordMapper* texMapper;
    Material* material;


    InfinitePlane() {}
    InfinitePlane(const Point& origin, const Vector& normal, CoordMapper* texMapper, Material* material);

    virtual BBox getBounds() const;
	virtual void setMaterial(Material* m);
	virtual void setCoordMapper(CoordMapper* cm) { texMapper = cm; };
    virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const;
    virtual Sample sample() const;
    virtual float getArea() const;
	virtual CoordMapper* getCoordMapper() const { return texMapper; };
	virtual Material* getMaterial() const { return material; };
};

}

#endif