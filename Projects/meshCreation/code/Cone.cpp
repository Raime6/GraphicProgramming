
// Public Domain Code
// Author: Xavier Canals

#include "Cone.hpp"



namespace meshCreation
{
	Cone::Cone(float height, float radius, unsigned numBaseVertex)
	{
		buildCone(height, radius, numBaseVertex);

		generateForm();
	}



	void Cone::buildCone(float height, float radius, unsigned numBaseVertex)
	{
		coordinates.clear();
		colors     .clear();
		index      .clear();

		// Cone base center
		glm::vec3 baseCenter(-2.f, 0.f, 0.f);
		coordinates.push_back(baseCenter);
		colors     .push_back(glm::vec3(1.f, 0.f, 0.f));

		// Cone apex
		glm::vec3 coneApex(-2.f, height, 0.f);
		coordinates.push_back(coneApex);
		colors     .push_back(glm::vec3(0.f, 0.f, 1.f));

		// Cone base vertex generation
		float angleIncrement = 2.f * (float)PI / numBaseVertex;
		
		for (unsigned i = 0; i < numBaseVertex; ++i)
		{
			float angle = i * angleIncrement;
			float     x = cos(angle) * radius;
			float     z = sin(angle) * radius;

			glm::vec3 baseVertex(baseCenter.x + x, baseCenter.y, baseCenter.z + z);
			coordinates.push_back(baseVertex);
			colors.push_back(glm::vec3(1.f, 0.f, 0.f));
		}

		// Cone's base triangles index generation
		for (unsigned i = 0; i < numBaseVertex; ++i)
		{
			unsigned next = (i + 1) % numBaseVertex;
			index.push_back(       0);
			index.push_back(   i + 2);
			index.push_back(next + 2);
		}

		// Cone's body triangles index generation
		for (unsigned i = 0; i < numBaseVertex; ++i)
		{
			unsigned next = (i + 1) % numBaseVertex;
			index.push_back(       1);
			index.push_back(next + 2);
			index.push_back(   i + 2);
		}
	}
}
