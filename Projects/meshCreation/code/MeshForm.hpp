
// Public Domain Code
// Author: Xavier Canals

#pragma once



#ifndef MESHFORM_HEADER
#define MESHFORM_HEADER



#include <glad/glad.h>
#include <glm.hpp>
#include <vector>



#define PI 3.14159265358979323846



namespace meshCreation
{
	class MeshForm
	{
		protected:

			enum
			{
				VBO_COORDINATES,
				VBO_COLORS,
				EBO_INDEX,
				VBO_COUNT
			};



			std::vector<glm::vec3> coordinates;
			std::vector<glm::vec3>      colors;
			std::vector<GLubyte>         index;

		private:

			GLuint vboIDs[VBO_COUNT];
			GLuint vaoID;

		public:

		   ~MeshForm();

			void render();

		protected:

			void generateForm();
	};
}



#endif
