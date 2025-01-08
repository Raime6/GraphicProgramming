
// Public Domain Code
// Author: Xavier Canals

#pragma once



#ifndef CONE_HEADER
#define CONE_HEADER



#include "MeshForm.hpp"



namespace meshCreation
{
	class Cone : public MeshForm
	{
		public:

			 Cone(float height, float radius, unsigned numBaseVertex);

		private:

			void buildCone(float height, float radius, unsigned numBaseVertex);
	};
}



#endif
