
// Public Domain Code
// Author: Xavier Canals

#pragma once



#ifndef CYLINDER_HEADER
#define CYLINDER_HEADER



#include "MeshForm.hpp"



namespace meshCreation
{
	class Cylinder : public MeshForm
	{
		public:

			Cylinder(float height, float radius, unsigned numBaseVertex);

		private:

			void buildCylinder(float height, float radius, unsigned numBaseVertex);
	};
}



#endif
