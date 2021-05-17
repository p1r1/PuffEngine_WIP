#pragma once
#include <string>
#include <GL/glew.h>

namespace PuffEngine {
class GLSLProgram {
public:
	// refrence:: https://www.khronos.org/opengl/wiki/Shader_Compilation
	GLSLProgram();
	~GLSLProgram();

	//read shaders from file and  compile them for gpu can read
	// these file paths never going to change so they are CONST
	//we not want to copy file path variable so they are refrence parameter &
	void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

	void compileShadersFromSource(const char* vertexSource, const char* fragmentSource);

	// final shader program
	void linkShaders();
	//bind the attributes like positions colors and
	void addAttribute(const std::string& attributeName);

	GLint getUniformLocation(const std::string& uniformName);

	void use();
	void unUse();

	void dispose();
private:
	void compileShader(const char* source, const std::string& name, GLuint id);

	int numAttributes_;
	//combine program
	GLuint programID_;

	GLuint vertexShaderID_;
	GLuint fragmentShaderID_;
};

}