#ifndef CG1RAYTRACER_SOLIDS_TRIANGLE_HEADER
#define CG1RAYTRACER_SOLIDS_TRIANGLE_HEADER

#include <rt/solids/solid.h>

namespace rt {

class Triangle : public Solid {
public:
    Point v1;
    Point v2;
    Point v3;
    CoordMapper* texMapper;
    Material* material;

    Triangle() {}
    Triangle(Point vertices[3], CoordMapper* texMapper, Material* material);
    Triangle(const Point& v1, const Point& v2, const Point& v3, CoordMapper* texMapper, Material* material);

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