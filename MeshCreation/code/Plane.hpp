
// Public Domain Code
// Author: Xavier Canals

#pragma once



#ifndef PLANE_HEADER
#define PLANE_HEADER



#include <glad/glad.h>



namespace meshCreation
{
	class Plane
	{
		private:
		
			enum
			{
				VBO_COORDINATES,
				VBO_COLORS,
				EBO_INDEX,
				VBO_COUNT
			};



			static const GLfloat coordinates[];
			static const GLfloat colors     [];
			static const GLubyte index      [];

		private:

			GLuint vboIDs[VBO_COUNT];
			GLuint vaoID;

		public:

			 Plane();
			~Plane();

			 void render();
			 void buildPlane();
	};
}



#endif

