
/*
	Public Domain Code

	Author: Xavier Canals
	Author: Ángel Rodríguez
*/

#pragma once



#ifndef TEXTURE_HEADER
#define TEXTURE_HEADER



#include "Color.hpp"
#include "ColorBuffer.hpp"



#include <glad/glad.h>
#include <memory>
#include <SOIL2.h>
#include <string>



namespace finalPractice
{
	/// <summary>
	/// The Texture class represents a 2D or cubemap texture that can be used in OpenGL applications.
	/// It provides methods for loading textures from files and binding them to OpenGL.
	/// </summary>
	class Texture
	{
	private:

		/// <summary>
		/// Enumeration for texture types.
		/// </summary>
		enum TypeTexture { NO_TYPE, TEXTURE2D, TEXTURECUBEMAP };

	public:

		/// <summary>
		/// Enumeration for different 2D texture types.
		/// </summary>
		enum TypeTexture2D { ALBEDO, NORMAL, HEIGHTMAP, CUBEMAP };

	private:

		GLuint                   ID;						///< The OpenGL texture ID.
		bool        textureIsLoaded;						///< Flag indicating whether the texture was successfully loaded.

		TypeTexture            type;						///< The type of the texture (2D or cubemap).

	public:

		/// <summary>
		/// Default constructor for the Texture class.
		/// Initializes the texture with an invalid ID and sets the texture load status to false.
		/// </summary>
		Texture();

		/// <summary>
		/// Destructor for the Texture class.
		/// Deletes the texture from OpenGL if it has been loaded.
		/// </summary>
		~Texture();

	private:

		// Delete the copy constructor and copy assignment operator to prevent copying
		Texture(const Texture&) = delete;
		Texture& operator = (const Texture&) = delete;

	public:

		/// <summary>
		/// Sets the texture ID.
		/// </summary>
		/// 
		/// <param name="id">The OpenGL texture ID.</param>
		void setID(GLuint id);

	public:

		/// <summary>
		/// Checks whether the texture has been successfully loaded.
		/// </summary>
		/// 
		/// <returns>True if the texture is loaded, false otherwise.</returns>
		bool isOk() const;

		/// <summary>
		/// Binds the texture to OpenGL.
		/// </summary>
		/// 
		/// <returns>True if the texture was successfully bound, false otherwise.</returns>
		bool bind() const;

	private:

		/// <summary>
		/// Loads an image from a file into a ColorBuffer object.
		/// </summary>
		/// 
		/// <typeparam name="COLOR_FORMAT">The color format for the buffer.</typeparam>
		/// 
		/// <param name="imagePath">The file path of the image to load.</param>
		/// 
		/// <returns>A unique pointer to the loaded ColorBuffer object, or nullptr if loading failed.</returns>
		template< typename COLOR_FORMAT >
		std::unique_ptr< ColorBuffer< COLOR_FORMAT > > loadImage(const std::string& imagePath, TypeTexture2D texture2DType)
		{
			{
				int imageWidth    = 0;
				int imageHeight   = 0;
				int imageChannels = 0;

				uint8_t* loadedPixels;

				if (texture2DType == HEIGHTMAP)
				{
					loadedPixels = SOIL_load_image
					(
						imagePath.c_str(),
						&imageWidth,
						&imageHeight,
						&imageChannels,
						SOIL_LOAD_L
					);
				}
				else
				{
					loadedPixels = SOIL_load_image
					(
						imagePath.c_str(),
						&imageWidth,
						&imageHeight,
						&imageChannels,
						SOIL_LOAD_RGBA
					);
				}

				if (loadedPixels)
				{
					auto image = std::make_unique< ColorBuffer< COLOR_FORMAT > >(imageWidth, imageHeight);

					// Copy the pixel data into the color buffer
					std::copy_n
					(
						loadedPixels,
						size_t(imageWidth) * size_t(imageHeight) * sizeof(COLOR_FORMAT),
						reinterpret_cast<uint8_t*>(image->colors())
					);

					// Free the image data after copying
					SOIL_free_image_data(loadedPixels);

					return image;
				}

				return nullptr;
			}
		}

	public:

		/// <summary>
		/// Creates a 2D texture from an image file and uploads it to OpenGL.
		/// </summary>
		/// 
		/// <typeparam name="COLOR_FORMAT">The color format for the texture.</typeparam>
		/// 
		/// <param name="texturePath">The file path of the texture image.</param>
		/// <param name="texture2DType">The type of the 2D texture (Albedo, Normal, Heightmap).</param>
		/// 
		/// <returns>The OpenGL texture ID on success, or -1 on failure.</returns>
		template< typename COLOR_FORMAT >
		GLuint createTexture2D(const std::string& texturePath, TypeTexture2D texture2DType)
		{
			auto image = loadImage< COLOR_FORMAT >(texturePath, texture2DType);

			if (image)
			{
				GLuint textureID;

				glEnable(GL_TEXTURE_2D);
				glGenTextures(1, &textureID);
				glBindTexture(GL_TEXTURE_2D, textureID);

				// Set texture parameters
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				// Upload the texture based on its type
				if (texture2DType == ALBEDO)
				{
					glTexImage2D
					(
						GL_TEXTURE_2D,
						0,
						GL_RGBA,
						image->getWidth(),
						image->getHeight(),
						0,
						GL_RGBA,
						GL_UNSIGNED_BYTE,
						image->colors()
					);
				}
				else if (texture2DType == HEIGHTMAP)
				{
					glTexImage2D
					(
						GL_TEXTURE_2D,
						0,
						GL_R8,
						image->getWidth(),
						image->getHeight(),
						0,
						GL_RED,
						GL_UNSIGNED_BYTE,
						image->colors()
					);
				}

				glGenerateMipmap(GL_TEXTURE_2D);

				textureIsLoaded = true;
				type = TEXTURE2D;

				return textureID;
			}

			return -1;
		}

		/// <summary>
		/// Creates a cubemap texture from six images (one for each face) and uploads it to OpenGL.
		/// </summary>
		/// 
		/// <typeparam name="COLOR_FORMAT">The color format for the texture.</typeparam>
		/// 
		/// <param name="texturePath">The file path for the cubemap texture images.</param>
		/// 
		/// <returns>The OpenGL texture ID on success, or -1 on failure.</returns>
		template< typename COLOR_FORMAT >
		GLuint createTextureCubeMap(const std::string& texturePath)
		{
			std::vector< std::unique_ptr< ColorBuffer< COLOR_FORMAT > > > textureSides(6);

			// Load each side of the cubemap
			for (size_t i = 0; i < 6; ++i)
			{
				textureSides[i] = loadImage< COLOR_FORMAT >(texturePath + char('0' + i) + ".png", CUBEMAP);

				if (!textureSides[i])
					return -1;
			}

			GLuint textureID;

			glEnable(GL_TEXTURE_CUBE_MAP);
			glGenTextures(1, &textureID);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

			// Set texture parameters
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

			// Texture targets for the 6 cubemap faces
			static const GLenum textureTarget[] =
			{
				GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
				GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
				GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
				GL_TEXTURE_CUBE_MAP_POSITIVE_X,
				GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
				GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
			};

			// Upload each side of the cubemap
			for (size_t i = 0; i < 6; ++i)
			{
				ColorBuffer< COLOR_FORMAT >& texture = *textureSides[i];

				glTexImage2D
				(
					textureTarget[i],
					0,
					GL_RGBA,
					texture.getWidth(),
					texture.getHeight(),
					0,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					texture.colors()
				);
			}

			textureIsLoaded = true;
			type = TEXTURECUBEMAP;

			return textureID;
		}
	};
}



#endif
