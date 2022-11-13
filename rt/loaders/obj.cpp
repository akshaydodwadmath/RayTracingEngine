#define DISABLE_COORDMAPPERS
#define DISABLE_SMOOTH_TRIANGLE
#define DISABLE_MATERIALS

#include <core/assert.h>
#include <core/color.h>
#include <rt/loaders/obj.h>
#include <rt/groups/group.h>
#include <rt/solids/triangle.h>
#include <string>
#include <fstream>
#include <set>
#include <rt/materials/flatmaterial.h>
#include <rt/materials/mirror.h>
#include <rt/materials/lambertian.h>
#include <rt/materials/glass.h>
#include <rt/materials/phong.h>
#include <rt/materials/combine.h>
#include <rt/textures/constant.h>
#include <rt/textures/imagetex.h>
#include <rt/primmod/bmap.h>
#ifndef DISABLE_COORDMAPPERS
#include <rt/coordmappers/world.h>
#include <rt/coordmappers/tmapper.h>
#endif

#ifndef DISABLE_SMOOTH_TRIANGLE
#include <rt/solids/striangle.h>
#endif

namespace rt {

namespace {
enum Instruction {
    Obj_None, //no instruction at all
    Obj_Invalid, //error while parsing an instruction
    Obj_Vertex, //geometric vertices
    Obj_TexVertex, //texture vertices
    Obj_NormalVertex, //vertex normals
    Obj_ParamVertex, //parameter space vertices - Free-form curve/surface attributes
    Obj_CurveVertex, //rational or non-rational forms of curve or surface type: basis matrix, Bezier, B-spline, Cardinal, Taylor
    Obj_Degree, //degree
    Obj_BasisMatrix, //basis matrix
    Obj_Step, //step size
    Obj_Point, //point
    Obj_Line, //line
    Obj_Face, //face
    Obj_Curve, //curve
    Obj_Curve2, //2D-curve
    Obj_Surface, //surface
    Obj_CurveParameter, //Free-form curve/surface body - parameter values
    Obj_CurveTrim, //Free-form curve/surface body - outer trimming loop
    Obj_CurveHole, //Free-form curve/surface body - inner trimming loop
    Obj_CurveSpecialCurve, //Free-form curve/surface body - special curve
    Obj_CurveSpecialPoint, //Free-form curve/surface body - special point
    Obj_CurveEnd, //Free-form curve/surface body - end
    Obj_Connect, //Connect free-form surfaces
    Obj_Group, //group
    Obj_Smooth, //smooth group
    Obj_MergingGroup, //merging group
    Obj_Object, //object name
    Obj_Bevel, //bevel interpolation
    Obj_ColorInterpolation, //color interpolation
    Obj_DissolveInterpolation, //dissolve interpolation
    Obj_LOD, //level of detail
    Obj_Material, //use material
    Obj_MaterialLibrary, //material library
    Obj_Shadow, //shadow casting
    Obj_Trace, //ray tracing
    Obj_ApproxCurve, //curve approximation technique
    Obj_ApproxSurface //surface approximation technique
};

struct Int3 {
    size_t vidx, tidx, nidx;
};
struct Float2 {
    float x, y;
    operator Point() {
        return Point(x,y,0.0f);
    }
    operator Vector() {
        return Vector(x,y,0.0f);
    }
};
struct Float3 {
    float x, y, z;
    operator Point() {
        return Point(x,y,z);
    }
    operator Vector() {
        return Vector(x,y,z);
    }
};

struct FileLine {
    std::string str;
    size_t pos;
    size_t lineIdx;
    std::string filename;
    std::ifstream file;
    void open(const std::string& filename);
    void close();
    void nextLine();
    bool eof() const { return file.eof(); }
    void removeComments();
    void skipWhitespace();
    bool match(const char* reference);
    Instruction fetchInstruction();
    float fetchFloat();
    float fetchFloat(float defaultv);
    std::string fetchString();
    Int3 fetchVertex();
};

void FileLine::open( const std::string& infilename ) {
    filename = infilename;
    lineIdx = 0;
    pos = 0;
    file.open(filename.c_str(), std::ios_base::in);
    release_assert(!file.fail()) << "Error opening " << filename << "\n";
}

void FileLine::close() {
    file.close();
}

void FileLine::nextLine() {
    ++lineIdx;
    std::getline(file,str);
    pos = 0;
}

void FileLine::removeComments() {
    size_t idx=str.find('#');
    if (idx!=std::string::npos)
        str.erase(idx); //clear all elements starting from idx
}

void FileLine::skipWhitespace() {
    while (str.c_str()[pos]==' ' || str.c_str()[pos]=='\t' || str.c_str()[pos]=='\n')   ++pos;
}

bool FileLine::match(const char* reference) {
    size_t i=0;
    //check each character of "reference"
    while (reference[i]!=0) {
        if (str.c_str()[pos+i] != reference[i])
            return false;
        ++i;
    }
    //confirm that the input word ends right here
    char c=str.c_str()[pos+i];
    if (!((c>='A' && c<='Z') || (c>='a' && c<='z') || (c>='0' && c<='9')|| (c=='_'))) {
        pos = pos+i;
        return true;
    }
    return false;
}

Instruction FileLine::fetchInstruction() {
    Instruction i=Obj_None;
    skipWhitespace();

    if (match("")) return Obj_None;
    if (match("v")) return Obj_Vertex;
    if (match("vn")) return Obj_NormalVertex;
    if (match("vt")) return Obj_TexVertex;
    if (match("f")) return Obj_Face;
    if (match("usemtl")) return Obj_Material;
    if (match("mtllib")) return Obj_MaterialLibrary;
    if (match("o")) return Obj_Object;
    if (match("g")) return Obj_Group;
    if (match("p")) return Obj_Point;
    if (match("l")) return Obj_Line;
    if (match("mg")) return Obj_MergingGroup;
    if (match("vp")) return Obj_ParamVertex;
    if (match("surf")) return Obj_Surface;
    if (match("cstype")) return Obj_CurveVertex;
    if (match("curv")) return Obj_Curve;
    if (match("curv2")) return Obj_Curve2;
    if (match("con")) return Obj_Connect;
    if (match("c_interp")) return Obj_ColorInterpolation;
    if (match("ctech")) return Obj_ApproxCurve;
    if (match("deg")) return Obj_Degree;
    if (match("bmat")) return Obj_BasisMatrix;
    if (match("bevel")) return Obj_Bevel;
    if (match("sp")) return Obj_CurveSpecialPoint;
    if (match("step")) return Obj_Step;
    if (match("scrv")) return Obj_CurveSpecialCurve;
    if (match("shadow_obj")) return Obj_Shadow;
    if (match("stech")) return Obj_ApproxSurface;
    if (match("s")) return Obj_Smooth;
    if (match("parm")) return Obj_CurveParameter;
    if (match("lod")) return Obj_LOD;
    if (match("trim")) return Obj_CurveTrim;
    if (match("trace_obj")) return Obj_Trace;
    if (match("hole")) return Obj_CurveHole;
    if (match("end")) return Obj_CurveEnd;
    if (match("d_interp")) return Obj_DissolveInterpolation;
    return Obj_Invalid;
};

float FileLine::fetchFloat() {
    skipWhitespace();
    const char* src=str.c_str()+pos;
    char *dest;
    float f=float(strtod(src,&dest));
    release_assert(dest!=src) << "Error in file " << filename << ":" << lineIdx << "." << pos << " : expected a floating-point number";
    pos+=(dest-src);
    return f;
}

float FileLine::fetchFloat(float defaultv) {
    skipWhitespace();
    const char* src=str.c_str()+pos;
    char *dest;
    float f=float(strtod(src,&dest));
    if (dest==src)
        f=defaultv;
    else
        pos+=dest-src;
    return f;
}

std::string FileLine::fetchString() {
    skipWhitespace();
    size_t start = pos;
    while (str.c_str()[pos]!=' ' && str.c_str()[pos]!='\t' && str.c_str()[pos]!='\n')   ++pos;
    return str.substr(start, pos-start);
}

Int3 FileLine::fetchVertex() {
    skipWhitespace();
    const char* src=str.c_str()+pos;
    char *dest;
    Int3 i;
    i.vidx = i.tidx = i.nidx = 0;
    i.vidx=strtol(src,&dest,10);
    if (dest==src) return i; // (0,0,0)
    pos+=dest-src;
    skipWhitespace();
    if (str.c_str()[pos]!='/') return i;
    ++pos;
    skipWhitespace();
    if (str.c_str()[pos]!='/') {
        src = str.c_str()+pos;
        i.tidx=strtol(src,&dest,10);
        release_assert(dest!=src) << "Error in file " << filename << ":" << lineIdx << "." << pos << " : Expected an integer (texture coordinate index)";
        pos+=dest-src;
        skipWhitespace();
        if (str.c_str()[pos]!='/') return i;
    }
    ++pos;
    skipWhitespace();
    src = str.c_str()+pos;
    i.nidx=strtol(src,&dest,10);
    release_assert(dest!=src) << "Error in file " << filename << ":" << lineIdx << "." << pos << " : Expected an integer (vertex normal index)";
    pos+=dest-src;
    return i;
}
}


void loadOBJ(Group* dest, const std::string& path, int texNum, const std::string& filename, const std::string& tpath, const std::string& tfilename, MatLib* inmats) {
    MatLib* matlib;
    if (inmats)
        matlib = inmats;
    else
        matlib = new MatLib;

    std::vector<std::string> matFiles;

    std::vector<Float3> vertices;
    std::vector<Float3> normals;
    std::vector<Float2> texcoord;

    std::set<Instruction> unsupportedEncounters;
	//Texture* greentex;
	//Texture* bluetex;
	//bluetex = new ConstantTexture(RGBColor(0.f, 0.f, 0.7f));
	//greentex = new ConstantTexture(RGBColor(0.f, .7f, 0.f));

	Texture* redtex = new ConstantTexture(RGBColor(.7f, 0.f, 0.f));
	Texture* greentex = new ConstantTexture(RGBColor(0.f, .7f, 0.f));
	Texture* yellowtex = new ConstantTexture(RGBColor(0.7f, .7f, 0.f));
	Texture* bluetex = new ConstantTexture(RGBColor(0.f, 0.f, 0.7f));
	Texture* lightbluetex = new ConstantTexture(RGBColor(0.678f, 0.847f, 0.902f));
	Texture* browntex = new ConstantTexture(RGBColor(0.545f, 0.271f, 0.075f));
	Texture* graytex = new ConstantTexture(RGBColor(0.5f, 0.5f, 0.5f));
	Texture* tomato = new ConstantTexture(RGBColor(1.0f, 0.389f, 0.278f));
	Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
	Texture* whitetex = new ConstantTexture(RGBColor::rep(1.0f));
	Texture* maroontex = new ConstantTexture(RGBColor(0.5f, 0.0f, 0.0f));

	Material* grey = new LambertianMaterial(blacktex, whitetex);
	Material* yellowmat = new LambertianMaterial(blacktex, yellowtex);
	Material* leftWallMaterial = new LambertianMaterial(blacktex, redtex);
	Material* rightWallMaterial = new LambertianMaterial(blacktex, greentex);

	ImageTexture* woodenTex;
	Material* material;

	std::string texfileName = tpath + tfilename;
	//Mirror
	if (texNum == 4) {
		material = new MirrorMaterial(2.485f, 3.433f);
	}

	//Lambertian2 - Brown
	if (texNum == 5) {
		Texture* blacktex1 = new ConstantTexture(RGBColor::rep(0.05f));
		Material* lamb = new LambertianMaterial(blacktex, browntex);
		material = lamb;
	}

	//Lambertian1 - ImageTexture/ BumoMap
	if (texNum == 6 || texNum == 10) {
		woodenTex = new ImageTexture(texfileName, ImageTexture::REPEAT, ImageTexture::NEAREST);
		material = new LambertianMaterial(blacktex,woodenTex );
		//material = grey;
	}
	//Glass
	if (texNum == 7) {
		Material* glass =  new GlassMaterial(1.52f);
		Material* lamb = new LambertianMaterial(blacktex, whitetex);
		CombineMaterial* combined = new CombineMaterial();
		combined->add(lamb, 0.8f);
		combined->add(glass, 0.2f);

		//PhongMaterial* p =  new PhongMaterial(bluetex, 10.0f);
		material = combined;
	}
	if (texNum == 15) {
		Material* lamb = new LambertianMaterial(blacktex,lightbluetex );
		Material* glass = new GlassMaterial(1.333f);
		CombineMaterial* combined = new CombineMaterial();
		combined->add(lamb, 0.8f);
		combined->add(glass, 0.2f);

		//PhongMaterial* p =  new PhongMaterial(bluetex, 10.0f);
		material = combined;
	}
	//fan
	if (texNum == 16) {
		//woodenTex = new ImageTexture(texfileName, ImageTexture::REPEAT, ImageTexture::NEAREST);
		material = new LambertianMaterial(blacktex, graytex);
	}

	
	//Combined - Blue
	if (texNum == 8) {
		//whitetex = new ConstantTexture(RGBColor::rep(1.0f));
		//material = new PhongMaterial(whitetex, 10.0f);
		woodenTex = new ImageTexture(texfileName, ImageTexture::REPEAT, ImageTexture::NEAREST);
		Material* lamb = new LambertianMaterial(blacktex , maroontex);
		Material* glass = new GlassMaterial(5.0f);
		//Material* mirror = new MirrorMaterial(0.0f, 0.0f);
		CombineMaterial* combined = new CombineMaterial();
		combined->add(lamb, 0.8f);
		combined->add(glass, 0.2f);
		material = combined;
		
	}
	//Combined - Brown
	if (texNum == 12) {
		
		//whitetex = new ConstantTexture(RGBColor::rep(1.0f));
		//material = new PhongMaterial(whitetex, 10.0f);
		woodenTex = new ImageTexture(texfileName, ImageTexture::REPEAT, ImageTexture::NEAREST);
		Material* lamb = new LambertianMaterial(blacktex, browntex);
		Material* glass = new GlassMaterial(5.0f);
		//Material* mirror = new MirrorMaterial(0.0f, 0.0f);
		CombineMaterial* combined = new CombineMaterial();
		combined->add(lamb, 0.8f);
		combined->add(glass, 0.2f);
		material = combined;

	}

	//Combined - Tomato
	if (texNum == 13) {
		//whitetex = new ConstantTexture(RGBColor::rep(1.0f));
		//material = new PhongMaterial(whitetex, 10.0f);
		woodenTex = new ImageTexture(texfileName, ImageTexture::REPEAT, ImageTexture::NEAREST);
		Material* lamb = new LambertianMaterial(blacktex, tomato);
		Material* glass = new GlassMaterial(5.0f);
		//Material* mirror = new MirrorMaterial(0.0f, 0.0f);
		CombineMaterial* combined = new CombineMaterial();
		combined->add(lamb, 0.8f);
		combined->add(glass, 0.2f);
		material = combined;

	}

	//DummyMaterial *material = new DummyMaterial();
   // Material* material = nullptr;

    FileLine fileline;
    fileline.open(path + filename);

    size_t numfaces = 0;
    size_t lineIdx = 0;
    while (!fileline.eof()) {
        fileline.nextLine();
        fileline.removeComments();
        Instruction i = fileline.fetchInstruction();
        switch (i) {
            case Obj_Vertex:
            case Obj_NormalVertex: {
                Float3 v;
                v.x = fileline.fetchFloat();
                v.y = fileline.fetchFloat();
                v.z = fileline.fetchFloat();
                float w = fileline.fetchFloat(1.0f);
                if (w != 1.0f) { v.x/=w; v.y/=w; v.z/=w; }
                if (i == Obj_Vertex)
                    vertices.push_back(v);
                else
                    normals.push_back(v);
                break;
            }
            case Obj_TexVertex: {
                Float2 v;
                v.x = fileline.fetchFloat();
                v.y = 1.0f - fileline.fetchFloat(0.0f);
                fileline.fetchFloat(0.0f); //u, v, w. Ignore w
                texcoord.push_back(v);
                break;
            }
            case Obj_Face: {
                ++numfaces;

                Int3 v[3];

#define REMAP_INDEX(idx, size) \
                if ((idx) != 0) { \
                    if ((idx) < 0) \
                        (idx) = (size) + (idx); \
                    else \
                        --(idx); \
                } \

#define READ_VERTEX(i, do_check) \
                { \
                    v[i] = fileline.fetchVertex(); \
                    if (do_check) \
                        release_assert(v[i].vidx != 0) << "Error in file " << fileline.filename << ":" << fileline.lineIdx << "." << fileline.pos << " : Vertex index cannot be 0"; \
                    REMAP_INDEX(v[i].vidx, vertices.size()) \
                    REMAP_INDEX(v[i].tidx, texcoord.size()) \
                    REMAP_INDEX(v[i].nidx, normals.size()) \
                }

                READ_VERTEX(0, true)
                READ_VERTEX(1, true)
                READ_VERTEX(2, true)

                do {
                    CoordMapper* mapper = nullptr;
#ifndef DISABLE_COORDMAPPERS
                    bool skiptex = v[0].tidx == 0 || v[1].tidx == 0 || v[2].tidx == 0;
                    if (skiptex) {
                        mapper = new WorldMapper();
                    } else if (skipnormal) {
                        mapper = new TriangleMapper(
                            Point(texcoord[v[0].tidx]),
                            Point(texcoord[v[1].tidx]),
                            Point(texcoord[v[2].tidx]));
                    } else {
                        mapper = new TriangleMapper(
                            Point(texcoord[v[0].tidx]),
                            Point(texcoord[v[1].tidx]),
                            Point(texcoord[v[2].tidx]));
                    }
#endif

					Triangle* t;
#ifdef DISABLE_SMOOTH_TRIANGLE
                    bool skipnormal = true;
#else
                    bool skipnormal = v[0].nidx == 0 || v[1].nidx == 0 || v[2].nidx == 0;
#endif
                    if (skipnormal) {
						//std::cout << "triangle\n" << std::endl;
						

                        t = new Triangle(Point(vertices[v[0].vidx]), Point(vertices[v[1].vidx]), Point(vertices[v[2].vidx]), mapper, material);
                    } else {
#ifndef DISABLE_SMOOTH_TRIANGLE
                        t = new SmoothTriangle(
                            Point(vertices[v[0].vidx]), Point(vertices[v[1].vidx]), Point(vertices[v[2].vidx]),
                            Vector(normals[v[0].nidx]), Vector(normals[v[1].nidx]), Vector(normals[v[2].nidx]),
                            mapper, material);
#endif
                    }
					if (texNum == 10) {
						t->setCoordMapper(nullptr);
						t->setMaterial(nullptr);
						ImageTexture* bumptex = new ImageTexture("models/stones_bump.png", ImageTexture::REPEAT, ImageTexture::NEAREST);
						static const float bumbscale = 0.8f;
						//dest->add(
						//	new BumpMapper(t, bumptex, Point(0.0f, 0.0f, 0.0f), Point(0.0f, 0.0f, 1.0f), Point(0.0f, 1.0f, 0.0f), bumbscale));

						//floor
						dest->add(
							new BumpMapper(
								t,
								bumptex, Point(2.0f, 2.0f, 0.0f), Point(2.0f, 0.0f, 0.0f), Point(0.0f, 2.0f,0.0f), bumbscale)
						);
						/*dest->add(
							new BumpMapper(
								t,
								bumptex, Point(1.0f, 1.0f, 0.0f), Point(1.0f, 0.0f, 0.0f), Point(0.0f, 1.0f, 0.0f), bumbscale)
						);*/
					}
					else {
						dest->add(t);
					}
                    v[1] = v[2];
                    READ_VERTEX(2, false)
                } while (v[2].vidx != 0);
                break;
            }
            case Obj_MaterialLibrary: {
#ifndef DISABLE_MATERIALS
                std::string libname = fileline.fetchString();
                loadOBJMat(matlib, path, libname);
#endif
                break;
            }
            case Obj_Material: {
#ifndef DISABLE_MATERIALS
                std::string matname = fileline.fetchString();
                MatLib::iterator i = matlib->find(matname);
                if (i != matlib->end())
                    material = i->second;
                else
                    material = nullptr;
#endif
                break;
            }
            case Obj_None: break; //empty line
            case Obj_Invalid: 
                release_assert(false) << "Error in file " << fileline.filename << ":" << fileline.lineIdx << "." << fileline.pos << " : Vertex index cannot be 0";
            default:
                if (unsupportedEncounters.insert(i).second) //element inserted (was not there previously)
                    std::cerr << "Warning: Unsupported OBJ instruction encountered at " << fileline.filename << ":" << fileline.lineIdx << "." << fileline.pos << ", ignoring.\n";
        }
    }
    std::cerr << numfaces << " faces loaded.\n";
    fileline.close();
    if (!inmats)
        delete matlib;
}

}
