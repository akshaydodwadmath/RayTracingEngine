#include <rt/solids/triangle.h>
#include <core/random.h>
#include <core/float4.h>
namespace rt {

    Triangle::Triangle(Point vertices[3], CoordMapper* texMapper, Material* material)
    {
        this->v1 = vertices[0];
        this->v2 = vertices[1];
        this->v3 = vertices[2];
		this->material = material;
		
		if (texMapper == nullptr) {
			this->texMapper = new WorldMapper(Vector::rep(1.0));
		}
		else {
			this->texMapper = texMapper;
		}
    }

    Triangle::Triangle(const Point& v1, const Point& v2, const Point& v3, CoordMapper* texMapper, Material* material)
    {
        this->v1 = v1;
        this->v2 = v2;
        this->v3 = v3;
        this->material = material;
        this->texMapper = texMapper;
		if (texMapper == nullptr) {
			this->texMapper = new WorldMapper(Vector::rep(1.0));
		}
		else {
			this->texMapper = texMapper;
		}
    }

    BBox Triangle::getBounds() const {
		Point v1 = this->v1;
		Point v2 = this->v2;
		Point v3 = this->v3;

		Point bBoxMin(std::min(v1.x, std::min(v2.x, v3.x)), 
			std::min(v1.y, std::min(v2.y, v3.y)),
				std::min(v1.z, std::min(v2.z, v3.z)));
		Point bBoxMax(std::max(v1.x, std::max(v2.x, v3.x)),
			std::max(v1.y, std::max(v2.y, v3.y)),
			std::max(v1.z, std::max(v2.z, v3.z)));
		return BBox(bBoxMin, bBoxMax);
    }

	void Triangle::setMaterial(Material* m) {
		material = m;
	}

    Intersection Triangle::intersect(const Ray& ray, float previousBestDistance) const {
        Vector edge1, edge2, x, y, z;
        float a, b, c, d;
        edge1 = v2 - v1;
        edge2 = v3 - v1;
        x = cross(ray.d, edge2);
        a = dot(edge1, x);
        if (a == 0)
            return Intersection::failure();
        b = 1.0 / a;
        y = ray.o - v1;
        c = b * dot(y, x);
        if (c < 0.0 || c > 1.0)
            return false;
        z = cross(y, edge1);
        d = b * dot(ray.d, z);
        if (d < 0.0 || c + d > 1.0) {
            return Intersection::failure();
        }
        float t = b * dot(edge2, z);
        Point uv = Point(c, d, 1 - c - d);
        if (t > 0 && t < previousBestDistance)
        {
            return Intersection(t, ray, this, cross(edge1, edge2).normalize(), uv);
        }
        else
            return Intersection::failure();
    }

    Solid::Sample Triangle::sample() const {
		float u = random();
		float v = random();
		
		/*Float4 randF =
			(1 - sqrt(r1)) * Float4(v1) + (sqrt(r1)*(1 - r2)) * Float4(v2) + 
			(r2*sqrt(r1)) * Float4(v3);

		Point randP(randF);
		*/
		if (u + v > 1) {
			u = 1 - u;
			v = 1 - v;
		}
		Float4 randF = ((1 - u - v) * Float4(v1))
						+ (u * Float4(v2)) + (v * Float4(v3));
		Point randP(randF);
		Vector norm = cross(v2 - v1, v3 - v1).normalize();
		Sample s = { randP, norm };
		
		return s;

        /* TODO */ NOT_IMPLEMENTED;
    }

    float Triangle::getArea() const {
        Vector  edge1 = v2 - v1;
        Vector  edge2 = v3 - v1;
        Vector edge1n = edge1.normalize();
        Vector edge2n = edge2.normalize();
        float angle = acosf(dot(edge1n, edge2n));
        return ((edge1.length() * edge2.length()) / 2) * sinf(angle);

    }
}