
/*
	Public Domain Code

	Author: Xavier Canals
*/

#pragma once



#ifndef LIGHTING_HEADER
#define LIGHTING_HEADER



#include <glm/glm.hpp>
#include <glad/glad.h>



namespace finalPractice
{
	/// <summary>
	/// The Lighting class stores the parameters of the point light used by the
	/// Blinn-Phong shaders.
	/// </summary>
	class Lighting
	{
	private:

		glm::vec4	 lightPosition; ///< The position of the light in 3D space (x, y, z, w).
		glm::vec3		lightColor; ///< The color of the light (RGB components).
		
		float	  ambientIntensity; ///< The intensity of the ambient light (between 0 and 1).
		float	  diffuseIntensity; ///< The intensity of the diffuse light (between 0 and 1).
		float	 specularIntensity; ///< The intensity of the specular light (between 0 and 1).

		float attenuationConstant; ///< Constant attenuation term.
		float   attenuationLinear; ///< Linear attenuation term.
		float attenuationQuadratic; ///< Quadratic attenuation term.

	public:
		
		/// <summary>
		/// Default constructor for the Lighting class. Initializes the light properties with default values.
		/// </summary>
		Lighting();

		/// <summary>
		/// Configures the lighting parameters in the given shader program by setting uniform values.
		/// </summary>
		/// 
		/// <param name="shaderID">The ID of the shader program to configure.</param>
		/// <param name="viewMatrix">View matrix used to place the light in view space.</param>
		void configureLight(GLuint shaderID, const glm::mat4& viewMatrix) const;
	};
}



#endif
