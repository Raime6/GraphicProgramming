
// Public Domain Code
// Author: Xavier Canals

#pragma once



#ifndef SKYBOXTEXTURE_HEADER
#define SKYBOXTEXTURE_HEADER



#include "Color.hpp"
#include "ColorBuffer.hpp"



#include <glad/glad.h>
#include <memory>
#include <string>



namespace finalPractice
{
	class SkyboxTexture
	{
		private:

			typedef ColorBuffer< Rgba8888 > ColorBuffer;

		private:

			GLuint textureID;
			bool   textureIsLoaded;

		public:

			SkyboxTexture(const std::string & textureBasePath);
		   ~SkyboxTexture();

		private:

			SkyboxTexture			   (const SkyboxTexture &) = delete;
			SkyboxTexture & operator = (const SkyboxTexture &) = delete;

		private:
			
			std::shared_ptr< ColorBuffer > loadImage(const std::string & imagePath);

		public:

			bool isOk() const
			{
				return textureIsLoaded;
			}

			bool bind() const
			{
				return textureIsLoaded ? glBindTexture(GL_TEXTURE_CUBE_MAP, textureID), true : false;
			}
	};
}



#endif
