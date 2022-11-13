#include <rt/primmod/bmap.h>
#include <core/assert.h>
#include <rt/solids/triangle.h>
#include <rt/coordmappers/tmapper.h>
#include <rt/materials/flatmaterial.h>
#include <rt/materials/material.h>
#include <rt/coordmappers/coordmapper.h>
#include <core/float4.h>
#include <core/matrix.h>
namespace rt {

BumpMapper::BumpMapper(Triangle* base, Texture* bumpmap, const Point& bv1, const Point& bv2, const Point& bv3, float vscale)
{
	mbase = base;
	Material *m = new FlatMaterial(bumpmap);
	mbase->setMaterial(m);
	CoordMapper *cm = new TriangleMapper(bv1, bv2, bv3);
	mbase->setCoordMapper(cm);
	mbumpmap = bumpmap;
	mbv1 = bv1;
	mbv2 = bv2;
	mbv3 = bv3;
	mvscale = vscale;

	Vector norm = cross(base->v2 - base->v1, base->v3 - base->v1);
	Float4 vect1(base->v2 - base->v1);
	Float4 vect2(base->v3 - base->v1);
	Float4 vect3 = Float4(norm);
	Float4 vect4(bv2 - bv1);
	Float4 vect5(bv3 - bv1);
	Float4 vect6(cross(bv2 - bv1, bv3 - bv1).normalize());

	Matrix mat1 = Matrix(vect1, vect2, vect3, Float4(base->v1)).transpose();
	Matrix mat2 = Matrix(vect4, vect5, vect6, Float4(bv1)).transpose().invert();

	Matrix transform = product(mat1, mat2);

	mvectorX = transform * Vector(1.0f, 0.0f, 0.0f);
	mvectorY = transform * Vector(0.0f, 1.0f, 0.0f);
}

BBox BumpMapper::getBounds() const {
	return mbase->getBounds();
}

// TODO: A full implementation has to make use of the coord mappers as well!
Intersection BumpMapper::intersect(const Ray& ray, float previousBestDistance) const {
	Intersection intersectTemp = mbase->intersect(ray, previousBestDistance);
	if (intersectTemp) {
		CoordMapper *cM = mbase->getCoordMapper();
		Point texP = cM->getCoords(intersectTemp);

		Point Ou(Float4((Float4(texP) + Float4(Point(1, 0, 0)))));
		Point Ov(Float4((Float4(texP) + Float4(Point(0, 1, 0)))));

		RGBColor offset = mbumpmap->getColor(texP);
		RGBColor offsetX = mbumpmap->getColorDX(texP);
		RGBColor offsetY = mbumpmap->getColorDY(texP);

		float shiftX = mvscale * offsetX.r;
		float shiftY = mvscale * offsetY.r;

		Vector pu = Ou - texP;
		Vector pv = Ov - texP;
		//Vector normal = intersectTemp.normal();
		//Vector D = offsetX.r/mvscale * cross(normal, pv) - offsetY.r/ mvscale * cross(normal, pu);

		Vector norm_new(-offsetX.r, -offsetY.r, mvscale);

		intersectTemp.normalVec = intersectTemp.normalVec - offset.r * norm_new;
		
		Vector newNormal = (intersectTemp.normal() - (shiftX * mvectorX) - (shiftY * mvectorY)).normalize();
		intersectTemp.normalVec = newNormal;
	}

	return intersectTemp;
}

void BumpMapper::setMaterial(Material* m) {
    /* TODO */ NOT_IMPLEMENTED;
}

void BumpMapper::setCoordMapper(CoordMapper* cm) {
    /* TODO */ NOT_IMPLEMENTED;
}

}