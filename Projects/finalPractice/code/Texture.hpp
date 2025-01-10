
// Public Domain Code
// Author: Xavier Canals

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
	class Texture
	{
		enum TypeTexture{ NO_TYPE, TEXTURE2D, CUBEMAP };

		private:

			GLuint                   ID;
			bool        textureIsLoaded;

			TypeTexture            type;

		public:

			Texture();
		   ~Texture();

		private:

			Texture			    (const Texture &) = delete;
			Texture& operator = (const Texture &) = delete;

		public:

			void setID(GLuint id);

		public:

			bool isOk() const;

			bool bind() const;

		private:

			template< typename COLOR_FORMAT >
			std::unique_ptr< ColorBuffer< COLOR_FORMAT > > loadImage(const std::string& imagePath)
			{
				{
					int imageWidth = 0;
					int imageHeight = 0;
					int imageChannels = 0;

					uint8_t* loadedPixels = SOIL_load_image
					(
						imagePath.c_str(),
						&imageWidth,
						&imageHeight,
						&imageChannels,
						SOIL_LOAD_RGBA
					);

					if (loadedPixels)
					{
						auto image = std::make_unique< ColorBuffer< COLOR_FORMAT > >(imageWidth, imageHeight);

						std::copy_n
						(
							loadedPixels,
							size_t(imageWidth) * size_t(imageHeight) * sizeof(COLOR_FORMAT),
							reinterpret_cast<uint8_t*>(image->colors())
						);

						SOIL_free_image_data(loadedPixels);

						return image;
					}

					return nullptr;
				}
			}

		public:

			template< typename COLOR_FORMAT >
			GLuint createTexture2D(const std::string& texturePath)
			{
				auto image = loadImage< COLOR_FORMAT >(texturePath);

				if (image)
				{
					GLuint textureID;

					glEnable(GL_TEXTURE_2D);
					glGenTextures(1, &textureID);
					glBindTexture(GL_TEXTURE_2D, textureID);

					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

					glGenerateMipmap(GL_TEXTURE_2D);

					textureIsLoaded = true;
					type = TEXTURE2D;

					return textureID;
				}

				return -1;
			}

			template< typename COLOR_FORMAT >
			GLuint createTextureCubeMap(const std::string& texturePath)
			{
				std::vector< std::unique_ptr< ColorBuffer< COLOR_FORMAT > > > textureSides(6);

				for (size_t i = 0; i < 6; ++i)
				{
					textureSides[i] = loadImage< COLOR_FORMAT >(texturePath + char('0' + i) + ".png");

					if (!textureSides[i])
						return -1;
				}

				GLuint textureID;

				glEnable(GL_TEXTURE_CUBE_MAP);
				glGenTextures(1, &textureID);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

				static const GLenum textureTarget[] =
				{
					GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
					GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
					GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
					GL_TEXTURE_CUBE_MAP_POSITIVE_X,
					GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
					GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
				};

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
				type = CUBEMAP;

				return textureID;
			}
	};
}



#endif
