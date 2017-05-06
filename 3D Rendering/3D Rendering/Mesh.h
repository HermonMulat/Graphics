#pragma once
#include <GL/glew.h>		 
#include <GL/freeglut.h>

#include <string>
#include <vector>
#include <fstream> 
#include "Maths.h"


class Mesh
{
	struct Face
	{
		int positionIndices[4];
		int normalIndices[4];
		int texcoordIndices[4];
		bool isQuad;
	};

	vector<string*> rows;
	vector<vec3*> positions;
	vector<vector<Face*> > submeshFaces;
	vector<vec3*> normals;
	vector<vec2*> texcoords;

	unsigned int vao;
	int nTriangles;

	public:
		Mesh(const char *filename){
			fstream file(filename);
			if (!file.is_open()){
				return;
			}
			char buffer[256];
			while (!file.eof()){
				file.getline(buffer, 256);
				rows.push_back(new string(buffer));
			}

			submeshFaces.push_back(vector<Face*>());
			vector<Face*>* faces = &submeshFaces.at(submeshFaces.size() - 1);

			for (int i = 0; i < rows.size(); i++){
				if (rows[i]->empty() || (*rows[i])[0] == '#')
					continue;
				else if ((*rows[i])[0] == 'v' && (*rows[i])[1] == ' '){
					float tmpx, tmpy, tmpz;
					sscanf(rows[i]->c_str(), "v %f %f %f", &tmpx, &tmpy, &tmpz);
					positions.push_back(new vec3(tmpx, tmpy, tmpz));
				}
				else if ((*rows[i])[0] == 'v' && (*rows[i])[1] == 'n'){
					float tmpx, tmpy, tmpz;
					sscanf(rows[i]->c_str(), "vn %f %f %f", &tmpx, &tmpy, &tmpz);
					normals.push_back(new vec3(tmpx, tmpy, tmpz));
				}
				else if ((*rows[i])[0] == 'v' && (*rows[i])[1] == 't'){
					float tmpx, tmpy;
					sscanf(rows[i]->c_str(), "vt %f %f", &tmpx, &tmpy);
					texcoords.push_back(new vec2(tmpx, tmpy));
				}
				else if ((*rows[i])[0] == 'f'){
					if (count(rows[i]->begin(), rows[i]->end(), ' ') == 3){
						Face* f = new Face();
						f->isQuad = false;
						sscanf(rows[i]->c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
							&f->positionIndices[0], &f->texcoordIndices[0], &f->normalIndices[0],
							&f->positionIndices[1], &f->texcoordIndices[1], &f->normalIndices[1],
							&f->positionIndices[2], &f->texcoordIndices[2], &f->normalIndices[2]);
						faces->push_back(f);
					}
					else{
						Face* f = new Face();
						f->isQuad = true;
						sscanf(rows[i]->c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d",
							&f->positionIndices[0], &f->texcoordIndices[0], &f->normalIndices[0],
							&f->positionIndices[1], &f->texcoordIndices[1], &f->normalIndices[1],
							&f->positionIndices[2], &f->texcoordIndices[2], &f->normalIndices[2],
							&f->positionIndices[3], &f->texcoordIndices[3], &f->normalIndices[3]);
						faces->push_back(f);
					}
				}
				else if ((*rows[i])[0] == 'g'){
					if (faces->size() > 0){
						submeshFaces.push_back(vector<Face*>());
						faces = &submeshFaces.at(submeshFaces.size() - 1);
					}
				}
			}

			int numberOfTriangles = 0;
			for (int iSubmesh = 0; iSubmesh<submeshFaces.size(); iSubmesh++){
				vector<Face*>& faces = submeshFaces.at(iSubmesh);

				for (int i = 0; i<faces.size(); i++){
					if (faces[i]->isQuad) numberOfTriangles += 2;
					else numberOfTriangles += 1;
				}
			}

			nTriangles = numberOfTriangles;

			float *vertexCoords = new float[numberOfTriangles * 9];
			float *vertexTexCoords = new float[numberOfTriangles * 6];
			float *vertexNormalCoords = new float[numberOfTriangles * 9];


			int triangleIndex = 0;
			for (int iSubmesh = 0; iSubmesh<submeshFaces.size(); iSubmesh++){
				vector<Face*>& faces = submeshFaces.at(iSubmesh);

				for (int i = 0; i<faces.size(); i++){
					if (faces[i]->isQuad){
						vertexTexCoords[triangleIndex * 6] = texcoords[faces[i]->texcoordIndices[0] - 1]->x;
						vertexTexCoords[triangleIndex * 6 + 1] = 1 - texcoords[faces[i]->texcoordIndices[0] - 1]->y;

						vertexTexCoords[triangleIndex * 6 + 2] = texcoords[faces[i]->texcoordIndices[1] - 1]->x;
						vertexTexCoords[triangleIndex * 6 + 3] = 1 - texcoords[faces[i]->texcoordIndices[1] - 1]->y;

						vertexTexCoords[triangleIndex * 6 + 4] = texcoords[faces[i]->texcoordIndices[2] - 1]->x;
						vertexTexCoords[triangleIndex * 6 + 5] = 1 - texcoords[faces[i]->texcoordIndices[2] - 1]->y;


						vertexCoords[triangleIndex * 9] = positions[faces[i]->positionIndices[0] - 1]->x;
						vertexCoords[triangleIndex * 9 + 1] = positions[faces[i]->positionIndices[0] - 1]->y;
						vertexCoords[triangleIndex * 9 + 2] = positions[faces[i]->positionIndices[0] - 1]->z;

						vertexCoords[triangleIndex * 9 + 3] = positions[faces[i]->positionIndices[1] - 1]->x;
						vertexCoords[triangleIndex * 9 + 4] = positions[faces[i]->positionIndices[1] - 1]->y;
						vertexCoords[triangleIndex * 9 + 5] = positions[faces[i]->positionIndices[1] - 1]->z;

						vertexCoords[triangleIndex * 9 + 6] = positions[faces[i]->positionIndices[2] - 1]->x;
						vertexCoords[triangleIndex * 9 + 7] = positions[faces[i]->positionIndices[2] - 1]->y;
						vertexCoords[triangleIndex * 9 + 8] = positions[faces[i]->positionIndices[2] - 1]->z;


						vertexNormalCoords[triangleIndex * 9] = normals[faces[i]->normalIndices[0] - 1]->x;
						vertexNormalCoords[triangleIndex * 9 + 1] = normals[faces[i]->normalIndices[0] - 1]->y;
						vertexNormalCoords[triangleIndex * 9 + 2] = normals[faces[i]->normalIndices[0] - 1]->z;

						vertexNormalCoords[triangleIndex * 9 + 3] = normals[faces[i]->normalIndices[1] - 1]->x;
						vertexNormalCoords[triangleIndex * 9 + 4] = normals[faces[i]->normalIndices[1] - 1]->y;
						vertexNormalCoords[triangleIndex * 9 + 5] = normals[faces[i]->normalIndices[1] - 1]->z;

						vertexNormalCoords[triangleIndex * 9 + 6] = normals[faces[i]->normalIndices[2] - 1]->x;
						vertexNormalCoords[triangleIndex * 9 + 7] = normals[faces[i]->normalIndices[2] - 1]->y;
						vertexNormalCoords[triangleIndex * 9 + 8] = normals[faces[i]->normalIndices[2] - 1]->z;

						triangleIndex++;


						vertexTexCoords[triangleIndex * 6] = texcoords[faces[i]->texcoordIndices[1] - 1]->x;
						vertexTexCoords[triangleIndex * 6 + 1] = 1 - texcoords[faces[i]->texcoordIndices[1] - 1]->y;

						vertexTexCoords[triangleIndex * 6 + 2] = texcoords[faces[i]->texcoordIndices[2] - 1]->x;
						vertexTexCoords[triangleIndex * 6 + 3] = 1 - texcoords[faces[i]->texcoordIndices[2] - 1]->y;

						vertexTexCoords[triangleIndex * 6 + 4] = texcoords[faces[i]->texcoordIndices[3] - 1]->x;
						vertexTexCoords[triangleIndex * 6 + 5] = 1 - texcoords[faces[i]->texcoordIndices[3] - 1]->y;


						vertexCoords[triangleIndex * 9] = positions[faces[i]->positionIndices[1] - 1]->x;
						vertexCoords[triangleIndex * 9 + 1] = positions[faces[i]->positionIndices[1] - 1]->y;
						vertexCoords[triangleIndex * 9 + 2] = positions[faces[i]->positionIndices[1] - 1]->z;

						vertexCoords[triangleIndex * 9 + 3] = positions[faces[i]->positionIndices[2] - 1]->x;
						vertexCoords[triangleIndex * 9 + 4] = positions[faces[i]->positionIndices[2] - 1]->y;
						vertexCoords[triangleIndex * 9 + 5] = positions[faces[i]->positionIndices[2] - 1]->z;

						vertexCoords[triangleIndex * 9 + 6] = positions[faces[i]->positionIndices[3] - 1]->x;
						vertexCoords[triangleIndex * 9 + 7] = positions[faces[i]->positionIndices[3] - 1]->y;
						vertexCoords[triangleIndex * 9 + 8] = positions[faces[i]->positionIndices[3] - 1]->z;


						vertexNormalCoords[triangleIndex * 9] = normals[faces[i]->normalIndices[1] - 1]->x;
						vertexNormalCoords[triangleIndex * 9 + 1] = normals[faces[i]->normalIndices[1] - 1]->y;
						vertexNormalCoords[triangleIndex * 9 + 2] = normals[faces[i]->normalIndices[1] - 1]->z;

						vertexNormalCoords[triangleIndex * 9 + 3] = normals[faces[i]->normalIndices[2] - 1]->x;
						vertexNormalCoords[triangleIndex * 9 + 4] = normals[faces[i]->normalIndices[2] - 1]->y;
						vertexNormalCoords[triangleIndex * 9 + 5] = normals[faces[i]->normalIndices[2] - 1]->z;

						vertexNormalCoords[triangleIndex * 9 + 6] = normals[faces[i]->normalIndices[3] - 1]->x;
						vertexNormalCoords[triangleIndex * 9 + 7] = normals[faces[i]->normalIndices[3] - 1]->y;
						vertexNormalCoords[triangleIndex * 9 + 8] = normals[faces[i]->normalIndices[3] - 1]->z;

						triangleIndex++;
					}
					else{
						vertexTexCoords[triangleIndex * 6] = texcoords[faces[i]->texcoordIndices[0] - 1]->x;
						vertexTexCoords[triangleIndex * 6 + 1] = 1 - texcoords[faces[i]->texcoordIndices[0] - 1]->y;

						vertexTexCoords[triangleIndex * 6 + 2] = texcoords[faces[i]->texcoordIndices[1] - 1]->x;
						vertexTexCoords[triangleIndex * 6 + 3] = 1 - texcoords[faces[i]->texcoordIndices[1] - 1]->y;

						vertexTexCoords[triangleIndex * 6 + 4] = texcoords[faces[i]->texcoordIndices[2] - 1]->x;
						vertexTexCoords[triangleIndex * 6 + 5] = 1 - texcoords[faces[i]->texcoordIndices[2] - 1]->y;

						vertexCoords[triangleIndex * 9] = positions[faces[i]->positionIndices[0] - 1]->x;
						vertexCoords[triangleIndex * 9 + 1] = positions[faces[i]->positionIndices[0] - 1]->y;
						vertexCoords[triangleIndex * 9 + 2] = positions[faces[i]->positionIndices[0] - 1]->z;

						vertexCoords[triangleIndex * 9 + 3] = positions[faces[i]->positionIndices[1] - 1]->x;
						vertexCoords[triangleIndex * 9 + 4] = positions[faces[i]->positionIndices[1] - 1]->y;
						vertexCoords[triangleIndex * 9 + 5] = positions[faces[i]->positionIndices[1] - 1]->z;

						vertexCoords[triangleIndex * 9 + 6] = positions[faces[i]->positionIndices[2] - 1]->x;
						vertexCoords[triangleIndex * 9 + 7] = positions[faces[i]->positionIndices[2] - 1]->y;
						vertexCoords[triangleIndex * 9 + 8] = positions[faces[i]->positionIndices[2] - 1]->z;


						vertexNormalCoords[triangleIndex * 9] = normals[faces[i]->normalIndices[0] - 1]->x;
						vertexNormalCoords[triangleIndex * 9 + 1] = normals[faces[i]->normalIndices[0] - 1]->y;
						vertexNormalCoords[triangleIndex * 9 + 2] = normals[faces[i]->normalIndices[0] - 1]->z;

						vertexNormalCoords[triangleIndex * 9 + 3] = normals[faces[i]->normalIndices[1] - 1]->x;
						vertexNormalCoords[triangleIndex * 9 + 4] = normals[faces[i]->normalIndices[1] - 1]->y;
						vertexNormalCoords[triangleIndex * 9 + 5] = normals[faces[i]->normalIndices[1] - 1]->z;

						vertexNormalCoords[triangleIndex * 9 + 6] = normals[faces[i]->normalIndices[2] - 1]->x;
						vertexNormalCoords[triangleIndex * 9 + 7] = normals[faces[i]->normalIndices[2] - 1]->y;
						vertexNormalCoords[triangleIndex * 9 + 8] = normals[faces[i]->normalIndices[2] - 1]->z;

						triangleIndex++;
					}
				}
			}

			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			unsigned int vbo[3];
			glGenBuffers(3, &vbo[0]);

			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, nTriangles * 9 * sizeof(float), vertexCoords, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			glBufferData(GL_ARRAY_BUFFER, nTriangles * 6 * sizeof(float), vertexTexCoords, GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

			glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
			glBufferData(GL_ARRAY_BUFFER, nTriangles * 9 * sizeof(float), vertexNormalCoords, GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

			delete vertexCoords;
			delete vertexTexCoords;
			delete vertexNormalCoords;
		}

		~Mesh(){
			for (unsigned int i = 0; i < rows.size(); i++) delete rows[i];
			for (unsigned int i = 0; i < positions.size(); i++) delete positions[i];
			for (unsigned int i = 0; i < submeshFaces.size(); i++)
				for (unsigned int j = 0; j < submeshFaces.at(i).size(); j++)
					delete submeshFaces.at(i).at(j);
			for (unsigned int i = 0; i < normals.size(); i++) delete normals[i];
			for (unsigned int i = 0; i < texcoords.size(); i++) delete texcoords[i];
		}

		void DrawModel(){
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, nTriangles * 3);
		}
	
};