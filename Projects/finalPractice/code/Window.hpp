
/*
	Public Domain Code

	Author: Xavier Canals
	Author: Ángel Rodríguez
*/

#pragma once



#ifndef WINDOW_HEADER
#define WINDOW_HEADER



#include <SDL.h>
#include <string>



namespace finalPractice
{
	/// <summary>
	/// The Window class represents an SDL window with an OpenGL context.
	/// It provides methods to create, manage, and interact with the window and OpenGL context.
	/// </summary>
	class Window
	{
	public:
		
		/// <summary>
		/// Struct to define the settings for the OpenGL context.
		/// </summary>
		struct OpenGL_Context_Settings
		{
			unsigned versionMajor      = 3;					///< The major version of OpenGL.
			unsigned versionMinor      = 3;					///< The minor version of OpenGL.
			bool     coreProfile       = true;				///< Whether to use the core profile for OpenGL (true) or the compatibility profile (false).
			unsigned depthBufferSize   = 24;				///< The size of the depth buffer in bits.
			unsigned stenzilBufferSuze = 0;					///< The size of the stencil buffer in bits.
			bool     enableVsync       = true;				///< Whether to enable vertical synchronization (vsync).
		};

	private:

		SDL_Window*    windowHandle;						///< The handle to the SDL window.
		SDL_GLContext openGLContext;						///< The handle to the OpenGL context.

	public:

		/// <summary>
		/// Constructs a new Window with a given title, position, size, and OpenGL context settings.
		/// </summary>
		/// 
		/// <param name="title">The title of the window.</param>
		/// <param name="leftX">The x position of the window on the screen.</param>
		/// <param name="topY">The y position of the window on the screen.</param>
		/// <param name="width">The width of the window.</param>
		/// <param name="height">The height of the window.</param>
		/// <param name="contextDetails">The OpenGL context settings to be applied.</param>
		Window
		(
			const char*                            title,
			int					                   leftX,
			int					                    topY,
			unsigned				               width,
			unsigned				              height,
			const OpenGL_Context_Settings contextDetails
		);

		/// <summary>
		/// Constructs a new Window using a string title for convenience.
		/// </summary>
		///
		/// <param name="title">The title of the window.</param>
		/// <param name="leftX">The x position of the window on the screen.</param>
		/// <param name="topY">The y position of the window on the screen.</param>
		/// <param name="width">The width of the window.</param>
		/// <param name="height">The height of the window.</param>
		/// <param name="contextDetails">The OpenGL context settings to be applied.</param>
		Window
		(
			const std::string&                     title,
			int						               leftX,
			int						                topY,
			unsigned					           width,
			unsigned					          height,
			const OpenGL_Context_Settings contextDetails
		) : Window(title.c_str(), leftX, topY, width, height, contextDetails) { }


		/// <summary>
		/// Destructor that cleans up the SDL window and OpenGL context.
		/// </summary>
		~Window();

	public:

		// Prevent copy operations
		Window(const Window&) = delete;
		Window& operator = (const Window&) = delete;

		// Allow move operations
		Window(Window&& other) noexcept
		{
			this->windowHandle = std::exchange(other.windowHandle, nullptr);
			this->openGLContext = std::exchange(other.openGLContext, nullptr);
		}

		Window& operator = (Window&& other) noexcept
		{
			this->windowHandle = std::exchange(other.windowHandle, nullptr);
			this->openGLContext = std::exchange(other.openGLContext, nullptr);
		}

	public:

		/// <summary>
		/// Swaps the window buffers, displaying the rendered content to the screen.
		/// </summary>
		void swapBuffers();
	};
}



#endif
