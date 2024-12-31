
// Public Domain Code
// Author: Xavier Canals

#include "Plane.hpp"



namespace meshCreation
{
	const GLfloat Plane::coordinates[] =
	{
		-4, +0, -3, // 0
		-2, +0, -3, // 1
		+0, +0, -3, // 2
		+2, +0, -3, // 3
		+4, +0, -3, // 4
		+4, +0, -1, // 5
		+2, +0, -1, // 6
		-0, +0, -1, // 7
		-2, +0, -1, // 8
		-4, +0, -1, // 9
		-4, +0, +1, // 10
		-2, +0, +1, // 11
		+0, +0, +1, // 12
		+2, +0, +1, // 13
		+4, +0, +1, // 14
		+4, +0, +3, // 15
		+2, +0, +3, // 16
		+0, +0, +3, // 17
		-2, +0, +3, // 18
		-4, +0, +3, // 19
	};

	const GLfloat Plane::colors[] =
	{
		1, 0, 1, // 0
		1, 0, 1, // 1
		1, 0, 1, // 2
		1, 0, 1, // 3
		1, 0, 1, // 4
		1, 0, 1, // 5
		1, 0, 1, // 6
		1, 0, 1, // 7
		1, 0, 1, // 8
		1, 0, 1, // 9
		1, 0, 1, // 10
		1, 0, 1, // 11
		1, 0, 1, // 12
		1, 0, 1, // 13
		1, 0, 1, // 14
		1, 0, 1, // 15
		1, 0, 1, // 16
		1, 0, 1, // 17
		1, 0, 1, // 18
		1, 0, 1, // 19

	};

	const GLubyte Plane::index[] =
	{
		 0,  9,  8,
		 0,  8,  1,
		 1,  8,  7,
		 1,  7,  2,
		 2,  7,  6,
		 2,  6,  3,
		 3,  6,  5,
		 3,  5,  4,
		 6, 14,  5,
		 6, 13, 14,
		 7, 13,  6,
		 7, 12, 13,
		 8, 12,  7,
		 8, 11, 12,
		 9, 11,  8,
		 9, 10, 11,
		10, 19, 18,
		10, 18, 11,
		11, 18, 17,
		11, 17, 12,
		12, 17, 16,
		12, 16, 13,
		13, 16, 15,
		13, 15, 14,
	};



	Plane::Plane()
	{
		glGenBuffers     (VBO_COUNT, vboIDs);
		glGenVertexArrays(1        , &vaoID);

		glBindVertexArray(vaoID);

		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBO_COORDINATES]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(coordinates), coordinates, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer    (0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBO_COLORS]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer    (1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIDs[EBO_INDEX]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	Plane::~Plane()
	{
		glDeleteVertexArrays(1        , &vaoID);
		glDeleteBuffers     (VBO_COUNT, vboIDs);
	}



	void Plane::render()
	{
		glBindVertexArray(vaoID);
		glDrawElements   (GL_TRIANGLES, sizeof(index), GL_UNSIGNED_BYTE, 0);
		glBindVertexArray(0);
	}
}
