#ifndef __TGEN_PLANE_H__
#define __TGEN_PLANE_H__

#include "Mesh.h"
#include "glm/glm.hpp"

class Plane :
	public Mesh
{
public:
	Plane();
	Plane(unsigned int w, unsigned int h, unsigned int lod, glm::vec2 pos);
	void CreatePlaneMesh(GLenum array_type, GLenum draw_type);
	void UpdatePlaneMesh(GLenum array_type);
	void RecalculateNormals();
	void ClearPlaneMesh();
	~Plane();
protected:
	unsigned int width, height;
	GLfloat* vertices;
	GLfloat* normals;
	GLuint* indices;
	GLuint vertexCount, indexCount;

	void CreatePlane();
private:
	glm::vec2 LocalPos;
	unsigned int LevelOfDetails;
};
#endif