#include <rt/groups/simplegroup.h>

namespace rt {

BBox SimpleGroup::getBounds() const {
	BBox fullGroupBox;
	fullGroupBox = fullGroupBox.empty();
	for (Primitive* p : primitives) {
		BBox primBox = p->getBounds();
		fullGroupBox.extend(primBox);
	}
	return fullGroupBox;
}

Intersection SimpleGroup::intersect( const Ray& ray, float previousBestDistance) const {
    Intersection intersect = Intersection::failure();
    float shortestDistance = previousBestDistance; 

    for (Primitive* p : primitives) {
        
      Intersection intersectTemp = p->intersect(ray, shortestDistance);
       if (intersectTemp && intersectTemp.distance < shortestDistance) {
           shortestDistance = intersectTemp.distance;
           intersect = intersectTemp;
       }
    }
    return intersect;
}

void SimpleGroup::rebuildIndex() {
    //do nothing
}

void SimpleGroup::add(Primitive* p) {
    this->primitives.push_back(p);
}

void SimpleGroup::setMaterial(Material* m) {
	for (Primitive* p : primitives) {
		p->setMaterial(m);
	}
}

void SimpleGroup::setCoordMapper(CoordMapper* cm) {
	for (Primitive* p : primitives) {
		p->setCoordMapper(cm);
	}
}

}
