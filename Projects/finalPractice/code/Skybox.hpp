
/*
	Public Domain Code

	Author: Xavier Canals
	Author: Ángel Rodríguez
*/

#pragma once



#ifndef SKYBOX_HEADER
#define SKYBOX_HEADER



#include "Camera.hpp"
#include "Shader.hpp"
#include "Texture.hpp"



namespace finalPractice
{
	/// <summary>
	/// The Skybox class represents a 3D environment that can be rendered as a background.
	/// It utilizes a cube map texture and shaders to render the skybox.
	/// </summary>
	class Skybox
	{
		private:

			static const GLfloat          coordinates[];			///< Coordinates for the vertices of the skybox cube.
			static const std::string   vertexShaderCode;			///< Source code for the vertex shader.
			static const std::string fragmentShaderCode;			///< Source code for the fragment shader.

			Shader				shader;								///< Shader used for rendering the skybox.
			Texture			   texture;								///< Texture of the skybox, typically a cube map.

		private:

			GLuint				 vboID;								///< Vertex Buffer Object ID.
			GLuint				 vaoID;								///< Vertex Array Object ID.

			GLint    modelViewMatrixID;								///< Location of the model-view matrix in the shader.
			GLint   projectionMatrixID;								///< Location of the projection matrix in the shader.

		public:

			/// <summary>
			/// Constructor that initializes the skybox by loading a cube map texture.
			/// </summary>
			/// 
			/// <param name="texturePath">The path to the texture file for the skybox cube map.</param>
			Skybox(const std::string & texturePath);
			
			/// <summary>
			/// Destructor that cleans up OpenGL resources used by the skybox.
			/// </summary>
		   ~Skybox();

		public:

			/// <summary>
			/// Renders the skybox using the provided camera.
			/// </summary>
			/// 
			/// <param name="camera">The camera that provides the view and projection matrices for rendering.</param>
			void render(const Camera & camera);
	};
}



#endif
