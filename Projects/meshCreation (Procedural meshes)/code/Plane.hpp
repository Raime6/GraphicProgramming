
// Public Domain Code
// Author: Xavier Canals

#pragma once



#ifndef PLANE_HEADER
#define PLANE_HEADER



#include <glad/glad.h>
#include <glm.hpp>
#include <iostream>
#include <vector>



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



			std::vector<glm::vec3> coordinates;
			static const GLfloat   colors[];
			std::vector<int>       index;

		private:

			GLuint vboIDs[VBO_COUNT];
			GLuint vaoID;

		public:

			Plane(float width, float height, unsigned cols, unsigned rows);
			~Plane();

			 void render();
			 void buildPlane(float width, float height, unsigned cols, unsigned rows);
	};
}



#endif

