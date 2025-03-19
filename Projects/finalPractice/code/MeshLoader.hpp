
/*
	Public Domain Code

	Author: Xavier Canals
*/

#pragma once



#ifndef MESHLOADER_HEADER
#define MESHLOADER_HEADER



#include "Camera.hpp"
#include "Lighting.hpp"
#include "Shader.hpp"
#include "Texture.hpp"



#include <glad/glad.h>
#include <glm.hpp>
#include <string>



namespace finalPractice
{
	/// <summary>
	/// MeshLoader is responsible for loading and rendering 3D meshes, applying materials and textures, 
	/// and handling animations and transformations.
	/// </summary>
	class MeshLoader
	{
		private:

			/// <summary>
			/// Enum representing the different VBO types.
			/// </summary>
			enum
			{
				VBO_COORDINATES,								///< Vertex coordinates VBO
				VBO_COLORS,										///< Vertex colors VBO
				VBO_NORMALS,									///< Vertex normals VBO
				EBO_INDEX,										///< Element Index Buffer Object
				VBO_COUNT										///< Total number of VBOs
			};

		private:

			static const std::string          vertexShaderCode; ///< Vertex shader code for non-textured rendering.
			static const std::string        fragmentShaderCode; ///< Fragment shader code for non-textured rendering.
			static const std::string   vertexShaderCodeTexture; ///< Vertex shader code for textured rendering.
			static const std::string fragmentShaderCodeTexture; ///< Fragment shader code for textured rendering.

			Shader			   shader;							///< Shader used for rendering the mesh.
			Lighting		 lighting;							///< Lighting setup for the scene.
			Texture           texture;							///< Texture used for the mesh (if any).
			//Texture     textureNormal;

		private:

			GLuint  vboIDs[VBO_COUNT];							///< IDs for the vertex buffer objects.
			GLuint				vaoID;							///< ID for the vertex array object.

			GLsizei			 numIndex;							///< Number of indices for rendering.

			GLint   modelViewMatrixID;							///< ID for the model-view matrix uniform.
			GLint  projectionMatrixID;							///< ID for the projection matrix uniform.
			GLint      normalMatrixID;							///< ID for the normal matrix uniform.

			bool		  needTexture;							///< Flag indicating whether the mesh requires a texture.
			bool			 moveDown;							///< Flag for animating movement downwards.
			float		 transparency;							///< Current transparency of the mesh (0f - 1f).
			float				angle;							///< Current rotation angle of the mesh.
			float				 posY;							///< Current vertical position of the mesh.

		public:

			/// <summary>
			/// Constructor that loads the mesh from a file path and sets up the shader without texture.
			/// </summary>
			/// 
			/// <param name="meshFilePath">The file path to the mesh to be loaded.</param>
			MeshLoader(const std::string& meshFilePath, float _transparency);

			/// <summary>
			/// Constructor that loads the mesh and applies a texture from a file path.
			/// </summary>
			/// 
			/// <param name="meshFilePath">The file path to the mesh to be loaded.</param>
			/// <param name="textureAlbedoPath">The file path to the texture (albedo).</param>
			MeshLoader(const std::string& meshFilePath, const std::string& textureAlbedoPath, float _transparency);

			/// <summary>
			/// Destructor that cleans up OpenGL resources.
			/// </summary>
			~MeshLoader();

		public:

			/// <summary>
			/// Updates the mesh, including any animations.
			/// </summary>
			void  update();

			/// <summary>
			/// Renders the mesh with the specified transformations and camera.
			/// </summary>
			/// 
			/// <param name="camera">The camera used to calculate the view matrix.</param>
			/// <param name="translateVector">The translation vector for the mesh.</param>
			/// <param name="angle">The rotation angle for the mesh.</param>
			/// <param name="rotateVector">The axis of rotation for the mesh.</param>
			/// <param name="scaleVector">The scaling vector for the mesh.</param>
			void  render(const Camera& camera, glm::vec3 tanslateVector, float angle, glm::vec3 rotateVector, glm::vec3 scaleVector);

			/// <summary>
			/// Resizes the viewport and updates the projection matrix.
			/// </summary>
			/// 
			/// <param name="width">New width of the viewport.</param>
			/// <param name="height">New height of the viewport.</param>
			void  resize(int width, int height);



			/// <summary>
			/// Gets the current angle of the mesh.
			/// </summary>
			/// 
			/// <returns>The current angle of the mesh.</returns>
			float getAngle();

			/// <summary>
			/// Gets the current vertical position (Y) of the mesh.
			/// </summary>
			/// 
			/// <returns>The current vertical position of the mesh.</returns>
			float getPosY();

		private:

			/// <summary>
			/// Loads the mesh from a file and sets up the vertex buffers.
			/// </summary>
			/// 
			/// <param name="meshFilePath">The file path to the mesh to be loaded.</param>
			void loadMesh(const std::string& meshFilePath);

			/// <summary>
			/// Sets a color for the mesh (Used on non-textured meshes).
			/// </summary>
			/// 
			/// <param name="shaderID">The ID of the shader program.</param>
			void configureMaterial(GLuint shaderID);



			/// <summary>
			/// Handles the crystal animation logic (used for crustal mesh animation only).
			/// </summary>
			void crystalAnimation();
	};
}



#endif

