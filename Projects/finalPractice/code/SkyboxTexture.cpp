
// Public Domain Code
// Author: Xavier Canals

#include "SkyboxTexture.hpp"



#include <SOIL2.h>



namespace finalPractice
{
	SkyboxTexture::SkyboxTexture(const std::string & textureBasePath)
	{
		textureIsLoaded = false;

		std::vector< std::shared_ptr< ColorBuffer > > textureSides(6);

		for (size_t i = 0; i < 6; ++i)
		{
			textureSides[i] = loadImage(textureBasePath + char('0' + i) + ".png");

			if (!textureSides[i])
				return;
		}

		glEnable	   (GL_TEXTURE_CUBE_MAP);
		glGenTextures  (1, &textureID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture  (GL_TEXTURE_CUBE_MAP, textureID);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S    , GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T    , GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R    , GL_CLAMP_TO_EDGE);

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
			ColorBuffer & texture = *textureSides[i];

			glTexImage2D
			(
				textureTarget[i]   ,
				0				   ,
				GL_RGBA			   ,
				texture.getWidth() ,
				texture.getHeight(),
				0				   ,
				GL_RGBA			   ,
				GL_UNSIGNED_BYTE   ,
				texture.colors()
			);
		}

		textureIsLoaded = true;
	}

	std::shared_ptr< SkyboxTexture::ColorBuffer > SkyboxTexture::loadImage(const std::string & imagePath)
	{
		int imageWidth    = 0;
		int imageHeight   = 0;
		int imageChannels = 0;

		uint8_t* loadedPixels = SOIL_load_image
		(
			imagePath.c_str(),
		   &imageWidth		 ,
		   &imageHeight		 ,
		   &imageChannels	 ,
			SOIL_LOAD_RGBA
		);

		if (loadedPixels)
		{
			auto image = std::make_shared< ColorBuffer >(imageWidth, imageHeight);

			std::copy_n
			(
				loadedPixels														 ,
				size_t(imageWidth) * size_t(imageHeight) * sizeof(ColorBuffer::Color),
				reinterpret_cast< uint8_t * >(image->colors())
			);

			SOIL_free_image_data(loadedPixels);

			return image;
		}

		return nullptr;
	}

	SkyboxTexture::~SkyboxTexture()
	{
		if (textureIsLoaded)
			glDeleteTextures(1, &textureID);
	}
}
