#ifndef __TGEN_SHADER_H__
#define __TGEN_SHADER_H__

#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader();
	void CreateFromFile(const char* vertexLoc, const char* fragLoc);
	void CreateFromString(const char* vertexCode, const char* fragmentCode);
	std::string ReadFromFile(const char* fileLocation);

	void UseShader();
	void ClearShader();

	//Set uniform variables functions.....
	void SetMat4(std::string name, glm::mat4 matrix);
	void SetFloat1(std::string name, GLfloat value);
	void SetFloat3(std::string name, GLfloat xVal, GLfloat yVal, GLfloat zVal);

	~Shader();

private:
	GLuint ShaderID;

	void CompileShader(const char* vertexCode, const char* fragmentCode);
	GLuint AddShader(GLuint program, const char* shaderCode, GLenum shader_type);
};
#endif