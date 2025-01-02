
// Public Domain Code
// Author: Xavier Canals

#include "Skybox.hpp"



#include <cassert>
#include <gtc/type_ptr.hpp>
#include <iostream>



namespace finalPractice
{
	const GLfloat Skybox::coordinates[] =
	{
		-1.0f, +1.0f, -1.0f, // 0
		-1.0f, -1.0f, -1.0f, // 1
		+1.0f, -1.0f, -1.0f, // 2
		+1.0f, -1.0f, -1.0f, // 3
		+1.0f, +1.0f, -1.0f, // 4
		-1.0f, +1.0f, -1.0f, // 5
		-1.0f, -1.0f, +1.0f, // 6
		-1.0f, -1.0f, -1.0f, // 7
		-1.0f, +1.0f, -1.0f, // 8
		-1.0f, +1.0f, -1.0f, // 9
		-1.0f, +1.0f, +1.0f, // 10
		-1.0f, -1.0f, +1.0f, // 11
		+1.0f, -1.0f, -1.0f, // 12
		+1.0f, -1.0f, +1.0f, // 13
		+1.0f, +1.0f, +1.0f, // 14
		+1.0f, +1.0f, +1.0f, // 15
		+1.0f, +1.0f, -1.0f, // 16
		+1.0f, -1.0f, -1.0f, // 17
		-1.0f, -1.0f, +1.0f, // 18
		-1.0f, +1.0f, +1.0f, // 19
		+1.0f, +1.0f, +1.0f, // 20
		+1.0f, +1.0f, +1.0f, // 21
		+1.0f, -1.0f, +1.0f, // 22
		-1.0f, -1.0f, +1.0f, // 23
		-1.0f, +1.0f, -1.0f, // 24
		+1.0f, +1.0f, -1.0f, // 25
		+1.0f, +1.0f, +1.0f, // 26
		+1.0f, +1.0f, +1.0f, // 27
		-1.0f, +1.0f, +1.0f, // 28
		-1.0f, +1.0f, -1.0f, // 29
		-1.0f, -1.0f, -1.0f, // 30
		-1.0f, -1.0f, +1.0f, // 31
		+1.0f, -1.0f, -1.0f, // 32
		+1.0f, -1.0f, -1.0f, // 33
		-1.0f, -1.0f, +1.0f, // 34
		+1.0f, -1.0f, +1.0f, // 35
	};

	const std::string Skybox::vertexShaderCode =

		"#version 330\n"
		""
		"uniform mat4 model_view_matrix;"
		"uniform mat4 projection_matrix;"
		""
		"layout (location = 0) in vec3 vertex_coordinates;"
		""
		"out vec3 texture_coordinates;"
		""
		"void main()"
		"{"
		"   texture_coordinates = vec3(vertex_coordinates.x, -vertex_coordinates.y, vertex_coordinates.z);"
		"   gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);"
		"}";

	const std::string Skybox::fragmentShaderCode =

		"#version 330\n"
		""
		"in  vec3 texture_coordinates;"
		"out vec4      fragment_color;"
		""
		"uniform samplerCube sampler;"
		""
		"void main()"
		"{"
		"    fragment_color = texture(sampler, texture_coordinates);"
		"}";



	Skybox::Skybox(const std::string & textureBasePath) : skyboxTexture(textureBasePath)
	{
		assert(skyboxTexture.isOk());

		shaderProgramID    = compileShaders();

		modelViewMatrixID  = glGetUniformLocation(shaderProgramID, "model_view_matrix");
		projectionMatrixID = glGetUniformLocation(shaderProgramID, "projection_matrix");
		
		glGenBuffers     (1, &vboID);
		glGenVertexArrays(1, &vaoID);

		glBindVertexArray(vaoID);

		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(coordinates), coordinates, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);
	}

	Skybox::~Skybox()
	{
		glDeleteVertexArrays(1, &vaoID);
		glDeleteBuffers		(1, &vboID);
	}



	void Skybox::render(const Camera & camera)
	{
		glUseProgram(shaderProgramID);

		skyboxTexture.bind();

		const glm::mat4   modelViewMatrix  = camera.getTransformMatrixInverse();
		const glm::mat4 & projectionMatrix = camera.getProjectionMatrix();

		glUniformMatrix4fv( modelViewMatrixID, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
		glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		glDepthMask       (GL_FALSE);

		glBindVertexArray (vaoID);
		glDrawArrays      (GL_TRIANGLES, 0, 36);

		glDepthMask       (GL_TRUE);

		glBindVertexArray (0);
		glUseProgram      (0);
	}



	GLuint Skybox::compileShaders()
	{
		GLint succeeded = GL_FALSE;

		GLuint   vertexShaderId = glCreateShader(  GL_VERTEX_SHADER);
		GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

		const char *   vertexShadersCode[] = {          vertexShaderCode.c_str() };
		const char * fragmentShadersCode[] = {		  fragmentShaderCode.c_str() };
		const GLint    vertexShadersSize[] = { (GLint)  vertexShaderCode.size () };
		const GLint  fragmentShadersSize[] = { (GLint)fragmentShaderCode.size () };

		glShaderSource(  vertexShaderId, 1,   vertexShadersCode,   vertexShadersSize);
		glShaderSource(fragmentShaderId, 1, fragmentShadersCode, fragmentShadersSize);

		glCompileShader(  vertexShaderId);
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

		glDeleteShader(  vertexShaderId);
		glDeleteShader(fragmentShaderId);

		return programID;
	}

	void Skybox::showCompilationError(GLuint shaderID)
	{
		std::string       infoLog;
		GLint       infoLogLenght;

		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLenght);

		infoLog.resize(infoLogLenght);

		glGetShaderInfoLog(shaderID, infoLogLenght, NULL, &infoLog.front());

		std::cerr << infoLog.c_str() << std::endl;
		assert(false);
	}

	void Skybox::showLinkageError(GLuint programID)
	{
		std::string       infoLog;
		GLint       infoLogLenght;

		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLenght);

		infoLog.resize(infoLogLenght);

		glGetProgramInfoLog(programID, infoLogLenght, NULL, &infoLog.front());

		std::cerr << infoLog.c_str() << std::endl;
		assert(false);
	}
}
