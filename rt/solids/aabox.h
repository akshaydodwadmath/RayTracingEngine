#ifndef CG1RAYTRACER_SOLIDS_BOX_HEADER
#define CG1RAYTRACER_SOLIDS_BOX_HEADER

#include <rt/solids/solid.h>

namespace rt {

class AABox : public Solid {
public:

    Point corner1;
    Point corner2;
    CoordMapper* texMapper;
    Material* material;


    AABox() {}
    AABox(const Point& corner1, const Point& corner2, CoordMapper* texMapper, Material* material);

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