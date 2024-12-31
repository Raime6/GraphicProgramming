
// Public Domain Code
// Author: Xavier Canals

#include "Plane.hpp"



namespace meshCreation
{	
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



	Plane::Plane(float width, float height, unsigned cols, unsigned rows)
	{
		buildPlane(width, height, rows, cols);
		
		glGenBuffers     (VBO_COUNT, vboIDs);
		glGenVertexArrays(1        , &vaoID);

		glBindVertexArray(vaoID);

		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBO_COORDINATES]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(coordinates), coordinates.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer    (0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBO_COLORS]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer    (1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIDs[EBO_INDEX]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index.data(), GL_STATIC_DRAW);

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

	void Plane::buildPlane(float width, float height, unsigned cols, unsigned rows)
	{
		coordinates.reserve((cols + 1) * (rows + 1));

		glm::vec3 vertex;

		float     tile_width   = width  / float(cols);
		float     title_height = height / float(rows);
		int       vertexIndex  = 0;
		
		for (unsigned i = 0; i < cols + 1; ++i)
		{
			vertex = glm::vec3{ -width / 2.f, -height / 2.f, 0.f };

			for (unsigned j = 0; j < rows + 1; ++j)
			{				
				coordinates.push_back(vertex);
				//index      .push_back(vertexIndex);
				
				vertex.x += tile_width;
				++vertexIndex;
			}
			vertex.y += tile_width;
		}
		index.push_back(0);
		index.push_back(11);
		index.push_back(10);
	}
}
