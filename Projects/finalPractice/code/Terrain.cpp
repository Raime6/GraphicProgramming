
/*
	Public Domain Code

	Author: Xavier Canals
	Author: Ángel Rodríguez
*/

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
		shader(vertexShaderCode, fragmentShaderCode)
	{
		shader.use();

		numVertex = xSlices * zSlices;

		coordinates.resize(numVertex * 2);
		textureUVs .resize(numVertex * 2);
		index      .resize(xSlices * zSlices * 6);
		
		float x = -width * .5f;
		float z = -depth * .5f;
		float u = 0.f;
		float v = 0.f;

		float xStep = width / float(xSlices);
		float zStep = depth / float(zSlices);
		float uStep =   1.f / float(xSlices);
		float vStep =   1.f / float(zSlices);

		int coordinateIndex = 0;

		// Fill the vertex coordinates and texture UVs
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

		int indexIndex = 0;

		// Fill the triangles index
		for (unsigned i = 0; i < zSlices; ++i)
		{
			for (unsigned j = 0; j < xSlices; ++j)
			{
				GLuint bottomLeft  =  i      * (xSlices + 1) +  j;
				GLuint bottomRight =  i      * (xSlices + 1) + (j + 1);
				GLuint topLeft     = (i + 1) * (xSlices + 1) +  j;
				GLuint topRight    = (i + 1) * (xSlices + 1) + (j + 1);

				// Primer triángulo
				index[indexIndex++] = bottomLeft;
				index[indexIndex++] = topLeft;
				index[indexIndex++] = bottomRight;

				// Segundo triángulo
				index[indexIndex++] = bottomRight;
				index[indexIndex++] = topLeft;
				index[indexIndex++] = topRight;
			}
		}

		glGenBuffers(VBO_COUNT, vboIDs);
		glGenVertexArrays(1, &vaoID);

		glBindVertexArray(vaoID);

		// TERRAIN COORDINATES
		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBO_COORDINATES]);
		glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(half_float::half), coordinates.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_HALF_FLOAT, GL_FALSE, 0, 0);

		// TERRAIN TEXTURE UVS
		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBO_TEXTURE_UVS]);
		glBufferData(GL_ARRAY_BUFFER, textureUVs.size() * sizeof(half_float::half), textureUVs.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_HALF_FLOAT, GL_FALSE, 0, 0);

		// TERRAIN TRIANGLES INDEX
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIDs[EBO_INDEX]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint), index.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);

		// Get the location of shader uniforms
		modelViewMatrixID  = glGetUniformLocation(shader.getID(), "model_view_matrix");
		projectionMatrixID = glGetUniformLocation(shader.getID(), "projection_matrix");

		// Set max height uniform
		glUniform1f(glGetUniformLocation(shader.getID(), "max_height"), 5.f);



		texture.setID(texture.createTexture2D< Monochrome8 >(texturePath, Texture::TypeTexture2D::HEIGHTMAP));
		assert(texture.isOk());

		// Resize the terrain based on the default window size
		resize(1024, 576);
	}

	Terrain::~Terrain()
	{
		glDeleteVertexArrays(1, &vaoID);
		glDeleteBuffers(VBO_COUNT, vboIDs);
	}



	void Terrain::render(const Camera & camera)
	{
		shader.use();

		glm::mat4 modelViewMatrix(1);

		modelViewMatrix = glm::rotate   (modelViewMatrix, .6f, glm::vec3(0.f, 1.f, 0.f));
		modelViewMatrix = glm::translate(modelViewMatrix,      glm::vec3(-15.f, -3.6f  , 20.f));
		modelViewMatrix = glm::scale    (modelViewMatrix,      glm::vec3( 2.5f,  2.5f ,  2.5f));

		modelViewMatrix = camera.getTransformMatrixInverse() * modelViewMatrix;

		glUniformMatrix4fv(modelViewMatrixID,  1, GL_FALSE, glm::value_ptr(modelViewMatrix));
		glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));

		texture.bind();

		glBindVertexArray(vaoID);
		//glDrawArrays(GL_LINE_STRIP, 0, numVertex);
		glDrawElements(GL_TRIANGLES, static_cast< GLsizei >(index.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

	void Terrain::resize(int width, int height)
	{
		glm::mat4 projectionMatrix = glm::perspective(20.f, GLfloat(width) / height, 1.f, 500.f);

		glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	}
}
