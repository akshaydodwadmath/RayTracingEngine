
#include <core/assert.h>
#include <core/scalar.h>
#include <core/image.h>
#include <rt/world.h>
#include <rt/renderer.h>
#include <rt/loaders/obj.h>
#include <rt/groups/bvh.h>
#include <rt/groups/simplegroup.h>
#include <rt/solids/sphere.h>
#include <rt/solids/quad.h>
#include <rt/solids/disc.h>
#include <rt/cameras/perspective.h>
#include <rt/cameras/dofperspective.h>
#include <rt/integrators/casting.h>
#include <rt/integrators/raytrace.h>
#include <rt/integrators/recraytrace.h>
#include <rt/lights/pointlight.h>
#include <rt/lights/spotlight.h>
#include <rt/lights/arealight.h>
#include <rt/materials/lambertian.h>
#include <rt/textures/constant.h>
#include <rt/textures/imagetex.h>
#include <rt/primmod/bmap.h>
#include <rt/solids/triangle.h>
using namespace rt;

void a_indexing() {
    //Image img(800, 600);
	//Image img1(480, 270);
	Image img2(1920, 1080);
    BVH* scene = new BVH();
	World world;
	world.scene = scene;

	//Table
	loadOBJ(scene, "models/table/", 6, "t1.obj", "models/table/", "ttex1.jpg");
	loadOBJ(scene, "models/table/", 6, "t2.obj", "models/table/", "ttex1.jpg");
	loadOBJ(scene, "models/table/", 6, "t3.obj", "models/table/", "ttex1.jpg");
	loadOBJ(scene, "models/table/", 6, "t4.obj", "models/table/", "ttex4.jpg");
	
	loadOBJ(scene, "models/chair/", 6, "chairseat.obj", "models/table/", "ttex1.jpg");
	loadOBJ(scene, "models/chair/", 6, "chairsides.obj", "models/table/", "ttex1.jpg");
	loadOBJ(scene, "models/chair/", 6, "chairup.obj", "models/table/", "ttex1.jpg");

	//Fan 
	loadOBJ(scene, "models/fan_lamp/", 16, "fanbase1.obj", "models/fan_lamp/", "fan2.jpg");
	loadOBJ(scene, "models/fan_lamp/", 16, "fanbase2.obj", "models/fan_lamp/", "fan3.jpg");
	loadOBJ(scene, "models/fan_lamp/", 16, "fanwings.obj", "models/fan_lamp/", "fan1.png");
	
	//Lamp
	loadOBJ(scene, "models/fan_lamp/", 6, "lampbelow.obj", "models/fan_lamp/", "lamp2.jpg");
	loadOBJ(scene, "models/fan_lamp/", 6, "lampabove.obj", "models/fan_lamp/", "lamp1.jpg");
	
	//glass
	loadOBJ(scene, "models/fan_lamp/", 15, "glassinside.obj", "models/fan_lamp/", "lamp1.jpg");
	loadOBJ(scene, "models/fan_lamp/", 7, "glassoutside.obj", "models/fan_lamp/", "lamp1.jpg");
	
	//walls
	//loadOBJ(scene, "models/", 6,"sidewall1.obj", "models/", "mat.jpg");
	//loadOBJ(scene, "models/", 6, "sidewall2.obj", "models/", "blacktexture.jpg");
	loadOBJ(scene, "models/", 6, "ceiling.obj", "models/", "blacktexture.jpg");
	loadOBJ(scene, "models/", 6, "backwall.obj", "models/walls/", "walltex1.jpg");
	loadOBJ(scene, "models/", 10, "floor.obj", "models/", "stones_bump.png");

	//bathroom
	loadOBJ(scene, "models/bathroom/", 8, "bathwallparts.obj", "models/", "mat2.jpg");
	loadOBJ(scene, "models/bathroom/", 12, "doorout.obj", "models/bathroom/", "doortex5.jpg");
	loadOBJ(scene, "models/bathroom/", 13, "doormid.obj", "models/bathroom/", "doortex5.jpg");
	loadOBJ(scene, "models/bathroom/", 5, "doorin1.obj", "models/bathroom/", "doortex5.jpg");
	loadOBJ(scene, "models/bathroom/", 5, "doorin2.obj", "models/bathroom/", "doortex5.jpg");
	
	//open book
	loadOBJ(scene, "models/book/", 6, "bookbottom.obj", "models/book/", "booktex1.jpg");
	loadOBJ(scene, "models/book/", 6, "bookleft.obj", "models/book/", "booktex2.jpg");
	loadOBJ(scene, "models/book/", 6, "bookright.obj", "models/book/", "booktex2.jpg");
	loadOBJ(scene, "models/book/", 6, "booktop.obj", "models/book/", "booktex5.png");
	
	//close book
	loadOBJ(scene, "models/book/closed/", 6, "book1a.obj", "models/book/", "booktex2.jpg");
	loadOBJ(scene, "models/book/closed/", 6, "book1b.obj", "models/book/", "booktex1.jpg");
	loadOBJ(scene, "models/book/closed/", 6, "book2a.obj", "models/book/", "booktex2.jpg");
	loadOBJ(scene, "models/book/closed/", 6, "book2b.obj", "models/book/", "booktex1.jpg");
	loadOBJ(scene, "models/book/closed/", 6, "book3a.obj", "models/book/", "booktex2.jpg");
	loadOBJ(scene, "models/book/closed/", 6, "book3b.obj", "models/book/", "booktex1.jpg");
	loadOBJ(scene, "models/book/closed/", 6, "book4a.obj", "models/book/", "booktex2.jpg");
	loadOBJ(scene, "models/book/closed/", 6, "book4b.obj", "models/book/", "booktex1.jpg");
	

	//loadOBJ(walls1, "models/bathroom/", 2, "hidden.obj", "models/table/", "ttex1.jpg");
	//scene->add(walls1);
	//Camera * cam2;
	//PerspectiveCamera cam2(Point(0.278f, 0.273f, -0.800f), Vector(0, 0, 1), Vector(0, 1, 0), 0.686f, 0.686f);
	PerspectiveCamera cam2(Point(-0, 0, -1.2), Vector(-0.0000, -0.0197, 0.9998), Vector(-0.0000, -0.9998, -0.0197), pi / 8, pi / 5);
	//DOFPerspectiveCamera* dofcam = new DOFPerspectiveCamera(Point(-0, 0, -1.2), Vector(-0.0000, -0.0197, 0.9998), Vector(-0.0000, -0.9998, -0.0197), pi / 8, pi / 5, 1.3f, 0.045f);
	//cam2 = dofcam;
	//Lights
	//float lscale = 0.1;
	float lamplightscale = 0.07;
	Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
	ConstantTexture* lightsrctex = new ConstantTexture(RGBColor(10000.0f*lamplightscale*lamplightscale, 5760.0f*lamplightscale*lamplightscale, 1610.0f*lamplightscale*lamplightscale));
	Material* lightsource = new LambertianMaterial(lightsrctex, blacktex);
	float scale = 0.001;

	Quad * lamplight = new Quad(Point(0.35, -0.05, 0.03), Vector(0, 0, 105 * scale), Vector(230 * scale, 0, 0), nullptr, lightsource);


//	Quad* light = new Quad(Point(-0.35, -0.16, +0.63), Vector(0, 0, 140 * scale), Vector(105 * scale, 0, 0), nullptr, lightsource);
	//Quad* lamplight = new Quad(Point(0.42, -0.05, +0.40), Vector(0, 0, 140 * scale), Vector(105 * scale, 0, 0), nullptr, lightsource);
	//Quad* lamplight = new Quad(Point(0.26, -0.02, -0.13), Vector(0, 0, 305 * scale), Vector(105 * scale, 0, 0),  nullptr, lightsource);
	AreaLight als(lamplight);
	world.light.push_back(&als);
	//scene->add(lamplight);
	
	
	float pscale = 0.005;

	
	world.light.push_back(new PointLight(Point(-0.31, -0.24, +0.61), RGBColor(10000.0f*pscale*pscale, 7726.0f*pscale*pscale, 5608.0f*pscale*pscale)));
	
	scene->rebuildIndex();

	RecursiveRayTracingIntegrator integrator(&world);
	Renderer engine2(&cam2, &integrator);
	engine2.setSamples(30);
	//engine2.setSamples(10);
	
	//engine2.render(img1);
	//img1.writePNG("test1.png");
	
	engine2.render(img2);
	img2.writePNG("high_qual.png");

	/************** END HERE ********************/
}


	//	PerspectiveCamera cam1(Point(-8.85f, -7.85f, 7.0f), Vector(1.0f, 1.0f, -0.6f), Vector(0, 0, 1), pi / 8, pi / 6);
//    PerspectiveCamera cam1(Point(-7.35f, -6.98f, 4.80f), Vector(1.0f,1.0f,-0.6f), Vector(0, 0,	-1), pi/8, pi/6);
//	PerspectiveCamera cam1(Point(1.0f, 0.0f, -1.5f), Vector(1.0f, 1.0f, -0.6f), Vector(0, 0, 1), pi / 8, pi / 6);
	//Working for tablecenter2
	//PerspectiveCamera cam2(Point(0.0, -12.506f, 0.0), Vector(-0.0, 12.506f, 0.0), Vector(0, 0, 1), pi/8, pi/6);
    
	//Working as expected for tablecenter.obj
	//PerspectiveCamera cam2(Point(-0, -1.2, 0), Vector(-0.0245, 0.9995, -0.0197), Vector(0.0404, -0.0187, -0.9990), pi / 8, pi / 6);
	
	//PerspectiveCamera cam2(Point(5, 0, 1), Vector(-0.9773, 0.1232, 0.1725), Vector(0.1215, 0.9924, -0.0202), pi / 8, pi / 6);
	//RayCastingIntegrator integrator(&world);



	//float scale = 0.01f;
	//point light
	//world.light.push_back(new PointLight(Point(0.0f, 0.0f, 1.0f), RGBColor::rep(40000.0f*scale*scale)));
	//world.light.push_back(new PointLight(Point(490 * scale, 329.99f*scale, 279.5f*scale), RGBColor(60000.0f*scale*scale, 0, 0)));
	//world.light.push_back(new PointLight(Point(40 * scale, 329.99f*scale, 279.5f*scale), RGBColor(0, 60000.0f*scale*scale, 0)));
	//world.light.push_back(new JuliaLight(Point(40 * scale, 329.99f*scale, 279.5f*scale), RGBColor(0, 60000.0f*scale*scale, 0)));
	   //Renderer engine1(&cam1, &integrator);
	//engine1.render(img);
	//img.writePNG("rc1.png");
