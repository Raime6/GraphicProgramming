
/*
	Public Domain Code

	Author: Xavier Canals
*/

#pragma once



#include "Scene.hpp"
#include "Window.hpp"



using finalPractice::Scene;
using finalPractice::Window;



int main(int, char* [])
{
	constexpr unsigned   viewportWidth = 1024; ///< Viewport width.
	constexpr unsigned  viewportHeight =  576; ///< Viewport height.



	/// <summary>
	/// Creates a SDL window with the specified dimensions.
	/// </summary>
	Window window
	(
		"Final Practice"      ,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		viewportWidth         ,
		viewportHeight        ,
		{ 3, 3 }
	);

	/// <summary>
	/// Creates the scene which will manage all 3D elements.
	/// </summary>
	Scene scene(viewportWidth, viewportHeight);



	// Camera management variables
	int  mouseX     =     0;				  ///< Mouse's X position.
	int  mouseY     =     0;				  ///< Mouse's Y position.
	bool buttonDown = false;				  ///< Indicates if Mouse's left button is pressed
	bool exit       = false;				  ///< Indicates if the program needs to be closed

	// Main program's loop
	do
	{
		SDL_Event event;

		while (SDL_PollEvent(&event) > 0)
		{
			switch (event.type)
			{
				case SDL_MOUSEBUTTONDOWN:									  // If Mouse's left button is pressed
				{
					auto buttons  = SDL_GetMouseState(&mouseX, &mouseY);
					auto leftDown = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);

					// If this action has not been already processed, it processes it
					if (leftDown && not buttonDown)
						scene.onClick(mouseX, mouseY, buttonDown = true);

					break;
				}

				case SDL_MOUSEBUTTONUP:										  // If Mouse's left button is released
				{
					auto buttons  = SDL_GetMouseState(&mouseX, &mouseY);
					auto leftDown = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);

					// If Mouse's left button was pressed, it processes it
					if (not leftDown && buttonDown)
						scene.onClick(mouseX, mouseY, buttonDown = false);

					break;
				}

				case SDL_MOUSEMOTION:										  // If Mouseir moving, updates position and drag
				{
					SDL_GetMouseState(&mouseX, &mouseY);

					scene.onDrag(mouseX, mouseY);

					break;
				}

				case SDL_KEYDOWN:											  // If a Keyboard key is pressed
				{
					if (event.key.keysym.sym == SDLK_w) scene.keys[0] = true;
					if (event.key.keysym.sym == SDLK_s) scene.keys[1] = true;
					if (event.key.keysym.sym == SDLK_a) scene.keys[2] = true;
					if (event.key.keysym.sym == SDLK_d) scene.keys[3] = true;
					break;
				}

				case SDL_KEYUP:												  // If a Keyboard key is released
				{
					if (event.key.keysym.sym == SDLK_w) scene.keys[0] = false;
					if (event.key.keysym.sym == SDLK_s) scene.keys[1] = false;
					if (event.key.keysym.sym == SDLK_a) scene.keys[2] = false;
					if (event.key.keysym.sym == SDLK_d) scene.keys[3] = false;
					break;
				}

				case SDL_QUIT:												  // If User closes the window, ends main loop
				{
					exit = true;
					break;
				}
			}
		}

		/// <summary>
		/// Update the scene (including any animations or interactions).
		/// </summary>
		scene.update();

		/// <summary>
		/// Render the scene.
		/// </summary>
		scene.render();

		/// <summary>
		/// Swap the buffers (display the updated frame).
		/// </summary>
		window.swapBuffers();
	} while (not exit);



	/// <summary>
	/// Clean up and close the SDL library.
	/// </summary>
	SDL_Quit();

	return 0;
}
