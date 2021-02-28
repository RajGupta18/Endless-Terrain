#include "Shader.h"

Shader::Shader() {
	ShaderID = 0;
}

#pragma region READFILE_AND_STORE_SHADER_CODE
std::string Shader::ReadFromFile(const char* fileLocation) {
	std::string content;
	std::ifstream fileStream(fileLocation, std::ios::in);

	if (!fileStream.is_open()) {
		std::cout << "Failed to open " << fileLocation << " !" << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}

void Shader::CreateFromFile(const char* vertexLoc, const char* fragLoc) {
	std::string vertexCode = ReadFromFile(vertexLoc);
	std::string fragmentCode = ReadFromFile(fragLoc);
	CompileShader(vertexCode.c_str(), fragmentCode.c_str());
}

void Shader::CreateFromString(const char* vertexCode, const char* fragmentCode) {
	CompileShader(vertexCode, fragmentCode);
}
#pragma endregion

#pragma region CREATE_SHADER_PROGRAM
GLuint Shader::AddShader(GLuint program, const char* shaderCode, GLenum shader_type) {
	GLuint shader = glCreateShader(shader_type);

	const GLchar* code[1];
	code[0] = shaderCode;

	GLint codelength[1];
	codelength[0] = strlen(shaderCode);

	glShaderSource(shader, 1, code, codelength);
	glCompileShader(shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(shader, sizeof(eLog), NULL, eLog);
		std::cout << "Error Compiling " << shader_type << " Program: " << eLog << std::endl;
		return 0;
	}
	glAttachShader(program, shader);
	return shader;
}

void Shader::CompileShader(const char* vertexCode, const char* fragmentCode) {
	ShaderID = glCreateProgram();
	if (!ShaderID) {
		std::cout << "Failed to initialise Shader Program!" << std::endl;
		return;
	}
	GLuint vShaderID = AddShader(ShaderID, vertexCode, GL_VERTEX_SHADER);
	GLuint fShaderID = AddShader(ShaderID, fragmentCode, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(ShaderID);
	glGetProgramiv(ShaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(ShaderID, sizeof(eLog), NULL, eLog);
		std::cout << "Error Linking Program: " << eLog << std::endl;
		return;
	}

	glValidateProgram(ShaderID);
	glGetProgramiv(ShaderID, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(ShaderID, sizeof(eLog), NULL, eLog);
		std::cout << "Error Validating Program: " << eLog << std::endl;
		return;
	}

	//Delete shaders after creating program (because already copied in program)...
	glDetachShader(ShaderID, vShaderID);
	glDeleteShader(vShaderID);
	glDetachShader(ShaderID, fShaderID);
	glDeleteShader(fShaderID);
}
#pragma endregion

void Shader::UseShader() {
	glUseProgram(ShaderID);
}

void Shader::SetMat4(std::string name, glm::mat4 matrix) {
	glUniformMatrix4fv(glGetUniformLocation(ShaderID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::SetFloat1(std::string name, GLfloat value) {
	glUniform1f(glGetUniformLocation(ShaderID, name.c_str()), value);
}

void Shader::SetFloat3(std::string name, GLfloat xVal, GLfloat yVal, GLfloat zVal) {
	glUniform3f(glGetUniformLocation(ShaderID, name.c_str()), xVal, yVal, zVal);
}

void Shader::ClearShader() {
	if (ShaderID) {
		glDeleteProgram(ShaderID);
		ShaderID = 0;
	}
}

Shader::~Shader() {
	ClearShader();
}