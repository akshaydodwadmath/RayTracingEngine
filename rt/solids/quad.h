#ifndef CG1RAYTRACER_SOLIDS_QUAD_HEADER
#define CG1RAYTRACER_SOLIDS_QUAD_HEADER

#include <rt/solids/solid.h>

namespace rt {

class Quad : public Solid {
public:
    Point origin;
    Point vert1;
    Point vert2;
    Point vert3;
    Vector span1;
    Vector span2;
    CoordMapper* texMapper;
    Material* material;
	Solid *minfPlane;
	Vector mnormal;
    Quad() {}
    Quad(const Point& v1, const Vector& span1, const Vector& span2, CoordMapper* texMapper, Material* material);

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