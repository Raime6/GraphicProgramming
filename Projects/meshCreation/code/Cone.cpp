
// Public Domain Code
// Author: Xavier Canals

#include "Cone.hpp"



namespace meshCreation
{
	const GLfloat Cone::coordinates[] =
	{
		-2    , +0, +0    , // 0 (Base Center)
		-2    , +2, +0    , // 1 (Height)
		-1    , +0, +0    , // 2
		-1.13f, +0, +0.5f , // 3
		-1.5f , +0, +0.87f, // 4
		-2    , +0, +1    , // 5
		-2.5f , +0, +0.87f, // 6
		-2.87f, +0, +0.5f , // 7
		-3    , +0, +0    , // 8
		-2.87f, +0, -0.5f , // 9
		-2.5f , +0, -0.87f, // 10
		-2    , +0, -1    , // 11
		-1.5f , +0, -0.87f, // 12
		-1.13f, +0, -0.5f , // 13
	};

	const GLfloat Cone::colors[] =
	{
		1, 0, 0, // 0
		0, 0, 1, // 1
		1, 0, 0, // 2
		1, 0, 0, // 3
		1, 0, 0, // 4
		1, 0, 0, // 5
		1, 0, 0, // 6
		1, 0, 0, // 7
		1, 0, 0, // 8
		1, 0, 0, // 9
		1, 0, 0, // 10
		1, 0, 0, // 11
		1, 0, 0, // 12
		1, 0, 0, // 13
	};

	const GLubyte Cone::index[] =
	{
		 0,  2,  3, // Base
		 0,  3,  4,
		 0,  4,  5,
		 0,  5,  6,
		 0,  6,  7,
		 0,  7,  8,
		 0,  8,  9,
		 0,  9, 10,
		 0, 10, 11,
		 0, 11, 12,
		 0, 12, 13,
		 0, 13,  2,
		 1,  3,  2, // Body
		 1,  4,  3,
		 1,  5,  4,
		 1,  6,  5,
		 1,  7,  6,
		 1,  8,  7,
		 1,  9,  8,
		 1, 10,  9,
		 1, 11, 10,
		 1, 12, 11,
		 1, 13, 12,
		 1,  2, 13,
	};



	Cone::Cone()
	{
		glGenBuffers(VBO_COUNT, vboIDs);
		glGenVertexArrays(1, &vaoID);

		glBindVertexArray(vaoID);

		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBO_COORDINATES]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(coordinates), coordinates, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBO_COLORS]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIDs[EBO_INDEX]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

		glBindVertexArray(0);
	}

	Cone::~Cone()
	{
		glDeleteVertexArrays(1, &vaoID);
		glDeleteBuffers(VBO_COUNT, vboIDs);
	}



	void Cone::render()
	{
		glBindVertexArray(vaoID);
		glDrawElements(GL_TRIANGLES, sizeof(index), GL_UNSIGNED_BYTE, 0);
		glBindVertexArray(0);
	}
}
