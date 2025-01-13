
// Public Domain Code
// Xavier Canals

#pragma once



#ifndef MESHLOADER_HEADER
#define MESHLOADER_HEADER



#include "Camera.hpp"
#include "Lighting.hpp"
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
				VBO_NORMALS,
				EBO_INDEX,
				VBO_COUNT
			};

		private:

			static const std::string          vertexShaderCode;
			static const std::string        fragmentShaderCode;
			static const std::string   vertexShaderCodeTexture;
			static const std::string fragmentShaderCodeTexture;

			Shader			   shader;
			Lighting	     lighting;
			Texture           texture;
			//Texture     textureNormal;

		private:

			GLuint  vboIDs[VBO_COUNT];
			GLuint              vaoID;

			GLsizei         numIndex;

			GLint   modelViewMatrixID;
			GLint  projectionMatrixID;
			GLint      normalMatrixID;

			bool		  needTexture;
			bool			 moveDown;
			float               angle;
			float                posY;

		public:

			MeshLoader(const std::string & meshFilePath);
			MeshLoader(const std::string & meshFilePath, const std::string & textureAlbedoPath);
		   ~MeshLoader();

		public:

			void  update();
			void  render(const Camera& camera, glm::vec3 tanslateVector, float angle, glm::vec3 rotateVector, glm::vec3 scaleVector);
			void  resize(int width, int height);

			float getAngle();
			float getPosY ();

		private:

			void loadMesh(const std::string & meshFilePath);
			void configureMaterial(GLuint shaderID);

			void crystalAnimation();
	};
}



#endif

