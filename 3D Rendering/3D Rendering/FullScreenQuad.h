#pragma once
#include "Object.h"
#include "Maths.h"
#include "Texture.h"

#include "GL/glew.h"
#include "GL/freeglut.h"


class FullScreenQuad : public Object {
	unsigned int vao;// vertex array object id, stores texture coordinates rather than color

public:

	FullScreenQuad(const string& sp) : Object(sp) {

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		unsigned int vbo;

		// generate 3 vertex buffer objects (pos, tex coords, normals)
		glGenBuffers(1, &vbo);

		// vertex point mapping					 
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		static float vertexCoords[] = {	 0,0,0,1,
										-1,1,0,1,
										 1,1,0,1,
										 1,-1,0,1,
										-1,-1,0,1,
										-1,1,0,1};


		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), vertexCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	}

	void SetTransform(Camera* C) {
		mat4 viewDirMatrix = C->GetInverseProjectionMatrix() * C->GetInverseViewMatrix();

		int location = glGetUniformLocation(shader, "viewDirMatrix");
		if (location >= 0) glUniformMatrix4fv(location, 1, GL_TRUE, viewDirMatrix);
		else printf("uniform viewDirMatrix cannot be set\n");

	}

	virtual void Draw(Light* light, Camera* camera, TextureCube* Env) {
		//glDepthMask(GL_FALSE);
		glUseProgram(shader);
		Env->Bind(shader);
		SetTransform(camera);
		DrawModel();
		//glDepthMask(GL_TRUE);
	}

	virtual void DrawModel() {
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glDisable(GL_DEPTH_TEST);
	}

	virtual void Interact(Object* o) {
	}
};