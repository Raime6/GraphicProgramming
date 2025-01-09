
// Public Domain Code
// Author: Xavier Canals

#pragma once



#ifndef SHADER_HEADER
#define SHADER_HEADER



#include <glad/glad.h>
#include <string>



namespace meshCreation
{
	class Shader
	{
		private:
		
			GLint shaderID;

		public:

			Shader(const std::string & vertexShaderCode, const std::string & fragmentShaderCode);
		   ~Shader();

		   void  Use();
		   GLint getID();
	
		private:

			GLuint compileShaders(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
			void   showCompilationError(GLuint  shaderID);
			void   showLinkageError(GLuint programID);
	};
}



#endif
