#include "Plane.h"

Plane::Plane() : Mesh() {
	LevelOfDetails = 1;
	width = 1/LevelOfDetails;
	height = 1/LevelOfDetails;
	vertexCount = (width + 1)*(height + 1);
	indexCount = 6 * width * height;
	LocalPos = glm::vec2(0.0f, 0.0f);

	CreatePlane();
}

Plane::Plane(unsigned int w, unsigned int h, unsigned int lod, glm::vec2 pos) : Mesh() {
	LevelOfDetails = lod;
	width = w/lod;
	height = h/lod;
	vertexCount = (width + 1)*(height + 1);
	indexCount = 6 * width * height;
	LocalPos = pos;

	CreatePlane();
}

void Plane::CreatePlane() {
	vertices = new GLfloat[vertexCount * 3];
	normals = new GLfloat[vertexCount * 3];
	indices = new GLuint[indexCount];

	//for creating vertices...
	for (unsigned int j = 0; j <= height; j++) {
		for (unsigned int i = 0; i <= width; i++) {
			int ind = (j * (width+1) + i) * 3;
			//if plane is centered based---------------------------------------------------------
			/*vertices[ind] = (-(float)width / 2.0f + i)*(float)LevelOfDetails + LocalPos.x;
			vertices[ind + 1] = 0.0f;
			vertices[ind + 2] = (-(float)height / 2.0f + j)*(float)LevelOfDetails + LocalPos.y;*/

			//if plane is corner based-----------------------------------------------------------
			vertices[ind] = i*(float)LevelOfDetails + LocalPos.x;
			vertices[ind + 1] = 0.0f;
			vertices[ind + 2] = j*(float)LevelOfDetails + LocalPos.y;
		}
	}

	//for creatinf indices...
	int indpos = 0;
	for (unsigned int j = 0; j < height; j++) {
		for (unsigned int i = 0; i < width; i++) {
			int ind = j * (width+1) + i;
			//first triangle.
			indices[indpos] = ind;
			indices[indpos + 1] = ind + 1;
			indices[indpos + 2] = ind + width + 2;
			//second triangle.
			indices[indpos + 3] = ind;
			indices[indpos + 4] = ind + width + 2;
			indices[indpos + 5] = ind + width + 1;
			//increment to add next 6 inidces of next 2 triangles.
			indpos += 6;
		}
	}
}

void Plane::CreatePlaneMesh(GLenum array_type, GLenum draw_type) {
	//createMesh...
	if (array_type == GL_NORMAL_ARRAY_POINTER) CreateMesh(vertices, indices, normals, vertexCount * 3, indexCount, draw_type);
	else CreateMesh(vertices, indices, vertexCount * 3, indexCount, draw_type);
}

void Plane::UpdatePlaneMesh(GLenum array_type) {
	if (array_type == GL_NORMAL_ARRAY_POINTER) UpdateMesh(vertices, indices, normals, vertexCount * 3, indexCount);
	//else UpdateMesh(vertices, indices, vertexCount * 3, indexCount, draw_type);
}

void Plane::RecalculateNormals() {

	//reset normals to zero....
	for (unsigned int i = 0; i < vertexCount * 3; i++) {
		normals[i] = 0.0f;
	}

	for (unsigned int i = 0; i < indexCount; i+=3) {
		//vertex indices...
		unsigned int v1 = indices[i] * 3;
		unsigned int v2 = indices[i + 1] * 3;
		unsigned int v3 = indices[i + 2] * 3;

		//calculate surface vectors to get unit normal...
		glm::vec3 s1(vertices[v2] - vertices[v1], vertices[v2 + 1] - vertices[v1 + 1], vertices[v2 + 2] - vertices[v1 + 2]);
		glm::vec3 s2(vertices[v3] - vertices[v2], vertices[v3 + 1] - vertices[v2 + 1], vertices[v3 + 2] - vertices[v2 + 2]);
		glm::vec3 normal = glm::cross(s2, s1);
		normal = glm::normalize(normal);

		//add normal direction to its respective vertex index...
		normals[v1] += normal.x; normals[v1 + 1] += normal.y; normals[v1 + 2] += normal.z;
		normals[v2] += normal.x; normals[v2 + 1] += normal.y; normals[v2 + 2] += normal.z;
		normals[v2] += normal.x; normals[v3 + 1] += normal.y; normals[v3 + 2] += normal.z;
	}

	//average all normals on a vertex...
	for (unsigned int i = 0; i < vertexCount * 3; i += 3) {
		GLfloat mag = glm::sqrt(normals[i] * normals[i] + normals[i + 1] * normals[i + 1] + normals[i + 2] * normals[i + 2]);
		normals[i] /= mag;
		normals[i + 1] /= mag;
		normals[i + 2] /= mag;
	}
}

void Plane::ClearPlaneMesh() {
	delete[] vertices;
	delete[] indices;
	delete[] normals;
	ClearMesh();
}

Plane::~Plane() {
	ClearPlaneMesh();
}