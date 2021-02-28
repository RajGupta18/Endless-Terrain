#include "Mesh.h"
Mesh::Mesh() {
	VAO = 0;
	pos_VBO = 0;
	norm_VBO;
	IBO = 0;
	indicesCount = 0;
}

void Mesh::CreateMesh(GLfloat *vertices, GLuint* indices, GLuint vertexCnt, GLuint indCnt, GLenum draw_type) {
	indicesCount = indCnt;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indCnt, indices, draw_type);

	glGenBuffers(1, &pos_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertexCnt, vertices, draw_type);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::CreateMesh(GLfloat *vertices, GLuint* indices, GLfloat* normal, GLuint vertexCnt, GLuint indCnt, GLenum draw_type) {
	indicesCount = indCnt;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*indCnt, indices, draw_type);

	//for vertex position data----------------------------------------------------------
	glGenBuffers(1, &pos_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertexCnt, vertices, draw_type);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//----------------------------------------------------------------------------------

	//for vertex normal data------------------------------------------------------------
	glGenBuffers(1, &norm_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, norm_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*vertexCnt, normal, draw_type);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//----------------------------------------------------------------------------------
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::UpdateMesh(GLfloat *vertices, GLuint* indices, GLfloat* normal, GLuint vertexCnt, GLuint indCnt) {
	glBindVertexArray(VAO);
	//for vertex position data----------------------------------------------------------
	glBindBuffer(GL_ARRAY_BUFFER, pos_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*vertexCnt, vertices);
	//----------------------------------------------------------------------------------

	//for vertex normal data------------------------------------------------------------
	glBindBuffer(GL_ARRAY_BUFFER, norm_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat)*vertexCnt, normal);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//----------------------------------------------------------------------------------
	glBindVertexArray(0);
}

void Mesh::RenderMesh() {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indicesCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh() {
	if (IBO!=0) {
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}
	if (pos_VBO!=0)
	{
		glDeleteBuffers(1, &pos_VBO);
		pos_VBO = 0;
	}
	if (norm_VBO != 0) {
		glDeleteBuffers(1, &norm_VBO);
		norm_VBO = 0;
	}
	if (VAO!=0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
	indicesCount = 0;
}

Mesh::~Mesh() {
	ClearMesh();
}