#pragma once
#include "mat4x4.h"
#include "vec3.h"
#include "Material.h"
#include <algorithm>
#include <iostream>

using namespace std;
// Ray structure.
class Ray {
public:
	vec3 origin;
	vec3 dir;
	Ray(vec3 o, vec3 d) {
		origin = o;
		dir = d;
		dir.normalize();
	}
};

// Hit record structure. Contains all data that describes a ray-object intersection point.
class Hit {
public:
	Hit() {
		t = -1;
	}
	float t;				//< Ray paramter at intersection. Negative means no valid intersection.
	vec3 position;			//< Intersection coordinates.
	vec3 normal;			//< Surface normal at intersection.
	Material* material;		//< Material of intersected surface.

	bool operator<(const Hit& r) {
		return t < r.t;
	}
};

class Intersectable {
protected:
	Material* material;
public:
	Intersectable(Material* material) :material(material) {}
	virtual Hit intersect(const Ray& ray) = 0;
};


class QuadraticRoots {
public:
	float t1;
	float t2;
	// Solves the quadratic a*t*t + b*t + c = 0 using the Quadratic Formula [-b +- sqrt(b^2-4ac)] / 2a, 
	// and sets members t1 and t2 to store the roots.
	QuadraticRoots(float a, float b, float c) {
		float discr = b * b - 4.0 * a * c;
		if (discr < 0) // no roots
		{
			t1 = -1;
			t2 = -1;
			return;
		}
		float sqrt_discr = sqrt(discr);
		t1 = (-b + sqrt_discr) / 2.0 / a;
		t2 = (-b - sqrt_discr) / 2.0 / a;
	}

	// Returns the lesser of the positive solutions, 
	// or a negative value if there was no positive solution.
	float getLesserPositive() {
		return (0 < t1 && (t2 < 0 || t1 < t2)) ? t1 : t2;
	}
};

class Plane : public Intersectable {
	vec3 normal;
	vec3 refP;
public:
	Plane(const vec3& n, const vec3& ref, Material* material) :
		Intersectable(material),normal(n),refP(ref) {
		normal.normalize();
		}

	Hit intersect(const Ray& ray) {
		float denom = (ray.dir).dot(normal);
		float t;
		if (denom == 0) {
			t = -1;
		}
		else {
			t = (refP - ray.origin).dot(normal) / (denom);
		}

		Hit hit;
		hit.t = t;
		hit.material = material;
		hit.position = ray.origin + ray.dir * t;
		hit.normal = normal+snoiseGrad(hit.position); 

		return hit;
	}
};

class Quadric : public Intersectable {
protected:
	mat4x4 Coeff;
public:
	Quadric(Material* m) : Intersectable(m) {
		Coeff = mat4x4();
	}

	Quadric* Sphere(float r) {
		Coeff._33 = -1 * r*r;
		return this;
	}

	Quadric* Ellipsoid(float rx, float ry, float rz) {
		Coeff._00 = rx*rx;
		Coeff._01 = ry*ry;
		Coeff._02 = rz*rz;
		Coeff._33 = -1;

		return this;
	}

	Quadric* Cone(float r) {
		Coeff._11 = -1*r*r;
		Coeff._33 = 0;
		return this;
	}

	Quadric* Cylinder(float radius) {
		Coeff._33 = -1 * radius*radius;
		Coeff._11 = 0;
		return this;
	}

	Quadric* Planes(float dist) {
		Coeff._00 = 0;
		Coeff._22 = 0;
		Coeff._33 = -1 * dist*dist;
		return this;
	}

	Quadric* PlanesY(float dist) {
		Coeff._11 = 0;
		Coeff._22 = 0;
		Coeff._33 = -1 * dist*dist;
		return this;
	}

	Quadric* PlanesZ(float dist) {
		Coeff._00 = 0;
		Coeff._11 = 0;
		Coeff._33 = -1 * dist*dist;
		return this;
	}


	Quadric* Transform(mat4x4 t) {
		mat4x4 tInv = t.invert();
		Coeff = tInv * Coeff * tInv.transpose();
		return this;
	}

	Quadric(const mat4x4& coeff, Material* m) : Coeff(coeff), Intersectable(m) {}

	QuadraticRoots solveQuadratic(const Ray& ray) {
		vec4 rayDir = vec4(ray.dir, 0);
		vec4 rayOrigin = vec4(ray.origin, 1);
		float a = rayDir.dot(Coeff*rayDir);
		float b = rayDir.dot(Coeff*rayOrigin) + rayOrigin.dot(Coeff*rayDir);
		float c = rayOrigin.dot(Coeff*rayOrigin);
		return QuadraticRoots(a, b, c);

	}

	bool contains(vec3 r) {
		vec4 r4(r);
		float val = r4.dot(Coeff*r4);
		return val <= 0;
	}


	vec3 getNormalAt(vec3 r) {
		vec4 N4 = Coeff*vec4(r) + vec4(r)*Coeff;
		return vec3(N4.x, N4.y, N4.z).normalize(); 
	}
	Hit intersect(const Ray& ray) {
		// This is a generic intersect that works for any shape with a quadratic equation. 
		// solveQuadratic should solve the proper equation (+ ray equation) for the shape, 
		// and getNormalAt should return the proper normal
		float t = solveQuadratic(ray).getLesserPositive();

		Hit hit;
		hit.t = t;
		hit.material = material;
		hit.position = ray.origin + ray.dir * t;
		hit.normal = getNormalAt(hit.position);

		return hit;
	}
};

class ClippedQuadric : public Intersectable {
	protected:
		Quadric* shape;
		Quadric* clipper;
	public:
		ClippedQuadric(Material* m) : Intersectable(m) {
			shape = (new Quadric(m))->Sphere(0.38);
			clipper = (new Quadric(m))->Planes(0.35)->Transform(mat4x4::translation(vec3(0,0.6,0)));
		}

		ClippedQuadric* setShapes(Quadric* s, Quadric* c) {
			shape = s;
			clipper = c;
			return this;
		}

		ClippedQuadric* Cone(float height,float radius) {
			shape = (new Quadric(material))->Cone(radius/ height);
			height /= 2.0;
			clipper = (new Quadric(material))->Planes(height)->Transform(mat4x4::translation(vec3(0,-height, 0)));
			return this;
		}

		ClippedQuadric* Cylinder(float height, float radius) {
			shape = (new Quadric(material))->Cylinder(radius);
			height /= 2.0;
			clipper = (new Quadric(material))->Planes(height);
			return this;

		}

		ClippedQuadric* ClippedPlaneZ(float height, float width) {
			shape = (new Quadric(material))->Planes(width);
			clipper = (new Quadric(material))->PlanesY(height);
			return this;
		}

		ClippedQuadric* ClippedPlaneY(float height, float width) {
			shape = (new Quadric(material))->PlanesZ(width);
			clipper = (new Quadric(material))->Planes(height);
			return this;
		}

		ClippedQuadric* ClippedPlaneX(float height, float width) {
			shape = (new Quadric(material))->PlanesY(width);
			clipper = (new Quadric(material))->PlanesZ(height);
			return this;
		}


		ClippedQuadric* Transform(mat4x4 t) {
			shape->Transform(t);
			clipper->Transform(t);
			return this;
		}

		virtual Hit intersect(const Ray& ray) {
			QuadraticRoots roots = shape->solveQuadratic(ray);

			vec3 p1 = ray.origin + ray.dir*roots.t1;
			vec3 p2 = ray.origin + ray.dir*roots.t2;

			if (!clipper->contains(p1)) roots.t1 = -1 * abs(roots.t1);
			if (!clipper->contains(p2)) roots.t2 = -1 * abs(roots.t2);

			float t = roots.getLesserPositive();

			Hit hit;
			hit.t = t;
			hit.material = material;
			hit.position = ray.origin + ray.dir * t;
			hit.normal = getNormalAt(hit.position);

			return hit;
		}

		virtual vec3 getNormalAt(vec3 r) {
			return shape->getNormalAt(r) + snoiseGrad(r);
		}
};

class Beach : public ClippedQuadric {
	public :
		Beach(): ClippedQuadric(new Material(vec3(0.76,0.698,0.502))) {
			shape = (new Quadric(material))->Sphere(5)->Transform(mat4x4::translation(vec3(0, -5.25, -3)));
			clipper = (new Quadric(material))->Planes(4.75)->Transform(mat4x4::translation(vec3(0, 4.25, 0)));
		}

		vec3 getNormalAt(vec3 r) {
			return shape->getNormalAt(r) + snoiseGrad(r);
		}


};

class Parasol : public Intersectable {
	protected:
		ClippedQuadric *head, *body;
	public:
		Parasol(Material* h, Material* b) : Intersectable(h){
			head = new ClippedQuadric(h);
			body = (new ClippedQuadric(b))->Cylinder(0.35, 0.01)->Transform(mat4x4::translation(vec3(0, 0.08, 0)));
		}

		virtual Hit intersect(const Ray& ray) {
			Hit h1 = head->intersect(ray);
			Hit h2 = body->intersect(ray);

			return (0 < h1.t && (h2.t < 0 || h1.t < h2.t)) ? h1 : h2;
		}

		Parasol* Transform(mat4x4 t) {
			head->Transform(t);
			body->Transform(t);
			return this;
		}
};

class Faces : public Intersectable {
	Quadric *plane, *clipper, *clipper2;
	public:
		Faces(Material* m) : Intersectable(m) {
			plane = (new Quadric(material))->PlanesY(0.25);
			clipper = (new Quadric(material))->Planes(0.25);
			clipper2 = (new Quadric(material))->PlanesZ(0.25);
		}

		Faces* F1(float w) {
			plane = (new Quadric(material))->Planes(w);
			clipper = (new Quadric(material))->PlanesY(w);
			clipper2 = (new Quadric(material))->PlanesZ(w);
			return this;
		}
		Faces* F2(float w) {
			plane = (new Quadric(material))->PlanesY(w);
			clipper = (new Quadric(material))->Planes(w);
			clipper2 = (new Quadric(material))->PlanesZ(w);
			return this;
		}
		Faces* F3(float w) {
			plane = (new Quadric(material))->PlanesZ(w);
			clipper = (new Quadric(material))->Planes(w);
			clipper2 = (new Quadric(material))->PlanesY(w);
			return this;
		}

		Faces* Transform(mat4x4 t) {
			plane->Transform(t);
			clipper->Transform(t);
			clipper2->Transform(t);
			return this;
		}

		virtual Hit intersect(const Ray& ray) {
			QuadraticRoots roots = plane->solveQuadratic(ray);

			vec3 p1 = ray.origin + ray.dir*roots.t1;
			vec3 p2 = ray.origin + ray.dir*roots.t2;

			if (!clipper->contains(p1)) roots.t1 = -1 * abs(roots.t1);
			if (!clipper->contains(p2)) roots.t2 = -1 * abs(roots.t2);

			if (!clipper2->contains(p1)) roots.t1 = -1 * abs(roots.t1);
			if (!clipper2->contains(p2)) roots.t2 = -1 * abs(roots.t2);

			float t = roots.getLesserPositive();

			Hit hit;
			hit.t = t;
			hit.material = material;
			hit.position = ray.origin + ray.dir * t;
			hit.normal = getNormalAt(hit.position);

			return hit;
		}

		virtual vec3 getNormalAt(vec3 r) {
			return plane->getNormalAt(r);
		}
};

class Square : public Intersectable {
	Faces *f1, *f2, *f3;
	public:
		Square(float size,Material* m) :Intersectable(m) {
			f1 = (new Faces(m))->F1(size);
			f2 = (new Faces(m))->F2(size);
			f3 = (new Faces(m))->F3(size);
		}

		Hit intersect(const Ray& ray) {
			Hit hits[3];
			hits[0] = f1->intersect(ray);
			hits[1] = f2->intersect(ray);
			hits[2] = f3->intersect(ray);

			Hit min = Hit();
			for (int i = 0; i < 3; i++) {
				if (min.t < 0 || (hits[i].t>0 && hits[i].t < min.t)) {
					min = hits[i];
				}
			}
			return min;
		}

		Square* Transform(mat4x4 t) {
			f1->Transform(t);
			f2->Transform(t);
			f3->Transform(t);
			return this;
		}
};
