
// Public Domain Code
// Author: Xavier Canals

#include "Scene.hpp"
#include "Shader.hpp"



#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>



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



	Scene::Scene(unsigned width, unsigned height) :
		shader  (vertexShaderCode, fragmentShaderCode),
		cone    (2, 1, 12),
		cylinder(2, 1, 12),
		plane   (8, 6, 4, 3),
		angle   (0)
	{
		glEnable          (GL_CULL_FACE);
		glEnable         (GL_DEPTH_TEST);
		glClearColor(.2f, .2f, .2f, .1f);

		shader.Use();

		modelViewMatrixID  = glGetUniformLocation(shader.getID(), "model_view_matrix");
		projectionMatrixID = glGetUniformLocation(shader.getID(), "projection_matrix");

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
}
