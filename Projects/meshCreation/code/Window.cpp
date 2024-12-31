
// Public Domain Code
// Author: Xavier Canals

#include <cassert>
#include <glad/glad.h>
#include <SDL_opengl.h>



#include "Window.hpp"



namespace meshCreation
{
	Window::Window
	(
		const char					  * title,
			  int					    leftX,
			  int					    topY,
			  unsigned				    width,
			  unsigned				    height,
		const OpenGL_Context_Settings   contextDetails
	)
	{
		if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
			throw "Error while initializing SDL.";



		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, contextDetails.versionMajor);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, contextDetails.versionMinor);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER         , 1);
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL   , 1);
		SDL_GL_SetAttribute
		(
			SDL_GL_CONTEXT_PROFILE_MASK, contextDetails.coreProfile ? SDL_GL_CONTEXT_PROFILE_CORE : SDL_GL_CONTEXT_PROFILE_COMPATIBILITY
		);

		if (contextDetails.depthBufferSize)
			SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE  , contextDetails.depthBufferSize);
		if (contextDetails.stenzilBufferSuze)
			SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, contextDetails.stenzilBufferSuze);



		windowHandle = SDL_CreateWindow(title, leftX, topY, (int)width, (int)height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
		assert(windowHandle != nullptr);



		openGLContext = SDL_GL_CreateContext(windowHandle);
		assert(openGLContext != nullptr);



		GLenum gladIsEnabled = gladLoadGL();
		assert(gladIsEnabled);



		SDL_GL_SetSwapInterval(contextDetails.enableVsync ? 1: 0);
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
