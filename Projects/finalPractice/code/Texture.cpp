
/*
	Public Domain Code

	Author: Xavier Canals
*/

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



	void Texture::setID(GLuint id)
	{
		ID = id;
	}



	bool Texture::isOk() const
	{
		return textureIsLoaded;
	}

	bool Texture::bind() const
	{
		if (textureIsLoaded)
		{
			switch (type)
			{
			case TEXTURE2D:
			{
				glBindTexture(GL_TEXTURE_2D, ID);
				break;
			}
			case CUBEMAP:
			{
				glBindTexture(GL_TEXTURE_CUBE_MAP, ID);
				break;
			}
			}

			return true;
		}

		return false;
	}
}
