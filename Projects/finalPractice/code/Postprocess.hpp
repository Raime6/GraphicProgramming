
/*
	Public Domain Code

	Author: Xavier Canals
*/

#pragma once



#ifndef POSTPROCESS_HEADER
#define POSTPROCESS_HEADER



#include "Shader.hpp"



#include <string>



namespace finalPractice
{
	/// <summary>
	/// Postprocess is responsible for applying post-processing effects to a rendered image.
	/// It utilizes a framebuffer for offscreen rendering, applies a shader to the texture,
	/// and renders the processed texture onto the screen.
	/// </summary>
	class Postprocess
	{
	private:

		Shader shader;											///< Shader used for applying post-processing effects to a texture.

		static const std::string   postprocessVertexShaderCode; ///< Vertex shader code used for post-processing.
		static const std::string postprocessFragmentShaderCode; ///< Fragment shader code used for post-processing (applies a color effect).

		static const GLsizei framebufferWidth  = 256;			///< Width of the framebuffer used for offscreen rendering.
		static const GLsizei framebufferHeight = 256;			///< Height of the framebuffer used for offscreen rendering.

		GLuint          framebufferID;							///< ID for the framebuffer object.
		GLuint           outTextureID;							///< ID for the texture that stores the output of the framebuffer.
		GLuint          depthbufferID;							///< ID for the depth buffer used in the framebuffer.

		GLuint     framebufferQuadVAO;							///< ID for the VAO of the quad used for rendering the post-processed texture.
		GLuint framebufferQuadVBOs[2];							///< VBOs for the quad's vertex positions and texture coordinates.

		int               windowWidth;							///< Width of the window (for rendering the final output).
		int              windowHeight;							///< Height of the window (for rendering the final output).

	public:
		
		/// <summary>
		/// Constructor that initializes the postprocessing effect with the given window dimensions.
		/// </summary>
		/// 
		/// <param name="windowWidth">Width of the window.</param>
		/// <param name="windowHeight">Height of the window.</param>
		Postprocess(int windowWidth, int windowHeight);

		/// <summary>
		/// Destructor that cleans up OpenGL resources associated with post-processing.
		/// </summary>
		~Postprocess();

		/// <summary>
		/// Initializes the framebuffer and associated textures.
		/// </summary>
		void buildFramebuffer();

		/// <summary>
		/// Renders the post-processed image to the screen.
		/// </summary>
		void renderFramebuffer();

	};
}



#endif
