#ifndef __TGEN_MESH_H__
#define __TGEN_MESH_H__

#include<GL/glew.h>

class Mesh
{
public:
	Mesh();
	void CreateMesh(GLfloat *vertices, GLuint* indices, GLuint vertexCnt, GLuint indCnt, GLenum draw_type);
	void CreateMesh(GLfloat *vertices, GLuint* indices, GLfloat* normal, GLuint vertexCnt, GLuint indCnt, GLenum draw_type);
	void UpdateMesh(GLfloat *vertices, GLuint* indices, GLfloat* normal, GLuint vertexCnt, GLuint indCnt);
	void RenderMesh();
	void ClearMesh();
	~Mesh();
private:
	GLuint VAO, pos_VBO, norm_VBO, IBO;
	GLsizei indicesCount;
};
#endif