
/*
	Public Domain Code

	Author: Xavier Canals
*/

#include "Window.hpp"



#include <cassert>
#include <glad/glad.h>
#include <SDL_opengl.h>



namespace finalPractice
{
	Window::Window
	(
		const char*                            title,
		int					                   leftX,
		int					                    topY,
		unsigned				               width,
		unsigned				              height,
		const OpenGL_Context_Settings contextDetails
	)
	{
		// Initialize SDL video subsystem
		if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
			throw "Error while initializing SDL.";



		// Set OpenGL context attributes based on the provided settings
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, contextDetails.versionMajor);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, contextDetails.versionMinor);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		
		// Set OpenGL profile (core or compatibility)
		SDL_GL_SetAttribute
		(
			SDL_GL_CONTEXT_PROFILE_MASK, contextDetails.coreProfile ? SDL_GL_CONTEXT_PROFILE_CORE : SDL_GL_CONTEXT_PROFILE_COMPATIBILITY
		);

		// Set depth and stencil buffer sizes
		if (contextDetails.depthBufferSize)
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, contextDetails.depthBufferSize);
		if (contextDetails.stenzilBufferSuze)
			SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, contextDetails.stenzilBufferSuze);



		// Create the SDL window with OpenGL context
		windowHandle = SDL_CreateWindow(title, leftX, topY, (int)width, (int)height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		assert(windowHandle != nullptr);



		// Create the OpenGL context
		openGLContext = SDL_GL_CreateContext(windowHandle);
		assert(openGLContext != nullptr);



		// Initialize OpenGL using GLAD
		GLenum gladIsEnabled = gladLoadGL();
		assert(gladIsEnabled);



		// Set vertical synchronization (vsync) based on context settings
		SDL_GL_SetSwapInterval(contextDetails.enableVsync ? 1 : 0);
	}



	Window::~Window()
	{
		if (openGLContext)
			SDL_GL_DeleteContext(openGLContext);

		if (windowHandle)
			SDL_DestroyWindow(windowHandle);

		SDL_QuitSubSystem(SDL_INIT_VIDEO);
	}



	void Window::swapBuffers()
	{
		SDL_GL_SwapWindow(windowHandle);
	}
}
