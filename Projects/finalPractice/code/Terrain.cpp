
// Public Domain Code
// Author: Xavier Canals

#include "Terrain.hpp"



#include <cassert>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <half.hpp>
#include <vector>



namespace finalPractice
{
	const std::string Terrain::vertexShaderCode =

		"#version 330\n"
		""
		"uniform mat4 model_view_matrix;"
		"uniform mat4 projection_matrix;"
		""
		"layout (location = 0) in vec2 vertex_xz;"
		"layout (location = 1) in vec2 vertex_uv;"
		""
		"uniform sampler2D sampler;"
		"uniform float     max_height;"
		""
		"out float intensity;"
		""
		"void main()"
		"{"
		"   float sample = texture (sampler, vertex_uv).r;"
		"   intensity    = sample * 0.75 + 0.25;"
		"   float height = sample * max_height;"
		"   vec4  xyzw   = vec4(vertex_xz.x, height, vertex_xz.y, 1.0);"
		"   gl_Position  = projection_matrix * model_view_matrix * xyzw;"
		"}";

	const std::string Terrain::fragmentShaderCode =

		"#version 330\n"
		""
		"in  float intensity;"
		"out vec4  fragment_color;"
		""
		"void main()"
		"{"
		"    fragment_color = vec4(intensity, intensity, intensity, 1.0);"
		"}";



	Terrain::Terrain(float width, float depth, unsigned xSlices, unsigned zSlices, const std::string& texturePath) :
		angle(0.f)
	{
		numVertex = xSlices * zSlices;

		std::vector< half_float::half > coordinates(numVertex * 2);
		std::vector< half_float::half > textureUVs (numVertex * 2);
		
		float x = -width * .5f;
		float z = -depth * .5f;
		float u = 0.f;
		float v = 0.f;

		float xStep = width / float(xSlices);
		float zStep = depth / float(zSlices);
		float uStep =   1.f / float(xSlices);
		float vStep =   1.f / float(zSlices);

		int coordinateIndex = 0;

		for (unsigned i = 0; i < zSlices; ++i, z += zStep, v += vStep)
		{
			for (unsigned j = 0; j < xSlices; ++j, coordinateIndex += 2, x += xStep, u += uStep)
			{
				coordinates[coordinateIndex + 0] = half_float::half(x);
				coordinates[coordinateIndex + 1] = half_float::half(z);
				textureUVs [coordinateIndex + 0] = half_float::half(u);
				textureUVs [coordinateIndex + 1] = half_float::half(v);
			}

			x += xStep = -xStep;
			u += uStep = -uStep;
		}

		glGenBuffers(VBO_COUNT, vboIDs);
		glGenVertexArrays(1, &vaoID);

		glBindVertexArray(vaoID);

		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBO_COORDINATES]);
		glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(half_float::half), coordinates.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_HALF_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBO_TEXTURE_UVS]);
		glBufferData(GL_ARRAY_BUFFER, textureUVs.size() * sizeof(half_float::half), textureUVs.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_HALF_FLOAT, GL_FALSE, 0, 0);

		glBindVertexArray(0);



		shaderProgramID = compileShaders(vertexShaderCode, fragmentShaderCode);

		modelViewMatrixID  = glGetUniformLocation(shaderProgramID, "model_view_matrix");
		projectionMatrixID = glGetUniformLocation(shaderProgramID, "projection_matrix");

		glUniform1f(glGetUniformLocation(shaderProgramID, "max_height"), 5.f);



		texture.setID(texture.createTexture2D< Monochrome8 >(texturePath));
		assert(texture.isOk());
	}

	Terrain::~Terrain()
	{
		glDeleteVertexArrays(1, &vaoID);
		glDeleteBuffers(VBO_COUNT, vboIDs);

		glDeleteProgram(shaderProgramID);
	}



	void Terrain::update()
	{
		angle += 0.005f;
	}



	void Terrain::render(const Camera & camera)
	{
		//glUseProgram(shaderProgramID);

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 modelViewMatrix(1);

		modelViewMatrix = glm::translate(modelViewMatrix,        glm::vec3(0.f, 0.f, -11.f));
		modelViewMatrix = glm::rotate   (modelViewMatrix,   .4f, glm::vec3(1.f, 0.f,   0.f));
		modelViewMatrix = glm::rotate   (modelViewMatrix, angle, glm::vec3(0.f, 1.f,   0.f));

		glUniformMatrix4fv(modelViewMatrixID, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));

		texture.bind();

		glBindVertexArray(vaoID);
		glDrawArrays(GL_LINE_STRIP, 0, numVertex);
		/*glBindVertexArray(0);

		glUseProgram(0);*/
	}

	void Terrain::resize(int width, int height)
	{
		glm::mat4 projectionMatrix = glm::perspective(20.f, GLfloat(width) / height, 1.f, 500.f);

		glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	}
}
