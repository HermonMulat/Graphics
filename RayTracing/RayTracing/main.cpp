
#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <algorithm>

#include "mat4x4.h"
#include "vec3.h"
#include "vec4.h"
#include "vec2.h"

#include "LightSource.h"
#include "Camera.h"
#include "Material.h"

#include "Intersectable.h"


#if defined(__APPLE__)
#include <GLUT/GLUT.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#else
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif
#include <GL/glew.h>		 
#include <GL/freeglut.h>	
#endif


using namespace std;

const unsigned int windowWidth = 900, windowHeight = 900;
const float Epsillon = 0.0001;
int majorVersion = 3, minorVersion = 0;

// image to be computed by ray tracing
vec3 image[windowWidth * windowHeight];

inline vec3 reflect(vec3 inDir,vec3 normal){
	vec3 perp = normal * normal.dot(inDir);
	vec3 parallel = inDir - perp;
	return parallel - perp;
}

class Scene {
	Camera camera;
	vector<Intersectable*> objects;
	vector<Material*> materials;
	vector<LightSource*> lights;

	public:
		Scene() {
			lights.push_back(new DirectionalLight(vec3(0.5, 0.5, 0.5),vec3(0, 0, -1)));
			lights.push_back(new PointLight(vec3(1, 1, 1), vec3(1, 1.5, 1)));
			lights.push_back(new PointLight(vec3(2.5, 2.5, 2.5), vec3(0, 1.5, -3)));

			materials.push_back(new BeachBall());
			materials.push_back(new Wood());
			materials.push_back(new Material(vec3(0.447,0.392,0.216)));
			materials.push_back(new Material(vec3(0.121, 0.176,0.857),0.8));
			materials.push_back(new Material(vec3(1, 0.9, 0)));
			materials.push_back(new Marble());
			

			// Final Stuff//
			
			objects.push_back(new Beach());
			objects.push_back((new Parasol(materials[5],materials[1]))->Transform(mat4x4::translation(vec3(0.6,-0.2,0.5))));
			objects.push_back((new Quadric(materials[0]))->Sphere(0.2)->Transform(mat4x4::translation(vec3(-0.75, -0.2, -2))));
			objects.push_back((new Plane(vec3(0, 1, 0), vec3(0, -0.5, 0), materials[3])));
			objects.push_back((new Square(0.15,materials[1]))->Transform(mat4x4::rotation(vec3(1, 1, 0), -45))->Transform(mat4x4::translation(vec3(0,-0.5,1))));
			
			objects.push_back((new ClippedQuadric(materials[2]))->Cone(0.15, 0.15)->Transform(mat4x4::translation(vec3(-0.25, 0.1, -2))));
			objects.push_back((new ClippedQuadric(materials[2]))->Cylinder(0.4, 0.15)->Transform(mat4x4::translation(vec3(-0.25, -0.25, -2))));

		 }

		~Scene(){
			for (std::vector<Material*>::iterator iMaterial = materials.begin(); iMaterial != materials.end(); ++iMaterial)
				delete *iMaterial;
			for (std::vector<Intersectable*>::iterator iObject = objects.begin(); iObject != objects.end(); ++iObject)
				delete *iObject;		
		}

		Camera& getCamera(){
			return camera;
		}

		Hit firstIntersect(const Ray& ray) {
			Hit min = Hit();
			Hit curr;
			for (int i = 0; i < objects.size(); i++) {
				curr = objects[i]->intersect(ray);
				if (min.t < 0 || (curr.t>0 && curr.t < min.t)) {
					min = curr;
				}	
			}
			return min;
		}

		vec3 trace(const Ray& ray,int depth){
			Hit hit = firstIntersect(ray);
			if(hit.t < 0)
				return vec3(1, 1, 1);
			
			if (hit.material->Reflect && depth >=0) {
				float T = (1 - hit.material->reflectance);
				float R = hit.material->reflectance;

				vec3 reflectionDir = reflect(ray.dir, hit.normal);
				Ray ReflectionRay = Ray(hit.position + reflectionDir*Epsillon, reflectionDir);
				vec3 reflection = trace(ReflectionRay, depth - 1);

				return reflection*R + hit.material->getColor(hit.position)*T;
			}
			
			vec3 finalColor = vec3(0, 0, 0);
			vec3 shadowRayDir, shadowRayOrigin;
			for (int i = 0; i < lights.size(); i++) {
				shadowRayDir = lights[i]->getLightDirAt(hit.position);
				shadowRayOrigin = hit.position + shadowRayDir*Epsillon;
				Hit s = firstIntersect(Ray(shadowRayOrigin, shadowRayDir));
				if (s.t < 0 || lights[i]->getDistanceFrom(hit.position) < s.t || s.material->Reflect) {
					finalColor += hit.material->shade(hit.position, hit.normal, lights[i]);
				}
			}
			return finalColor;
		}
};

Scene scene;

bool computeImage(){
	static unsigned int iPart = 0;

	if(iPart >= 64)
		return false;
    for(int j = iPart; j < windowHeight; j+=64){
        for(int i = 0; i < windowWidth; i++){
			float ndcX = (2.0 * i - windowWidth) / windowWidth;
			float ndcY = (2.0 * j - windowHeight) / windowHeight;
			Camera& camera = scene.getCamera();
			Ray ray = Ray(camera.getEye(), camera.rayDirFromNdc(ndcX, ndcY));
			
			image[j*windowWidth + i] = scene.trace(ray,1);
		}
	}
	iPart++;
	return true;
}

void onDisplay( ) {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	if(computeImage())
		glutPostRedisplay();
    glDrawPixels(windowWidth, windowHeight, GL_RGB, GL_FLOAT, image);
 
    glutSwapBuffers(); 
}



int main(int argc, char * argv[]) {
	glutInit(&argc, argv);
#if !defined(__APPLE__)
	glutInitContextVersion(majorVersion, minorVersion);
#endif
	glutInitWindowSize(windowWidth, windowHeight);				
	glutInitWindowPosition(100, 100);							
#if defined(__APPLE__)
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);  
#else
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif
	glutCreateWindow("Ray Casting");

#if !defined(__APPLE__)
	glewExperimental = true;	
	glewInit();
#endif

	printf("GL Vendor    : %s\n", glGetString(GL_VENDOR));
	printf("GL Renderer  : %s\n", glGetString(GL_RENDERER));
	printf("GL Version (string)  : %s\n", glGetString(GL_VERSION));
	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
	printf("GL Version (integer) : %d.%d\n", majorVersion, minorVersion);
	printf("GLSL Version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	glViewport(0, 0, windowWidth, windowHeight);

	glutDisplayFunc(onDisplay);                

	glutMainLoop();

	return 1;
}

