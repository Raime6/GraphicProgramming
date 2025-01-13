
// Public Domain Code
// Author: Xavier Canals

#pragma once



#ifndef LIGHTING_HEADER
#define LIGHTING_HEADER



#include <glm.hpp>
#include <glad/glad.h>



namespace finalPractice
{
	class Lighting
	{
	private:

		glm::vec4 lightPosition;
		glm::vec3 lightColor;

		float ambientIntensity;
		float diffuseIntensity;

	public:
		Lighting();

		void configureLight(GLuint shaderID);
	};
}



#endif
