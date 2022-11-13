#include <rt/solids/quad.h>
#include <rt/solids/triangle.h>
#include <rt/solids/infiniteplane.h>
#include <core/random.h>
#include <core/float4.h>
namespace rt {

    Quad::Quad(const Point& origin, const Vector& span1, const Vector& span2, CoordMapper* texMapper, Material* material)
    {
        this->origin = origin;
        this->span1 = span1;
        this->span2 = span2;
        this->vert1 = origin + span1;
        this->vert2 = origin + span2;
        this->vert3 = origin + span1 + span2;
        this->material = material;
		if (texMapper == nullptr) {
			this->texMapper = new WorldMapper(Vector::rep(1.0));
		}
		else {
			this->texMapper = texMapper;
		}
		mnormal = cross(span1, span2).normalize();
		minfPlane = new InfinitePlane(origin, mnormal, this->texMapper, this->material);
    }

    BBox Quad::getBounds() const {
		Point v1 = this->origin;
		Point v2 = this->vert1;
		Point v3 = this->vert2;
		Point v4 = this->vert3;

		Point bBoxMin(std::min(v4.x, std::min(v1.x, std::min(v2.x, v3.x))),
			std::min(v4.y, std::min(v1.y, std::min(v2.y, v3.y))),
			std::min(v4.z, std::min(v1.z, std::min(v2.z, v3.z))));
		Point bBoxMax(std::max(v4.x, std::max(v1.x, std::max(v2.x, v3.x))),
			std::max(v4.y, std::max(v1.y, std::max(v2.y, v3.y))),
			std::max(v4.z, std::max(v1.z, std::max(v2.z, v3.z))));
		return BBox(bBoxMin, bBoxMax);
    }

	void Quad::setMaterial(Material* m) {
		material = m;
	}

    Intersection Quad::intersect(const Ray& ray, float previousBestDistance) const {
    	
        Intersection intersect = minfPlane->intersect(ray, previousBestDistance);
        if (intersect) {
            Point intersectionPoint = ray.getPoint(intersect.distance);
            Vector oiVec = intersectionPoint - origin;

            Vector a = (dot(oiVec, span1) / span1.lensqr()) * span1;
            Vector b = (dot(oiVec, span2) / span2.lensqr()) * span2;

            if ((a.length() <= span1.length() && a.length() > 0) && (b.length() > 0 && b.length() <= span2.length())) {
				return Intersection(intersect.distance, ray, this, mnormal, intersectionPoint);
            }
			
            return Intersection::failure();
        }
        return Intersection::failure();
    }

Solid::Sample Quad::sample() const {
	float u = random();
	float v = random();
	
	Point randP = origin + u * span1 + v * span2;

	Vector norm = cross(span1, span2).normalize();

	Sample s = { randP, norm };

	return s;
    /* TODO */ NOT_IMPLEMENTED;
}

float Quad::getArea() const {

	return cross(span1, span2).length();


    /*Vector span1n = span1.normalize();
    Vector span2n = span2.normalize();
    float angle = acosf(dot(span1n, span2n));

	//std::cout << ((span1.length() * span2.length())) * sinf(angle) << "area\n" << std::endl;
    return ((span1.length() * span2.length())) * sinf(angle);*/
}

}
