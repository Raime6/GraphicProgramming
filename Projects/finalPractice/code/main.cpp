
// Public Domain Code
// Author: Xavier Canals

#pragma once



#include "Scene.hpp"
#include "Window.hpp"



using finalPractice::Scene;
using finalPractice::Window;



int main(int, char* [])
{
	constexpr unsigned  viewportWidth  = 1024;
	constexpr unsigned  viewportHeight =  576;



	Window window
	(
		"Final Practice"      ,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		viewportWidth         ,
		viewportHeight        ,
		{ 3, 3 }
	);

	Scene scene(viewportWidth, viewportHeight);



	int  mouseX     = 0;
	int  mouseY     = 0;
	bool buttonDown = false;
	bool exit       = false;

	do
	{
		SDL_Event event;

		while (SDL_PollEvent(&event) > 0)
		{
			switch (event.type)
			{
				case SDL_MOUSEBUTTONDOWN:
				{
					auto buttons  = SDL_GetMouseState(&mouseX, &mouseY);
					auto leftDown = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);

					if (leftDown && not buttonDown)
						scene.onClick(mouseX, mouseY, buttonDown = true);

					break;
				}

				case SDL_MOUSEBUTTONUP:
				{
					auto buttons  = SDL_GetMouseState(&mouseX, &mouseY);
					auto leftDown = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);

					if (not leftDown && buttonDown)
						scene.onClick(mouseX, mouseY, buttonDown = false);

					break;
				}

				case SDL_MOUSEMOTION:
				{
					SDL_GetMouseState(&mouseX, &mouseY);

					scene.onDrag(mouseX, mouseY);

					break;
				}

				case SDL_KEYDOWN:
				{
					if (event.key.keysym.sym == SDLK_w) scene.keys[0] = true;
					if (event.key.keysym.sym == SDLK_s) scene.keys[1] = true;
					if (event.key.keysym.sym == SDLK_a) scene.keys[2] = true;
					if (event.key.keysym.sym == SDLK_d) scene.keys[3] = true;
					break;
				}

				case SDL_KEYUP:
				{
					if (event.key.keysym.sym == SDLK_w) scene.keys[0] = false;
					if (event.key.keysym.sym == SDLK_s) scene.keys[1] = false;
					if (event.key.keysym.sym == SDLK_a) scene.keys[2] = false;
					if (event.key.keysym.sym == SDLK_d) scene.keys[3] = false;
					break;
				}

				case SDL_QUIT:
				{
					exit = true;
					break;
				}
			}
		}

		scene.update();

		scene.render();

		window.swapBuffers();
	} while (not exit);



	SDL_Quit();

	return 0;
}
