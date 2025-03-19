
/*
	Public Domain Code

	Author: Xavier Canals
*/

#pragma once



#ifndef TERRAIN_HEADER
#define TERRAIN_HEADER



#include "Camera.hpp"
#include "Color.hpp"
#include "ColorBuffer.hpp"
#include "Shader.hpp"
#include "Texture.hpp"



#include <half.hpp>



namespace finalPractice
{
	/// <summary>
	/// The Terrain class represents a 3D terrain mesh, typically used for landscapes.
	/// It generates the terrain grid and renders it using shaders and textures.
	/// </summary>
	class Terrain
	{
	private:

		/// <summary>
		/// Enum representing the different VBO types.
		/// </summary>
		enum
		{
			VBO_COORDINATES,								///< Vertex coordinates VBO
			VBO_TEXTURE_UVS,								///< Texture coordinates (UVs)VBO.
			EBO_INDEX,										///< Triangle index EBO
			VBO_COUNT										///< Total number of VBOs
		};

	private:

		static const std::string   vertexShaderCode;		///< Vertex shader code for terrain rendering.
		static const std::string fragmentShaderCode;		///< Fragment shader code for terrain rendering.
		static const std::string        texturePath;		///< Path to the terrain texture.

		std::vector<half_float::half> coordinates;			///< Coordinates of the terrain vertex.
		std::vector<half_float::half> textureUVs;			///< UV texture coordinates.
		std::vector<GLuint> index;							///< Index for the terrain triangles.

		Shader             shader;							///< Shader used to render the terrain.
		Texture           texture;							///< Texture for the terrain.

	private:

		GLuint  vboIDs[VBO_COUNT];							///< Vertex Buffer Object IDs.
		GLuint              vaoID;							///< Vertex Array Object ID.

		GLsizei         numVertex;							///< Number of vertices in the terrain.

	private:

		GLint	modelViewMatrixID;							///< Location of the model-view matrix in the shader.
		GLint  projectionMatrixID;							///< Location of the projection matrix in the shader.

	public:

		/// <summary>
		/// Constructor for creating a terrain mesh with the specified dimensions.
		/// </summary>
		/// 
		/// <param name="width">Width of the terrain.</param>
		/// <param name="depth">Depth of the terrain.</param>
		/// <param name="xSlices">Number of slices in the X direction.</param>
		/// <param name="zSlices">Number of slices in the Z direction.</param>
		/// <param name="texturePath">Path to the texture file used for the terrain.</param>
		Terrain(float width, float depth, unsigned xSlices, unsigned zSlices, const std::string& texturePath);

		/// <summary>
		/// Destructor to clean up OpenGL resources used by the terrain.
		/// </summary>
		~Terrain();



		/// <summary>
		/// Renders the terrain using the provided camera for transformations.
		/// </summary>
		/// 
		/// <param name="camera">The camera used for the model-view and projection matrices.</param>
		void render(const Camera& camera);

		/// <summary>
		/// Resizes the terrain projection matrix when the window size changes.
		/// </summary>
		/// 
		/// <param name="width">New window width.</param>
		/// <param name="height">New window height.</param>
		void resize(int width, int height);
	};
}



#endif
