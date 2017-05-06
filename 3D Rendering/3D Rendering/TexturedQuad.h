#pragma once
#include "Object.h"
#include "Maths.h"
#include "Texture.h"
#include "Mesh.h"

#include "GL/glew.h"
#include "GL/freeglut.h"


class TexturedQuad : public Object {
	Texture *texture;
	unsigned int vao;// vertex array object id, stores texture coordinates rather than color

public:

	TexturedQuad(Material * mat, Texture* t, const string& sp ) : Object(sp), texture(t){
		material = mat;
		
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		unsigned int vbo[3];

		// generate 3 vertex buffer objects (pos, tex coords, normals)
		glGenBuffers(3, &vbo[0]);

		// vertex point mapping					 
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); 
		static float vertexCoords[] = {  0,0,0,1,
										-0.125,0,0.125,  0,
										 0.125,0,0.125,  0,
										 0.125,0,-0.125, 0,
										-0.125,0,-0.125, 0,
										-0.125,0,0.125,  0};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), vertexCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);//specify 4 for 4D (for vertex coords)
		
		// texture cord mapping
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); 
		static float vertexTexCoord[] = {0.5,0.5,  0,0,  1,0,  1,1,  0,1,  0,0};
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexTexCoord), vertexTexCoord, GL_STATIC_DRAW);																			  
		glEnableVertexAttribArray(1);  
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);


		// normal vector mapping
		glBindBuffer(GL_ARRAY_BUFFER, vbo[2]); 
		static float vertexNormal[] = { 0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0,    0, 1, 0 };
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexNormal), vertexNormal, GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	}

	virtual void DrawModel(){
		texture->Bind(shader); 
		glEnable(GL_DEPTH_TEST);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
		glDisable(GL_DEPTH_TEST);

	}

	virtual void Interact(Object* o) {
	}
};