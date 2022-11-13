#ifndef CG1RAYTRACER_SOLIDS_DISC_HEADER
#define CG1RAYTRACER_SOLIDS_DISC_HEADER

#include <rt/solids/solid.h>

namespace rt {

class Disc : public Solid {
public:
    Point center;
    Vector normal;
    float radius;
    CoordMapper* texMapper;
    Material* material;
	Solid *minfPlane;

    Disc() {}
    Disc(const Point& center, const Vector& normal, float radius, CoordMapper* texMapper, Material* material);

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