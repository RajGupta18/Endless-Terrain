#include "Plane.h"

Plane::Plane() : Mesh() {
	LevelOfDetails = 1;
	meshSize = 1/LevelOfDetails;
	borderSize = meshSize + 2;
	vertexCount = (meshSize + 1)*(meshSize + 1);
	indexCount = 6 * meshSize * meshSize;
	LocalPos = glm::vec2(0.0f, 0.0f);

	CreatePlane();
}

Plane::Plane(int size, unsigned int lod, glm::vec2 pos) : Mesh() {
	LevelOfDetails = lod;
	meshSize = size/lod;
	borderSize = meshSize + 2;
	indexCount = 6 * meshSize * meshSize;
	vertexCount = (meshSize + 1)*(meshSize + 1);
	LocalPos = pos;

	CreatePlane();
}

void Plane::CreatePlane() {
	vertices = new GLfloat[vertexCount * 3];
	normals = new GLfloat[vertexCount * 3];
	indices = new GLuint[indexCount];

	verticesIndexMap = new int[(borderSize + 1)*(borderSize + 1)];

	//(4*(meshSize+1) + 4) is no of vertices in border each having 3 float values...
	borderVertices = new GLfloat[12 * (meshSize + 1) + 12];

	//create Vertices Index Map...
	int borderIndex = -1;
	int meshIndex = 0;
	for (int i = 0; i <= borderSize; i++) {
		for (int j = 0; j <= borderSize; j++) {
			int index = i * (borderSize + 1) + j;
			if (i == 0 || j == 0 || i == borderSize || j == borderSize) {
				verticesIndexMap[index] = borderIndex;
				borderIndex--;
			}
			else {
				verticesIndexMap[index] = meshIndex;
				meshIndex++;
			}
		}
	}

	//for creating vertices (Corner based)...
	for (int i = 0; i <= borderSize; i++) {
		for (int j = 0; j <= borderSize; j++) {
			int vertexIndex = verticesIndexMap[i * (borderSize + 1) + j];

			if (vertexIndex >= 0) {	//mesh vertices...
				int ind = vertexIndex * 3;
				vertices[ind] = (i - 1) * (float)LevelOfDetails + LocalPos.x;
				vertices[ind + 1] = 0.0f;
				vertices[ind + 2] = (j - 1) * (float)LevelOfDetails + LocalPos.y;
			}
			else {	//border vertices...
				int ind = -(vertexIndex + 1) * 3;
				borderVertices[ind] = (i - 1) * (float)LevelOfDetails + LocalPos.x;
				borderVertices[ind + 1] = 0.0f;
				borderVertices[ind + 2] = (j - 1) * (float)LevelOfDetails + LocalPos.y;
			}
		}
	}


	//for creating indices...
	int indpos = 0;
	for (int i = 0; i < meshSize; i++) {
		for (int j = 0; j < meshSize; j++) {
			int ind = i * (meshSize+1) + j;
			//first triangle.
			indices[indpos] = ind;
			indices[indpos + 1] = ind + 1;
			indices[indpos + 2] = ind + meshSize + 2;
			//second triangle.
			indices[indpos + 3] = ind;
			indices[indpos + 4] = ind + meshSize + 2;
			indices[indpos + 5] = ind + meshSize + 1;
			//increment to add next 6 inidces of next 2 triangles.
			indpos += 6;
		}
	}
}

void Plane::CreatePlaneMesh(GLenum draw_type) {
	//Load mesh in GPU...
	CreateMesh(vertices, indices, normals, vertexCount * 3, indexCount, draw_type);
}

void Plane::UpdatePlaneMesh() {
	UpdateMesh(vertices, indices, normals, vertexCount * 3, indexCount);
	//else UpdateMesh(vertices, indices, vertexCount * 3, indexCount, draw_type);
}

void Plane::RecalculateNormals() {
	//reset normals to zero....
	for (unsigned int i = 0; i < vertexCount * 3; i++) {
		normals[i] = 0.0f;
	}

	for (int i = 0; i < borderSize; i++) {
		for (int j = 0; j < borderSize; j++) {
			// vertex indices...
			int a = verticesIndexMap[i * (borderSize + 1) + j];
			int b = verticesIndexMap[i * (borderSize + 1) + j + 1];
			int c = verticesIndexMap[(i + 1) * (borderSize + 1) + j + 1];
			int d = verticesIndexMap[(i + 1) * (borderSize + 1) + j];

			glm::vec3 avec;
			if (a < 0) {
				int ind = -(a + 1) * 3;
				avec = glm::vec3(borderVertices[ind], borderVertices[ind + 1], borderVertices[ind + 2]);
			}
			else {
				int ind = a * 3;
				avec = glm::vec3(vertices[ind], vertices[ind + 1], vertices[ind + 2]);
			}

			glm::vec3 bvec;
			if (b < 0) {
				int ind = -(b + 1) * 3;
				bvec = glm::vec3(borderVertices[ind], borderVertices[ind + 1], borderVertices[ind + 2]);
			}
			else {
				int ind = b * 3;
				bvec = glm::vec3(vertices[ind], vertices[ind + 1], vertices[ind + 2]);
			}

			glm::vec3 cvec;
			if (c < 0) {
				int ind = -(c + 1) * 3;
				cvec = glm::vec3(borderVertices[ind], borderVertices[ind + 1], borderVertices[ind + 2]);
			}
			else {
				int ind = c * 3;
				cvec = glm::vec3(vertices[ind], vertices[ind + 1], vertices[ind + 2]);
			}

			glm::vec3 dvec;
			if (d < 0) {
				int ind = -(d + 1) * 3;
				dvec = glm::vec3(borderVertices[ind], borderVertices[ind + 1], borderVertices[ind + 2]);
			}
			else {
				int ind = d * 3;
				dvec = glm::vec3(vertices[ind], vertices[ind + 1], vertices[ind + 2]);
			}

			glm::vec3 normal1 = glm::normalize(-glm::cross(cvec - bvec, bvec - avec));
			glm::vec3 normal2 = glm::normalize(-glm::cross(avec - dvec, dvec - cvec));

			if (a >= 0) {
				int ind = a * 3;
				normals[ind] += (normal1.x + normal2.x);
				normals[ind + 1] += (normal1.y + normal2.y);
				normals[ind + 2] += (normal1.z + normal2.z);
			}
			if (c >= 0) {
				int ind = c * 3;
				normals[ind] += (normal1.x + normal2.x);
				normals[ind + 1] += (normal1.y + normal2.y);
				normals[ind + 2] += (normal1.z + normal2.z);
			}
			if (b >= 0) {
				int ind = b * 3;
				normals[ind] += normal1.x;
				normals[ind + 1] += normal1.y;
				normals[ind + 2] += normal1.z;
			}
			if (d >= 0) {
				int ind = d * 3;
				normals[ind] += normal2.x;
				normals[ind + 1] += normal2.y;
				normals[ind + 2] += normal2.z;
			}
		}
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
	delete[] verticesIndexMap;
	delete[] borderVertices;
	ClearMesh();
}

Plane::~Plane() {
	ClearPlaneMesh();
}