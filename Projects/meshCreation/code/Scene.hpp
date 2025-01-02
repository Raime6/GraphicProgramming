
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

			Cone     cone;
			Cylinder cylinder;
			Plane    plane;
			float    angle;

		public:

			Scene(unsigned width, unsigned height);

			void update();
			void render();
			void resize(unsigned width, unsigned height);

		private:

			GLuint compileShaders      ();
			void   showCompilationError(GLuint  shaderID);
			void   showLinkageError    (GLuint programID);

	};
}
