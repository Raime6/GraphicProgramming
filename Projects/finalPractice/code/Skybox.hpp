
// Public Domain Code
// Author: Xavier Canals

#pragma once



#ifndef SKYBOX_HEADER
#define SKYBOX_HEADER



#include "Camera.hpp"
#include "Shader.hpp"
#include "Texture.hpp"



namespace finalPractice
{
	class Skybox
	{
		private:

			static const GLfloat          coordinates[];
			static const std::string   vertexShaderCode;
			static const std::string fragmentShaderCode;

			Shader				shader;

		private:

			GLuint				 vboID;
			GLuint				 vaoID;

			GLint    modelViewMatrixID;
			GLint   projectionMatrixID;

			Texture            texture;

		public:

			Skybox(const std::string & texturePath);
		   ~Skybox();

		public:

			void render(const Camera & camera);
	};
}



#endif
