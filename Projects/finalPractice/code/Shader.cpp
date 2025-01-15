
/*
	Public Domain Code

	Author: Xavier Canals
	Author: Ángel Rodríguez
*/

#include "Shader.hpp"



#include <SDL.h>



namespace finalPractice
{
	Shader::Shader(const std::string vertexShaderCode, const std::string fragmentShaderCode)
	{
		shaderID = compileShaders(vertexShaderCode, fragmentShaderCode);
	}

	Shader::~Shader()
	{
		glDeleteProgram(shaderID);
	}



	void Shader::use()
	{
		glUseProgram(shaderID);
	}

	GLuint Shader::getID()
	{
		return shaderID;
	}


	
	GLuint Shader::compileShaders(const std::string & vertexShaderCode, const std::string & fragmentShaderCode)
	{
		GLint succeeded = GL_FALSE;

		// Create the vertex and fragment shader objects
		GLuint   vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

		// Set the shader source code
		const char *   vertexShadersCode[] = {         vertexShaderCode.c_str() };
		const char * fragmentShadersCode[] = {       fragmentShaderCode.c_str() };
		const GLint    vertexShadersSize[] = { (GLint)  vertexShaderCode.size() };
		const GLint  fragmentShadersSize[] = { (GLint)fragmentShaderCode.size() };

		// Compile the shaders
		glShaderSource(  vertexShaderId, 1,   vertexShadersCode,   vertexShadersSize);
		glShaderSource(fragmentShaderId, 1, fragmentShadersCode, fragmentShadersSize);

		glCompileShader(  vertexShaderId);
		glCompileShader(fragmentShaderId);

		// Check for compilation errors
		glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &succeeded);
		if (!succeeded)
			showCompilationError(vertexShaderId);

		glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &succeeded);
		if (!succeeded)
			showCompilationError(fragmentShaderId);

		// Create the shader program and attach the shaders
		GLuint programID = glCreateProgram();

		glAttachShader(programID,   vertexShaderId);
		glAttachShader(programID, fragmentShaderId);

		// Link the program
		glLinkProgram(programID);

		// Check for linkage errors
		glGetShaderiv(programID, GL_LINK_STATUS, &succeeded);
		if (not succeeded)
			showLinkageError(programID);

		// Clean up the shader objects as they are no longer needed
		glDeleteShader(  vertexShaderId);
		glDeleteShader(fragmentShaderId);

		return programID;
	}

	void Shader::showCompilationError(GLuint shaderID)
	{
		static auto message = "Error while compiling a shader.";

		std::string       infoLog;
		GLint       infoLogLenght;

		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLenght);

		infoLog.resize(infoLogLenght);

		glGetShaderInfoLog(shaderID, infoLogLenght, NULL, &infoLog.front());

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, message, infoLog.c_str(), nullptr);

		throw message;
	}

	void Shader::showLinkageError(GLuint programID)
	{
		static auto message = "Error while linking shaders.";

		std::string       infoLog;
		GLint       infoLogLenght;

		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLenght);

		infoLog.resize(infoLogLenght);

		glGetProgramInfoLog(programID, infoLogLenght, NULL, &infoLog.front());

		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, message, infoLog.c_str(), nullptr);

		throw message;
	}
}
