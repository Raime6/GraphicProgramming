
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

			static const std::string          vertexShaderCode;
			static const std::string        fragmentShaderCode;
			static const std::string   vertexShaderCodeTexture;
			static const std::string fragmentShaderCodeTexture;

			Shader			   shader;
			Texture           texture;
			//Texture     textureNormal;

		private:

			GLuint  vboIDs[VBO_COUNT];
			GLuint              vaoID;

			GLsizei         numIndex;

			GLint   modelViewMatrixID;
			GLint  projectionMatrixID;

			bool		  needTexture;

		public:

			MeshLoader(const std::string & meshFilePath);
			MeshLoader(const std::string & meshFilePath, const std::string & textureAlbedoPath);
		   ~MeshLoader();

		public:

			void render(const Camera& camera, glm::vec3 tanslateVector, float angle, glm::vec3 rotateVector, glm::vec3 scaleVector);
			void resize(int width, int height);

		private:

			void loadMesh(const std::string & meshFilePath);
	};
}



#endif

