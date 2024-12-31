
// Public Domain Code
// Author: Xavier Canals

#pragma once



#include <SDL.h>
#include <string>



namespace meshCreation
{
	class Window
	{
	public:

		struct OpenGL_Context_Settings
		{
			unsigned versionMajor = 3;
			unsigned versionMinor = 3;
			bool	 coreProfile = true;
			unsigned depthBufferSize = 24;
			unsigned stenzilBufferSuze = 0;
			bool	 enableVsync = true;
		};

	private:

		SDL_Window* windowHandle;
		SDL_GLContext   openGLContext;

	public:

		Window
		(
			const char* title,
			int					    leftX,
			int					    topY,
			unsigned				    width,
			unsigned				    height,
			const OpenGL_Context_Settings   contextDetails
		);

		Window
		(
			const std::string& title,
			int						leftX,
			int						topY,
			unsigned					width,
			unsigned					height,
			const OpenGL_Context_Settings   contextDetails
		) : Window(title.c_str(), leftX, topY, width, height, contextDetails) { }



		~Window();

	public:

		Window(const Window&) = delete;

		Window& operator = (const Window&) = delete;

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

		void swapBuffers();
	};
}
