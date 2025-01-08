
// Public Domain Code
// Author: Xavier Canals

#include "Shader.hpp"



#include <SDL.h>



namespace meshCreation
{
	GLuint compileShaders(const std::string& vertexShaderCode, const std::string& fragmentShaderCode)
	{
		GLint succeeded = GL_FALSE;

		GLuint   vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

		const char* vertexShadersCode[] = { vertexShaderCode.c_str() };
		const char* fragmentShadersCode[] = { fragmentShaderCode.c_str() };
		const GLint    vertexShadersSize[] = { (GLint)vertexShaderCode.size() };
		const GLint  fragmentShadersSize[] = { (GLint)fragmentShaderCode.size() };

		glShaderSource(vertexShaderId, 1, vertexShadersCode, vertexShadersSize);
		glShaderSource(fragmentShaderId, 1, fragmentShadersCode, fragmentShadersSize);

		glCompileShader(vertexShaderId);
		glCompileShader(fragmentShaderId);

		glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &succeeded);
		if (!succeeded)
			showCompilationError(vertexShaderId);

		glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &succeeded);
		if (!succeeded)
			showCompilationError(fragmentShaderId);

		GLuint programID = glCreateProgram();

		glAttachShader(programID, vertexShaderId);
		glAttachShader(programID, fragmentShaderId);

		glLinkProgram(programID);

		glGetShaderiv(programID, GL_LINK_STATUS, &succeeded);
		if (not succeeded)
			showLinkageError(programID);

		glDeleteShader(vertexShaderId);
		glDeleteShader(fragmentShaderId);

		return programID;
	}

	void showCompilationError(GLuint shaderID)
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

	void showLinkageError(GLuint programID)
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
