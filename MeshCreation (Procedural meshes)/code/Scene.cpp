
// Public Domain Code
// Author: Xavier Canals

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>



#include "Scene.hpp"



namespace meshCreation
{
	const string Scene::vertexShaderCode =
		
		"#version 330\n"
		""
		"uniform mat4 model_view_matrix;"
		"uniform mat4 projection_matrix;"
		""
		"layout (location = 0) in vec3 vertex_coordinates;"
		"layout (location = 1) in vec3 vertex_color;"
		""
		"out vec3 front_color;"
		""
		"void main()"
		"{"
		"   gl_Position = projection_matrix * model_view_matrix * vec4(vertex_coordinates, 1.0);"
		"   front_color = vertex_color;"
		"}";

	const string Scene::fragmentShaderCode =

		"#version 330\n"
		""
		"in  vec3    front_color;"
		"out vec4 fragment_color;"
		""
		"void main()"
		"{"
		"    fragment_color = vec4(front_color, 1.0);"
		"}";



	Scene::Scene(unsigned width, unsigned height) : angle(0)
	{
		glEnable          (GL_CULL_FACE);
		glEnable         (GL_DEPTH_TEST);
		glClearColor(.2f, .2f, .2f, .1f);

		GLuint programID = compileShaders();

		glUseProgram(programID);

		modelViewMatrixID  = glGetUniformLocation(programID, "model_view_matrix");
		projectionMatrixID = glGetUniformLocation(programID, "projection_matrix");

		resize(width, height);
	}



	void Scene::update()
	{
		angle += 0.005f;
	}

	void Scene::render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 modelViewMatrix(1);

		modelViewMatrix = glm::translate(modelViewMatrix, glm::vec3(0.f, -1.f, -6.f));
		modelViewMatrix = glm::rotate   (modelViewMatrix, angle, glm::vec3(0.f, 1.f, 0.f));

		glUniformMatrix4fv(modelViewMatrixID, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));

		plane   .render();
		cylinder.render();
		cone    .render();
	}

	void Scene::resize(unsigned width, unsigned height)
	{
		glm::mat4 projectionMatrix = glm::perspective(20.f, GLfloat(width) / height, 1.f, 5000.f);

		glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		glViewport(0, 0, width, height);
	}



	GLuint Scene::compileShaders()
	{
		GLint succeeded = GL_FALSE;

		GLuint   vertexShaderId = glCreateShader(  GL_VERTEX_SHADER);
		GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

		const char *   vertexShadersCode[] = {         vertexShaderCode.c_str() };
		const char * fragmentShadersCode[] = {       fragmentShaderCode.c_str() };
		const GLint    vertexShadersSize[] = { (GLint)  vertexShaderCode.size() };
		const GLint  fragmentShadersSize[] = { (GLint)fragmentShaderCode.size() };

		glShaderSource(  vertexShaderId, 1,   vertexShadersCode, vertexShadersSize);
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

		glAttachShader(programID,   vertexShaderId);
		glAttachShader(programID, fragmentShaderId);

		glLinkProgram(programID);

		glGetShaderiv(programID, GL_LINK_STATUS, &succeeded);
		if (not succeeded)
			showLinkageError(programID);

		glDeleteShader(  vertexShaderId);
		glDeleteShader(fragmentShaderId);

		return programID;
	}

	void Scene::showCompilationError(GLuint shaderID)
	{
		string      infoLog;
		GLint infoLogLenght;

		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLenght);

		infoLog.resize(infoLogLenght);

		glGetShaderInfoLog(shaderID, infoLogLenght, NULL, &infoLog.front());

		cerr << infoLog.c_str() << endl;
		assert(false);
	}

	void Scene::showLinkageError(GLuint programID)
	{
		string infoLog;
		GLint  infoLogLenght;

		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLenght);

		infoLog.resize(infoLogLenght);

		glGetProgramInfoLog(programID, infoLogLenght, NULL, &infoLog.front());

		cerr << infoLog.c_str() << endl;
		assert(false);
	}
}
