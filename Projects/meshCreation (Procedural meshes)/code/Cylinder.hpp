
// Public Domain Code
// Author: Xavier Canals

#pragma once



#include <glad/glad.h>



#ifndef CYLINDER_HEADER
#define CYLINDER_HEADER



namespace meshCreation
{
	class Cylinder
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
			static const GLfloat colors[];
			static const GLubyte index[];

		private:

			GLuint vboIDs[VBO_COUNT];
			GLuint vaoID;

		public:

			Cylinder();
			~Cylinder();

			void render();
	};
}



#endif
