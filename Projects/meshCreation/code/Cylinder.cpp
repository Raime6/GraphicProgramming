
// Public Domain Code
// Author: Xavier Canals

#include "Cylinder.hpp"



namespace meshCreation
{
	Cylinder::Cylinder(float height, float radius, unsigned numBaseVertex)
	{
		buildCylinder(height, radius, numBaseVertex);

		generateForm();
	}



	void Cylinder::buildCylinder(float height, float radius, unsigned numBaseVertex)
	{
		coordinates.clear();
		colors.clear();
		index.clear();

		// Cylinder base center
		glm::vec3 baseCenter(2.f, 0.f, 0.f);
		coordinates.push_back(baseCenter);
		colors.push_back(glm::vec3(0.f, 1.f, 1.f));

		// Cylinder top center
		glm::vec3 topCenter(2.f, height, 0.f);
		coordinates.push_back(topCenter);
		colors.push_back(glm::vec3(0.f, 1.f, 0.f));

		// Cylinder base vertex generation
		float angleIncrement = 2.f * (float)PI / numBaseVertex;

		for (unsigned i = 0; i < numBaseVertex; ++i)
		{
			float angle = i * angleIncrement;
			float     x = cos(angle) * radius;
			float     z = sin(angle) * radius;

			glm::vec3 baseVertex(baseCenter.x + x, baseCenter.y, baseCenter.z + z);
			coordinates.push_back(baseVertex);
			colors     .push_back(glm::vec3(0.f, 1.f, 1.f));
		}

		// Cylinder top vertex generation
		for (unsigned i = 0; i < numBaseVertex; ++i)
		{
			glm::vec3 topVertex(coordinates[i + 2].x, coordinates[i + 2].y + height, coordinates[i + 2].z);
			coordinates.push_back(topVertex);
			colors     .push_back(glm::vec3(0.f, 1.f, 0.f));
		}

		// Cylinder's base triangles index generation
		for (unsigned i = 0; i < numBaseVertex; ++i)
		{
			unsigned next = (i + 1) % numBaseVertex;
			index.push_back(       0);
			index.push_back(   i + 2);
			index.push_back(next + 2);
		}

		// Cylinder's top triangles index generation
		for (unsigned i = 0; i < numBaseVertex; ++i)
		{
			unsigned next = (i + 1) % numBaseVertex;
			index.push_back(       1);
			index.push_back(next + 2 + numBaseVertex);
			index.push_back(   i + 2 + numBaseVertex);
		}

		// Cylinder's body triangles index generation
		for (unsigned i = 0; i < numBaseVertex; ++i)
		{
			unsigned next = (i + 1) % numBaseVertex;

			index.push_back(   i + 2);
			index.push_back(   i + 2 + numBaseVertex);
			index.push_back(next + 2);

			index.push_back(   i + 2 + numBaseVertex);
			index.push_back(next + 2 + numBaseVertex);
			index.push_back(next + 2);
		}
	}
}
