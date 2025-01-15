
/*
	Public Domain Code

	Author: Xavier Canals
	Author: Ángel Rodríguez
*/

#pragma once



#ifndef SHADER_HEADER
#define SHADER_HEADER



#include <glad/glad.h>
#include <string>



namespace finalPractice
{
	/// <summary>
	/// Shader class is responsible for managing the OpenGL shader programs, including compiling, linking,
	/// and using vertex and fragment shaders. It handles errors during shader compilation and linkage.
	/// </summary>
	class Shader
	{
	private:

		GLuint shaderID;										///< ID of the compiled shader program.

	public:

		/// <summary>
		/// Constructor for creating and compiling a shader program using the provided vertex and fragment shader codes.
		/// </summary>
		/// 
		/// <param name="vertexShaderCode">The source code for the vertex shader.</param>
		/// <param name="fragmentShaderCode">The source code for the fragment shader.</param>
		Shader(const std::string vertexShaderCode, const std::string fragmentShaderCode);

		/// <summary>
		/// Destructor that cleans up the OpenGL resources associated with the shader program.
		/// </summary>
		~Shader();



		/// <summary>
		/// Activates the shader program for rendering.
		/// </summary>
		void   use();

		/// <summary>
		/// Returns the OpenGL shader program ID.
		/// </summary>
		/// 
		/// <returns>The OpenGL shader program ID.</returns>
		GLuint getID();

	private:

		/// <summary>
		/// Compiles the vertex and fragment shaders from the provided source code and links them into a shader program.
		/// </summary>
		/// 
		/// <param name="vertexShaderCode">The source code for the vertex shader.</param>
		/// <param name="fragmentShaderCode">The source code for the fragment shader.</param>
		/// 
		/// <returns>The ID of the compiled shader program.</returns>
		GLuint compileShaders(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);

		/// <summary>
		/// Displays an error message if the shader compilation fails.
		/// </summary>
		/// 
		/// <param name="shaderID">The ID of the shader that failed to compile.</param>
		void   showCompilationError(GLuint  shaderID);

		/// <summary>
		/// Displays an error message if the shader program linking fails.
		/// </summary>
		/// <param name="programID">The ID of the shader program that failed to link.</param>
		void   showLinkageError(GLuint programID);
	};
}



#endif
