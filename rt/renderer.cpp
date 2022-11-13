#include <core/scalar.h>
#include <core/image.h>
#include <core/random.h>
#include <rt/renderer.h>
#include <rt/ray.h>
#include <iostream>
#include <rt/cameras/orthographic.h>
#include <rt/cameras/perspective.h>
#include <rt/integrators/integrator.h>

namespace rt {

Renderer::Renderer(Camera* cam, Integrator* integrator)
    : cam(cam), integrator(integrator), samples(1)
{}

void Renderer::render(Image& img) {
	RGBColor rgb;
	Ray primaryRay;
    for (uint prcy = 0; prcy < img.height(); prcy++) {
        for (uint prcx = 0; prcx < img.width(); prcx++) {
			if (samples > 1) {
				uint s = samples;
				RGBColor avgColor(0, 0, 0);
				
				while (s > 0) {
					s--;
					float ndcx = (prcx + random()) / img.width();
					float ndcy = (prcy + random()) / img.height();
					float sscx = ndcx * 2 - 1;
					float sscy = 1 - ndcy * 2;
					
					primaryRay = cam->getPrimaryRay(sscx, sscy);
					avgColor = avgColor + integrator->getRadiance(primaryRay);
				}
				rgb = avgColor / samples;
				img(prcx, prcy) = rgb;
			}
			else {
				float ndcx = (prcx + 0.5) / img.width();
				float ndcy = (prcy + 0.5) / img.height();
				float sscx = ndcx * 2 - 1;
				float sscy = 1 - ndcy * 2;

				primaryRay = cam->getPrimaryRay(sscx, sscy);
				rgb = integrator->getRadiance(primaryRay);
			
				img(prcx, prcy) = rgb;
			}
        }
    }
}
}

rt::RGBColor a1computeColor(rt::uint x, rt::uint y, rt::uint width, rt::uint height);

namespace rt {

void Renderer::test_render1(Image& img) {
    for (uint rowpixel = 0; rowpixel < img.width(); rowpixel++) {
        for (uint colpixel = 0; colpixel < img.height(); colpixel++) {
            img(rowpixel, colpixel) = a1computeColor(rowpixel, colpixel, img.width(), img.height());
        }
    }
}
}

rt::RGBColor a2computeColor(const rt::Ray& r);

namespace rt {

void Renderer::test_render2(Image& img) {
    for (uint prcy = 0; prcy < img.height(); prcy++) {
        for (uint prcx = 0; prcx < img.width(); prcx++) {
            float ndcx = (prcx + 0.5) / img.width();
            float ndcy = (prcy + 0.5) / img.height();
            float sscx = ndcx * 2 - 1;
            float sscy = 1 - ndcy * 2;
            Ray primaryRay = cam->getPrimaryRay(sscx, sscy);
            rt::RGBColor rgb = a2computeColor(primaryRay);
            img(prcx, prcy) = rgb;
        }
    }
}

}
