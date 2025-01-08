
// Public Domain Code
// Author: Xavier Canals

#pragma once



#ifndef TERRAIN_HEADER
#define TERRAIN_HEADER



#include "Camera.hpp"
#include "Color.hpp"
#include "ColorBuffer.hpp"
#include "Texture.hpp"



#include <Shader.hpp>



namespace finalPractice
{
	class Terrain
	{
		private:

			enum
			{
				VBO_COORDINATES,
				VBO_TEXTURE_UVS,
				VBO_COUNT
			};

		private:

			static const std::string   vertexShaderCode;
			static const std::string fragmentShaderCode;
			static const std::string        texturePath;

		private:

			GLuint  vboIDs[VBO_COUNT];
			GLuint              vaoID;

			GLsizei         numVertex;

		private:

			GLuint    shaderProgramID;
			Texture           texture;

			GLint	modelViewMatrixID;
			GLint  projectionMatrixID;

		private:

			float               angle;

		public:

			Terrain(float width, float depth, unsigned xSlices, unsigned zSlices, const std::string& texturePath);
		   ~Terrain();

			void update();

			void render(const Camera & camera);
			void resize(int width, int height);
	};
}



#endif
