
// Public Domain Code
// Author: Xavier Canals

#pragma once



#ifndef SHADER_HEADER
#define SHADER_HEADER



#include <glad/glad.h>
#include <string>



namespace meshCreation
{
	GLuint compileShaders(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);

	void   showCompilationError(GLuint  shaderID);

	void   showLinkageError(GLuint programID);
}



#endif
