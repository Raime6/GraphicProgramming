
// Public Domain Code
// Author: Xavier Canals

#pragma once



#include "Scene.hpp"
#include "Window.hpp"



using meshCreation::Scene;
using meshCreation::Window;



int main(int, char* [])
{
	constexpr unsigned  viewportWidth = 1024;
	constexpr unsigned  viewportHeight = 576;



	Window window("Final Practice", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, viewportWidth, viewportHeight, { 3, 3 });

	Scene scene(viewportWidth, viewportHeight);



	bool exit = false;
	do
	{
		SDL_Event event;

		while (SDL_PollEvent(&event) > 0)
		{
			if (event.type == SDL_QUIT)
				exit = true;
		}

		scene.update();

		scene.render();

		window.swapBuffers();
	} while (not exit);



	SDL_Quit();

	return 0;
}
