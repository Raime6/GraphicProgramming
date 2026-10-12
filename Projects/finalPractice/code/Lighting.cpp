
/*
	Public Domain Code

	Author: Xavier Canals
*/

#include "Lighting.hpp"



namespace finalPractice
{
	Lighting::Lighting() :
		lightPosition   (3.f, 5.f, 3.f, 1.f),
		lightColor      (1.f, 1.f, 1.f),
		ambientIntensity(.18f),
		diffuseIntensity(.85f),
		specularIntensity(1.f),
		attenuationConstant(1.f),
		attenuationLinear(.014f),
		attenuationQuadratic(.0007f)
	{}



	void Lighting::configureLight(GLuint shaderID, const glm::mat4& viewMatrix) const
	{
		// Mesh positions and normals are evaluated in view space, so the light
		// must be transformed to that same coordinate system every frame.
		const glm::vec4 viewSpacePosition = viewMatrix * lightPosition;

		// Gets the uniform attributes of the light in the shader
		GLint lightPositionLoc    = glGetUniformLocation(shaderID, "light.position"   );
		GLint lightColorLoc       = glGetUniformLocation(shaderID, "light.color"      );
		GLint ambientIntensityLoc = glGetUniformLocation(shaderID, "ambient_intensity");
		GLint diffuseIntensityLoc = glGetUniformLocation(shaderID, "diffuse_intensity");
		GLint specularIntensityLoc = glGetUniformLocation(shaderID, "specular_intensity");
		GLint attenuationConstantLoc = glGetUniformLocation(shaderID, "attenuation_constant");
		GLint attenuationLinearLoc = glGetUniformLocation(shaderID, "attenuation_linear");
		GLint attenuationQuadraticLoc = glGetUniformLocation(shaderID, "attenuation_quadratic");

		// Sets the uniform values in the shader program.
		glUniform4f(lightPositionLoc   , viewSpacePosition.x, viewSpacePosition.y, viewSpacePosition.z, viewSpacePosition.w);
		glUniform3f(lightColorLoc      , lightColor.x   , lightColor.y   , lightColor.z);
		glUniform1f(ambientIntensityLoc, ambientIntensity);
		glUniform1f(diffuseIntensityLoc, diffuseIntensity);
		glUniform1f(specularIntensityLoc, specularIntensity);
		glUniform1f(attenuationConstantLoc, attenuationConstant);
		glUniform1f(attenuationLinearLoc, attenuationLinear);
		glUniform1f(attenuationQuadraticLoc, attenuationQuadratic);
	}

}
