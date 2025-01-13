
// Public Domain Code
// Author: Xavier Canals

#pragma once



#ifndef POSTPROCESS_HEADER
#define POSTPROCESS_HEADER







namespace finalPractice
{
	class Postprocess
	{
	private:

		glm::vec4 lightPosition;
		glm::vec3 lightColor;

		float ambientIntensity;
		float diffuseIntensity;

	public:
		Postprocess();


	};
}



#endif
