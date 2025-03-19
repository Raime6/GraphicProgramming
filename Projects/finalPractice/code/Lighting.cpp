
/*
	Public Domain Code

	Author: Xavier Canals
*/

#include "Lighting.hpp"



namespace finalPractice
{
	Lighting::Lighting() :
		lightPosition   (3.f, 3.f, 3.f, 1.f),
		lightColor      (1.f, 1.f, 1.f),
		ambientIntensity(.2f),
		diffuseIntensity(.8f)
	{}



	void Lighting::configureLight(GLuint shaderID)
	{
		// Gets the uniform attributes of the light in the shader
		GLint lightPositionLoc    = glGetUniformLocation(shaderID, "light.position"   );
		GLint lightColorLoc       = glGetUniformLocation(shaderID, "light.color"      );
		GLint ambientIntensityLoc = glGetUniformLocation(shaderID, "ambient_intensity");
		GLint diffuseIntensityLoc = glGetUniformLocation(shaderID, "diffuse_intensity");

		// Sets the uniform values in the shader program.
		glUniform4f(lightPositionLoc   , lightPosition.x, lightPosition.y, lightPosition.z, lightPosition.w);
		glUniform3f(lightColorLoc      , lightColor.x   , lightColor.y   , lightColor.z);
		glUniform1f(ambientIntensityLoc, ambientIntensity);
		glUniform1f(diffuseIntensityLoc, diffuseIntensity);
	}

}
