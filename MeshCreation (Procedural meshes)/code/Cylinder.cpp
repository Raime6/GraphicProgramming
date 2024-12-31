
// Public Domain Code
// Author: Xavier Canals

#include "Cylinder.hpp"



namespace meshCreation
{
	const GLfloat Cylinder::coordinates[] =
	{
		+2    , +0, +0    , // 0  (Bottom Base Center)
		+2    , +2, +0    , // 1  (Top    Base Center)
		+3    , +0, +0    , // 2  (Bottom Base)
		+2.87f, +0, +0.5f , // 3
		+2.5f , +0, +0.87f, // 4
		+2    , +0, +1    , // 5
		+1.5f , +0, +0.87f, // 6
		+1.13f, +0, +0.5f , // 7
		+1    , +0, +0    , // 8
		+1.13f, +0, -0.5f , // 9
		+1.5f , +0, -0.87f, // 10
		+2    , +0, -1    , // 11
		+2.5f , +0, -0.87f, // 12
		+2.87f, +0, -0.5f , // 13
		+3    , +2, +0    , // 14 (Top    Base)
		+2.87f, +2, +0.5f , // 15
		+2.5f , +2, +0.87f, // 16
		+2    , +2, +1    , // 17
		+1.5f , +2, +0.87f, // 18
		+1.13f, +2, +0.5f , // 19
		+1    , +2, +0    , // 20
		+1.13f, +2, -0.5f , // 21
		+1.5f , +2, -0.87f, // 22
		+2    , +2, -1    , // 23
		+2.5f , +2, -0.87f, // 24
		+2.87f, +2, -0.5f , // 25
	};

	const GLfloat Cylinder::colors[] =
	{
		0, 1, 1, // 0
		0, 1, 0, // 1
		0, 1, 1, // 2
		0, 1, 1, // 3
		0, 1, 1, // 4
		0, 1, 1, // 5
		0, 1, 1, // 6
		0, 1, 1, // 7
		0, 1, 1, // 8
		0, 1, 1, // 9
		0, 1, 1, // 10
		0, 1, 1, // 11
		0, 1, 1, // 12
		0, 1, 1, // 13
		0, 1, 0, // 14
		0, 1, 0, // 15
		0, 1, 0, // 16
		0, 1, 0, // 17
		0, 1, 0, // 18
		0, 1, 0, // 19
		0, 1, 0, // 20
		0, 1, 0, // 21
		0, 1, 0, // 22
		0, 1, 0, // 23
		0, 1, 0, // 24
		0, 1, 0, // 25
	};

	const GLubyte Cylinder::index[] =
	{
		 0,  2,  3, // Bottom Base
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
		 1, 15, 14, // Top    Base
		 1, 16, 15,
		 1, 17, 16,
		 1, 18, 17,
		 1, 19, 18,
		 1, 20, 19,
		 1, 21, 20,
		 1, 22, 21,
		 1, 23, 22,
		 1, 24, 23,
		 1, 25, 24,
		 1, 14, 25,
		 3,  2, 14, // Body
		14, 15,  3,
		 4,  3, 15,
		15, 16,  4,
		 5,  4, 16,
		16, 17,  5,
		 6,  5, 17,
		17, 18,  6,
		 7,  6, 18,
		18, 19,  7,
		 8,  7, 19,
		19, 20,  8,
		 9,  8, 20,
		20, 21,  9,
		10,  9, 21,
		21, 22, 10,
		11, 10, 22,
		22, 23, 11,
		12, 11, 23,
		23, 24, 12,
		13, 12, 24,
		24, 25, 13,
		 2, 13, 25,
		25, 14,  2,
	};



	Cylinder::Cylinder()
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

	Cylinder::~Cylinder()
	{
		glDeleteVertexArrays(1, &vaoID);
		glDeleteBuffers(VBO_COUNT, vboIDs);
	}



	void Cylinder::render()
	{
		glBindVertexArray(vaoID);
		glDrawElements(GL_TRIANGLES, sizeof(index), GL_UNSIGNED_BYTE, 0);
		glBindVertexArray(0);
	}
}
