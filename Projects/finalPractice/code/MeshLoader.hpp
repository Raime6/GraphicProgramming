
// Public Domain Code
// Xavier Canals

#pragma once



#ifndef MESHLOADER_HEADER
#define MESHLOADER_HEADER



#include "Camera.hpp"
#include "Shader.hpp"
#include "Texture.hpp"



#include <glad/glad.h>
#include <string>



namespace finalPractice
{
	class MeshLoader
	{
		private:

			enum
			{
				VBO_COORDINATES,
				VBO_COLORS,
				EBO_INDEX,
				VBO_COUNT
			};

		private:

			static const std::string   vertexShaderCode;
			static const std::string fragmentShaderCode;

			Shader			   shader;
			Texture           texture;

		private:

			GLuint  vboIDs[VBO_COUNT];
			GLuint              vaoID;

			GLsizei         numIndex;

			GLint   modelViewMatrixID;
			GLint  projectionMatrixID;

		public:

			MeshLoader(const std::string& meshFilePath, const std::string& texturePath);
		   ~MeshLoader();

		public:

			void render(const Camera & camera);
			void resize(int width, int height);

		private:

			void loadMesh(const std::string & meshFilePath);
	};
}



#endif

