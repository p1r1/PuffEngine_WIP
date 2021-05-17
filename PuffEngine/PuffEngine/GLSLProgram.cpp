#include "GLSLProgram.h"
#include "PuffErrors.h"
#include "IOManager.h"

#include <fstream>
#include <vector>

namespace PuffEngine {
GLSLProgram::GLSLProgram() :
	numAttributes_(0),
	programID_(0),
	vertexShaderID_(0),
	fragmentShaderID_(0) {
}


GLSLProgram::~GLSLProgram() {
}

void GLSLProgram::compileShaders(const std::string & vertexShaderFilePath, const std::string & fragmentShaderFilePath) {
	// get source code
	std::string vertSource;
	std::string fragSource;
	
	IOManager::readFileToBuffer(vertexShaderFilePath, vertSource);
	IOManager::readFileToBuffer(fragmentShaderFilePath, fragSource);

	compileShadersFromSource(vertSource.c_str(), fragSource.c_str());

}

void GLSLProgram::compileShadersFromSource(const char* vertexSource, const char* fragmentSource) {

	//open file, read data, compile shaders
	//create id before bind addatribute222
	programID_ = glCreateProgram();

	//create shader file
	vertexShaderID_ = glCreateShader(GL_VERTEX_SHADER);
	if (vertexShaderID_ == 0) {
		fatalError("Vertex shader fail to be created!");
	}

	fragmentShaderID_ = glCreateShader(GL_FRAGMENT_SHADER);
	if (fragmentShaderID_ == 0) {
		fatalError("Fragment shader fail to be created!");
	}
	compileShader(vertexSource, "Vertex Shader", vertexShaderID_);
	compileShader(fragmentSource, "Fragment Shader", fragmentShaderID_);
}

void GLSLProgram::linkShaders() {
	// Vertex and fragment shaders are successfully compiled.
	// Now time to link them together into a program.
	// Get a program object.
	// Attach our shaders to our program
	glAttachShader(programID_, vertexShaderID_);
	glAttachShader(programID_, fragmentShaderID_);

	// Link our program
	glLinkProgram(programID_);

	// Note the different functions here: glGetProgram* instead of glGetShader*.
	//error check
	GLint isLinked = 0;
	glGetProgramiv(programID_, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(programID_, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetProgramInfoLog(programID_, maxLength, &maxLength, &errorLog[0]);

		// We don't need the program anymore.
		glDeleteProgram(programID_);
		// Don't leak shaders either.
		glDeleteShader(vertexShaderID_);
		glDeleteShader(fragmentShaderID_);

		// Use the infoLog as you see fit.

		// In this simple program, we'll just leave
		std::printf("%s\n", &(errorLog[0]));
		fatalError("Shader failed to link!");

	}

	// Always detach shaders after a successful link.
	glDetachShader(programID_, vertexShaderID_);
	glDetachShader(programID_, fragmentShaderID_);
	glDeleteShader(vertexShaderID_);
	glDeleteShader(fragmentShaderID_);

}

void GLSLProgram::addAttribute(const std::string & attributeName) {
	//0 --> first attrib (position)
	glBindAttribLocation(programID_, numAttributes_++, attributeName.c_str());
}

GLint GLSLProgram::getUniformLocation(const std::string& uniformName) {
	GLint location = glGetUniformLocation(programID_, uniformName.c_str());
	if (location == GL_INVALID_INDEX) {
		fatalError("Uniform " + uniformName + " not found in shader");
	}
	return location;
}

void GLSLProgram::use() {
	glUseProgram(programID_);
	//enable each vertex attribute
	for (int i = 0; i < numAttributes_; i++) {
		glEnableVertexAttribArray(i);
	}
}

void GLSLProgram::unUse() {
	glUseProgram(0);
	//disable each vertex attributes
	for (int i = 0; i < numAttributes_; i++) {
		glDisableVertexAttribArray(i);
	}
}

void GLSLProgram::dispose() {
	if (programID_ != 0) glDeleteProgram(programID_);
}

void GLSLProgram::compileShader(const char* source, const std::string& name, GLuint id) {
	

	//convert string to source code
	//1 --> all of the text in 1 string
	//nullptr--> we are using 1 sting 
	glShaderSource(id, 1, &source, nullptr);
	//compile shader
	glCompileShader(id);

	//error check
	GLint isCompiled = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		// The maxLength includes the NULL character
		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		// Provide the infolog in whatever manor you deem best.
		// Exit with failure.
		glDeleteShader(id); // Don't leak the shader.
		std::printf("%s\n", &(errorLog[0]));
		fatalError("Shader " + name + " failed to compile!");
	}
}

}