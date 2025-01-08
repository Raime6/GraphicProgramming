
// Public Domain Code
// Author: Xavier Canals

#pragma once



#include "Cone.hpp"
#include "Cylinder.hpp"
#include "Plane.hpp"



#include <string>



namespace meshCreation
{
	using namespace std;
	
	class Scene
	{
		private:

			static const string   vertexShaderCode;
			static const string fragmentShaderCode;

			GLint  modelViewMatrixID;
			GLint projectionMatrixID;

			Cone     cone     = Cone    (2, 1, 12);
			Cylinder cylinder = Cylinder(2, 1, 12);
			Plane    plane    = Plane   (8, 6,  4, 3);
			float    angle;

		public:

			Scene(unsigned width, unsigned height);

			void update();
			void render();
			void resize(unsigned width, unsigned height);
	};
}
