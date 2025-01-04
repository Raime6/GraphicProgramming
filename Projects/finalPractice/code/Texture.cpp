
// Public Domain Code
// Author: Xavier Canals

#include "Texture.hpp"



#include <SOIL2.h>



namespace finalPractice
{
	Texture::Texture()
	{
		ID                 = -1;
		textureIsLoaded = false;
		type =			NO_TYPE;
	}

	Texture::~Texture()
	{
		if (textureIsLoaded)
			glDeleteTextures(1, &ID);
	}
}
