
/*
	Public Domain Code

	Author: Xavier Canals
	Author: Ángel Rodríguez
*/

#include "Skybox.hpp"



#include <cassert>
#include <gtc/type_ptr.hpp>
#include <iostream>



namespace finalPractice
{
	const GLfloat Skybox::coordinates[] =
	{
		-5.0f, +5.0f, -5.0f, // 0
		-5.0f, -5.0f, -5.0f, // 1
		+5.0f, -5.0f, -5.0f, // 2
		+5.0f, -5.0f, -5.0f, // 3
		+5.0f, +5.0f, -5.0f, // 4
		-5.0f, +5.0f, -5.0f, // 5
		-5.0f, -5.0f, +5.0f, // 6
		-5.0f, -5.0f, -5.0f, // 7
		-5.0f, +5.0f, -5.0f, // 8
		-5.0f, +5.0f, -5.0f, // 9
		-5.0f, +5.0f, +5.0f, // 10
		-5.0f, -5.0f, +5.0f, // 11
		+5.0f, -5.0f, -5.0f, // 12
		+5.0f, -5.0f, +5.0f, // 13
		+5.0f, +5.0f, +5.0f, // 14
		+5.0f, +5.0f, +5.0f, // 15
		+5.0f, +5.0f, -5.0f, // 16
		+5.0f, -5.0f, -5.0f, // 17
		-5.0f, -5.0f, +5.0f, // 18
		-5.0f, +5.0f, +5.0f, // 19
		+5.0f, +5.0f, +5.0f, // 20
		+5.0f, +5.0f, +5.0f, // 21
		+5.0f, -5.0f, +5.0f, // 22
		-5.0f, -5.0f, +5.0f, // 23
		-5.0f, +5.0f, -5.0f, // 24
		+5.0f, +5.0f, -5.0f, // 25
		+5.0f, +5.0f, +5.0f, // 26
		+5.0f, +5.0f, +5.0f, // 27
		-5.0f, +5.0f, +5.0f, // 28
		-5.0f, +5.0f, -5.0f, // 29
		-5.0f, -5.0f, -5.0f, // 30
		-5.0f, -5.0f, +5.0f, // 31
		+5.0f, -5.0f, -5.0f, // 32
		+5.0f, -5.0f, -5.0f, // 33
		-5.0f, -5.0f, +5.0f, // 34
		+5.0f, -5.0f, +5.0f, // 35
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



	Skybox::Skybox(const std::string & texturePath) :
		shader(vertexShaderCode, fragmentShaderCode)
	{
		// Load the cube map texture
		texture.setID(texture.createTextureCubeMap< Rgba8888 >(texturePath));
		assert(texture.isOk());

		// Get the location of uniform variables in the shader
		modelViewMatrixID  = glGetUniformLocation(shader.getID(), "model_view_matrix");
		projectionMatrixID = glGetUniformLocation(shader.getID(), "projection_matrix");
		
		// Generate buffers and arrays for the skybox
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
		shader.use();

		texture.bind();

		// Get the inverse of the camera's transform matrix and the projection matrix
		glm::mat4   modelViewMatrix  = camera.getTransformMatrixInverse();
		const glm::mat4 & projectionMatrix = camera.getProjectionMatrix();

		modelViewMatrix = glm::rotate   (modelViewMatrix, .6f, glm::vec3(0.f, 1.f, 0.f));
		modelViewMatrix = glm::translate(modelViewMatrix,      glm::vec3(-15.f, 0.f, 20.f));
		modelViewMatrix = glm::scale    (modelViewMatrix,      glm::vec3( 5.f, 5.f,  5.f));

		// Set the uniform variables in the shader
		glUniformMatrix4fv( modelViewMatrixID, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
		glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));

		// Disable depth writing (skybox should not affect depth buffer)
		glDepthMask       (GL_FALSE);

		// Bind the vertex array and render the skybox
		glBindVertexArray (vaoID);
		glDrawArrays      (GL_TRIANGLES, 0, 36);

		// Re-enable depth writing
		glDepthMask       (GL_TRUE);

		glBindVertexArray (0);
	}
}
