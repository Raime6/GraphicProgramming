
/*
	Public Domain Code

	Author: Xavier Canals
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
		"uniform mat3 normal_matrix;"
		""
		"layout (location = 0) in vec2 vertex_xz;"
		"layout (location = 1) in vec2 vertex_uv;"
		""
		"uniform sampler2D height_map;"
		"uniform float     max_height;"
		"uniform vec2      height_uv_step;"
		"uniform vec2      terrain_cell_size;"
		""
		"out vec2  texture_uv;"
		"out float height_ratio;"
		"out vec3  view_position;"
		"out vec3  view_normal;"
		""
		"void main()"
		"{"
		"   float sample = texture(height_map, vertex_uv).r;"
		"   float height = sample * max_height;"
		"   vec4  xyzw   = vec4(vertex_xz.x, height, vertex_xz.y, 1.0);"
		""
		"   float height_left  = texture(height_map, vertex_uv - vec2(height_uv_step.x, 0.0)).r * max_height;"
		"   float height_right = texture(height_map, vertex_uv + vec2(height_uv_step.x, 0.0)).r * max_height;"
		"   float height_down  = texture(height_map, vertex_uv - vec2(0.0, height_uv_step.y)).r * max_height;"
		"   float height_up    = texture(height_map, vertex_uv + vec2(0.0, height_uv_step.y)).r * max_height;"
		"   vec3 tangent_x = vec3(2.0 * terrain_cell_size.x, height_right - height_left, 0.0);"
		"   vec3 tangent_z = vec3(0.0, height_up - height_down, 2.0 * terrain_cell_size.y);"
		"   vec3 object_normal = normalize(cross(tangent_z, tangent_x));"
		""
		"   vec4 position = model_view_matrix * xyzw;"
		"   gl_Position  = projection_matrix * position;"
		"   texture_uv   = vertex_uv;"
		"   height_ratio = sample;"
		"   view_position = position.xyz;"
		"   view_normal = normalize(normal_matrix * object_normal);"
		"}";

	const std::string Terrain::fragmentShaderCode =

		"#version 330\n"
		""
		"struct Light"
		"{"
		"    vec4 position;"
		"    vec3 color;"
		"};"
		""
		"uniform Light light;"
		"uniform float ambient_intensity;"
		"uniform float diffuse_intensity;"
		"uniform float specular_intensity;"
		"uniform float attenuation_constant;"
		"uniform float attenuation_linear;"
		"uniform float attenuation_quadratic;"
		"uniform float material_ambient;"
		"uniform float material_diffuse;"
		"uniform float material_specular;"
		"uniform float material_shininess;"
		""
		"uniform sampler2D grass_texture;"
		"uniform sampler2D rock_texture;"
		""
		"in  vec2  texture_uv;"
		"in  float height_ratio;"
		"in  vec3  view_position;"
		"in  vec3  view_normal;"
		"out vec4 fragment_color;"
		""
		"void main()"
		"{"
		"    vec2 tiled_uv = texture_uv * 12.0;"
		"    vec3 grass = texture(grass_texture, tiled_uv).rgb;"
		"    vec3 rock  = texture(rock_texture,  tiled_uv).rgb;"
		"    float rock_amount = smoothstep(0.42, 0.68, height_ratio);"
		"    vec3 albedo = mix(grass, rock, rock_amount);"
		""
		"    vec3 normal = normalize(view_normal);"
		"    vec3 to_light = light.position.xyz - view_position;"
		"    float light_distance = length(to_light);"
		"    vec3 light_direction = normalize(to_light);"
		"    vec3 view_direction = normalize(-view_position);"
		"    vec3 halfway_direction = normalize(light_direction + view_direction);"
		""
		"    float diffuse_factor = max(dot(normal, light_direction), 0.0);"
		"    float specular_factor = 0.0;"
		"    if (diffuse_factor > 0.0)"
		"        specular_factor = pow(max(dot(normal, halfway_direction), 0.0), material_shininess);"
		""
		"    float attenuation = 1.0 / (attenuation_constant"
		"        + attenuation_linear * light_distance"
		"        + attenuation_quadratic * light_distance * light_distance);"
		""
		"    vec3 ambient = ambient_intensity * material_ambient * albedo;"
		"    vec3 diffuse = diffuse_intensity * material_diffuse * diffuse_factor * light.color * albedo;"
		"    vec3 specular = specular_intensity * material_specular * specular_factor * light.color;"
		"    fragment_color = vec4(ambient + attenuation * (diffuse + specular), 1.0);"
		"}";



	Terrain::Terrain
	(
		float width,
		float depth,
		unsigned xSlices,
		unsigned zSlices,
		const std::string& heightMapPath,
		const std::string& grassTexturePath,
		const std::string& rockTexturePath
	) :
		shader(vertexShaderCode, fragmentShaderCode)
	{
		shader.use();

		assert(xSlices > 0 && zSlices > 0);

		const unsigned verticesPerRow = xSlices + 1;
		const unsigned verticesPerColumn = zSlices + 1;
		numVertex = static_cast<GLsizei>(verticesPerRow * verticesPerColumn);

		coordinates.resize(numVertex * 2);
		textureUVs .resize(numVertex * 2);
		index      .resize(xSlices * zSlices * 6);

		float xStep = width / float(xSlices);
		float zStep = depth / float(zSlices);

		// Fill the vertex coordinates and texture UVs
		for (unsigned i = 0; i <= zSlices; ++i)
		{
			const float z = -depth * .5f + float(i) * zStep;
			const float v = float(i) / float(zSlices);

			for (unsigned j = 0; j <= xSlices; ++j)
			{
				const float x = -width * .5f + float(j) * xStep;
				const float u = float(j) / float(xSlices);
				const size_t coordinateIndex = size_t(i * verticesPerRow + j) * 2;

				coordinates[coordinateIndex + 0] = half_float::half(x);
				coordinates[coordinateIndex + 1] = half_float::half(z);
				textureUVs [coordinateIndex + 0] = half_float::half(u);
				textureUVs [coordinateIndex + 1] = half_float::half(v);
			}
		}

		int indexIndex = 0;

		// Fill the triangles index
		for (unsigned i = 0; i < zSlices; ++i)
		{
			for (unsigned j = 0; j < xSlices; ++j)
			{
				GLuint bottomLeft  =  i      * verticesPerRow +  j;
				GLuint bottomRight =  i      * verticesPerRow + (j + 1);
				GLuint topLeft     = (i + 1) * verticesPerRow +  j;
				GLuint topRight    = (i + 1) * verticesPerRow + (j + 1);

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
		normalMatrixID = glGetUniformLocation(shader.getID(), "normal_matrix");
		heightMapSamplerID = glGetUniformLocation(shader.getID(), "height_map");
		grassTextureSamplerID = glGetUniformLocation(shader.getID(), "grass_texture");
		rockTextureSamplerID = glGetUniformLocation(shader.getID(), "rock_texture");

		// Set max height uniform
		glUniform1f(glGetUniformLocation(shader.getID(), "max_height"), 5.f);
		glUniform2f(glGetUniformLocation(shader.getID(), "height_uv_step"), 1.f / float(xSlices), 1.f / float(zSlices));
		glUniform2f(glGetUniformLocation(shader.getID(), "terrain_cell_size"), width / float(xSlices), depth / float(zSlices));
		glUniform1i(heightMapSamplerID, 0);
		glUniform1i(grassTextureSamplerID, 1);
		glUniform1i(rockTextureSamplerID, 2);
		glUniform1f(glGetUniformLocation(shader.getID(), "material_ambient"), 1.f);
		glUniform1f(glGetUniformLocation(shader.getID(), "material_diffuse"), .9f);
		glUniform1f(glGetUniformLocation(shader.getID(), "material_specular"), .12f);
		glUniform1f(glGetUniformLocation(shader.getID(), "material_shininess"), 12.f);

		heightMap.setID(heightMap.createTexture2D< Monochrome8 >(heightMapPath, Texture::TypeTexture2D::HEIGHTMAP));
		grassTexture.setID(grassTexture.createTexture2D< Rgba8888 >(grassTexturePath, Texture::TypeTexture2D::ALBEDO));
		rockTexture.setID(rockTexture.createTexture2D< Rgba8888 >(rockTexturePath, Texture::TypeTexture2D::ALBEDO));
		assert(heightMap.isOk() && grassTexture.isOk() && rockTexture.isOk());

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

		const glm::mat4 viewMatrix = camera.getTransformMatrixInverse();
		modelViewMatrix = viewMatrix * modelViewMatrix;
		const glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelViewMatrix)));

		glUniformMatrix4fv(modelViewMatrixID,  1, GL_FALSE, glm::value_ptr(modelViewMatrix));
		glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, glm::value_ptr(camera.getProjectionMatrix()));
		glUniformMatrix3fv(normalMatrixID, 1, GL_FALSE, glm::value_ptr(normalMatrix));

		lighting.configureLight(shader.getID(), viewMatrix);

		glActiveTexture(GL_TEXTURE0);
		heightMap.bind();

		glActiveTexture(GL_TEXTURE1);
		grassTexture.bind();

		glActiveTexture(GL_TEXTURE2);
		rockTexture.bind();

		glBindVertexArray(vaoID);
		//glDrawArrays(GL_LINE_STRIP, 0, numVertex);
		glDrawElements(GL_TRIANGLES, static_cast< GLsizei >(index.size()), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Leave the conventional texture unit active for the remaining meshes.
		glActiveTexture(GL_TEXTURE0);
	}

	void Terrain::resize(int width, int height)
	{
		glm::mat4 projectionMatrix = glm::perspective(20.f, GLfloat(width) / height, 1.f, 500.f);

		glUniformMatrix4fv(projectionMatrixID, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	}
}
