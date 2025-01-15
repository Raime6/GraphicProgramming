
/*
	Public Domain Code

	Author: Xavier Canals
	Author: Ángel Rodríguez
*/

#pragma once



#ifndef LIGHTING_HEADER
#define LIGHTING_HEADER



#include <glm.hpp>
#include <glad/glad.h>



namespace finalPractice
{
	/// <summary>
	/// The Lighting class represents a simple lighting model with ambient and diffuse lighting components.
	/// It manages the light position, color, and intensities for use in a shader program.
	/// </summary>
	class Lighting
	{
	private:

		glm::vec4	 lightPosition; ///< The position of the light in 3D space (x, y, z, w).
		glm::vec3		lightColor; ///< The color of the light (RGB components).
		
		float	  ambientIntensity; ///< The intensity of the ambient light (between 0 and 1).
		float	  diffuseIntensity; ///< The intensity of the diffuse light (between 0 and 1).

	public:
		
		/// <summary>
		/// Default constructor for the Lighting class. Initializes the light properties with default values.
		/// </summary>
		Lighting();

		/// <summary>
		/// Configures the lighting parameters in the given shader program by setting uniform values.
		/// </summary>
		/// 
		/// <param name="shaderID">The ID of the shader program to configure the light parameters for.</param>
		void configureLight(GLuint shaderID);
	};
}



#endif
