
// Public Domain Code
// Author: Xavier Canals

#pragma once



#ifndef PLANE_HEADER
#define PLANE_HEADER



#include "MeshForm.hpp"



namespace meshCreation
{
	class Plane : public MeshForm
	{
		public:

			Plane(float width, float height, unsigned cols, unsigned rows);
	
		private:
			void buildPlane(float width, float height, unsigned cols, unsigned rows);
	};
}



#endif
