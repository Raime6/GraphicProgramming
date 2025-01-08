
// Public Domain Code
// Author: Xavier Canals

#include "Plane.hpp"



namespace meshCreation
{	
	Plane::Plane(float width, float height, unsigned cols, unsigned rows)
	{
		buildPlane(width, height, rows, cols);

		generateForm();
	}



	void Plane::buildPlane(float width, float height, unsigned cols, unsigned rows)
	{
		coordinates.clear();
		colors     .clear();
		index      .clear();

		glm::vec3 vertex;

		float     tile_width   = width  / float(cols);
		float     title_height = height / float(rows);
		int       vertexIndex  = 0;

		vertex.y = 0.f;
		
		// Plane coordiantes generation
		for (unsigned i = 0; i < rows + 1; ++i)
		{
			vertex.x = -width / 2.f;

			for (unsigned j = 0; j < cols + 1; ++j)
			{				
				vertex.z = -height / 2.f + i * title_height;

				coordinates.push_back(vertex);
				colors     .push_back(glm::vec3{ 1, 0, 1 });
				
				vertex.x += tile_width;
				++vertexIndex;
			}
		}
		
		// Plane's triangles index generation
		for (unsigned i = 0; i < rows; ++i)
		{
			for (unsigned j = 0; j < cols; ++j)
			{
				int topLeft     = ( i      * (cols + 1) +  j);
				int topRight    = ((i + 1) * (cols + 1) +  j);
				int bottomLeft  = ( i      * (cols + 1) + (j + 1));
				int bottomRight = ((i + 1) * (cols + 1) + (j + 1));

				index.push_back(   topLeft);
				index.push_back(  topRight);
				index.push_back(bottomLeft);

				index.push_back(   topRight);
				index.push_back(bottomRight);
				index.push_back( bottomLeft);
			}
		}
	}
}
