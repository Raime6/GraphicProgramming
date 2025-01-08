
// Public Domain Code
// Xavier Canals

#pragma once



#ifndef MESHLOADER_HEADER
#define MESHLOADER_HEADER



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

			GLuint  vboIDs[VBO_COUNT];
			GLuint              vaoID;

			GLsizei         numIndex;

			GLint   modelViewMatrixId;
			GLint  projectionMatrixID;

			float               angle;

		public:

			MeshLoader(const std::string& meshFilePath);
		   ~MeshLoader();

		public:

			void update();
			void render();
			void resize(int width, int height);

		private:

			void loadMesh(const std::string & meshFilePath);
	};
}



#endif

