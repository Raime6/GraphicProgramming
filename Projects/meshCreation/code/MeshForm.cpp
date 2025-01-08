
// Public Domain Code
// Author: Xavier Canals

#include "Plane.hpp"



namespace meshCreation
{
	MeshForm::~MeshForm()
	{
		glDeleteVertexArrays(1, &vaoID);
		glDeleteBuffers(VBO_COUNT, vboIDs);
	}



	void MeshForm::render()
	{
		glBindVertexArray(vaoID);
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(index.size()), GL_UNSIGNED_BYTE, 0);
		glBindVertexArray(0);
	}



	void MeshForm::generateForm()
	{
		glGenBuffers(VBO_COUNT, vboIDs);
		glGenVertexArrays(1, &vaoID);

		glBindVertexArray(vaoID);

		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBO_COORDINATES]);
		glBufferData(GL_ARRAY_BUFFER, coordinates.size() * sizeof(coordinates[0]), coordinates.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, vboIDs[VBO_COLORS]);
		glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(colors[0]), colors.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIDs[EBO_INDEX]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(index[0]), index.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
}
