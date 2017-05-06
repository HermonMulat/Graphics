#pragma once
#include "Object.h"
#include "Maths.h"
#include "Texture.h"
#include "Mesh.h"


class MeshInstance : public Object
{
	Texture *texture;
	Mesh * mesh;

public:

	MeshInstance(Texture* t, Mesh* m, const string& sp) : Object(sp), texture(t), mesh(m)
	{
		scaling = vec3(0.018, 0.018, 0.018);
		position = vec3(0.0, 0.0, 0.5);
		angularVelocity = 0;
	}

	virtual void DrawModel(){
		texture->Bind(shader);
		glEnable(GL_DEPTH_TEST);
		mesh->DrawModel();
		glDisable(GL_DEPTH_TEST);
	}
};

class Tree : public MeshInstance {
public:
	Tree(Texture* t, Mesh* m, const string& sp) : MeshInstance(t, m, sp) {
		scaling = vec3(0.016, 0.016, 0.008);
		position = vec3(0.5, 0.0, -0.03);
		angularVelocity = 0;
	}
	void setPosition(const vec3& p) {
		position = p;
	}
	void setScaling(const vec3& s) {
		scaling = s;
	}
};