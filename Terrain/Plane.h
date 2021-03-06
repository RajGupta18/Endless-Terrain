#ifndef __TGEN_PLANE_H__
#define __TGEN_PLANE_H__

#include "Mesh.h"
#include "glm/glm.hpp"

class Plane :
	public Mesh
{
public:
	Plane();
	Plane(int size, unsigned int lod, glm::vec2 pos);
	void CreatePlane();
	void CreatePlaneMesh(GLenum draw_type);
	void UpdatePlaneMesh();
	void RecalculateNormals();
	void ClearPlaneMesh();
	void DeletePlane();
	~Plane();
protected:
	int meshSize;
	int borderSize;
	GLfloat* vertices;
	GLfloat* normals;
	GLuint* indices;
	GLuint vertexCount, indexCount;

	int* verticesIndexMap;
	GLfloat* borderVertices;

private:
	glm::vec2 LocalPos;
	unsigned int LevelOfDetails;
};
#endif