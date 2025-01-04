
// Public Domain Code
// Author: Xavier Canals

#include "Skybox.hpp"



#include <cassert>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <Shader.hpp>



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



	Skybox::Skybox(const std::string & texturePath)
	{
		texture.setID(texture.createTextureCubeMap< Rgba8888 >(texturePath));
		assert(texture.isOk());

		shaderProgramID    = compileShaders(vertexShaderCode, fragmentShaderCode);

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

		glDeleteProgram(shaderProgramID);
	}



	void Skybox::render(const Camera & camera)
	{
		glUseProgram(shaderProgramID);

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		texture.bind();

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
}
