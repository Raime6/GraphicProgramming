
/*
	Public Domain Code

	Author: Xavier Canals
	Author: Ángel Rodríguez
*/

#pragma once



#ifndef SCENE_HEADER
#define SCENE_HEADER



#include "Camera.hpp"
#include "MeshLoader.hpp"
#include "Postprocess.hpp"
#include "Skybox.hpp"
#include "Terrain.hpp"



namespace finalPractice
{
	/// <summary>
	/// The Scene class manages and renders all the objects in the 3D scene, including meshes, terrain,
	/// skybox, and post-processing effects. It also handles user input for camera movement and scene interaction.
	/// </summary>
	class Scene
	{
	private:

		Camera           camera;								///< The camera used for the scene's view.

		MeshLoader        table;								///< Mesh loader for the table model.
		MeshLoader    beerMug01;								///< Mesh loader for the beer mug model.
		MeshLoader    beerMug02;								///< Mesh loader for the beer mug model.
		MeshLoader    beerMug03;								///< Mesh loader for the beer mug model.
		MeshLoader      chair01;								///< Mesh loader for the chair model.
		MeshLoader      chair02;								///< Mesh loader for the chair model.
		MeshLoader     fishBowl;								///< Mesh loader for the fishbowl model.
		MeshLoader      crystal;								///< Mesh loader for the crystal model.

		Skybox           skybox;								///< The skybox for the scene.
		Terrain         terrain;								///< The terrain for the scene.

		Postprocess postprocess;								///< Post-processing effects for the scene.

		int               width;								///< Width of the scene's window.
		int              height;								///< Height of the scene's window.

		float      angleAroundX;								///< Rotation angle around the X-axis.
		float      angleAroundY;								///< Rotation angle around the Y-axis.
		float       angleDeltaX;								///< Delta for X-axis rotation during interaction.
		float       angleDeltaY;								///< Delta for Y-axis rotation during interaction.

		bool     pointerPressed;								///< Flag to track if the mouse button is pressed.
		int        lastPointerX;								///< Last known mouse X position.
		int        lastPointerY;								///< Last known mouse Y position.

	public:
		bool keys[4] = { false, false, false, false };			///< Array to track pressed keys (W, S, A, D).

	public:

		/// <summary>
		/// Initializes a new scene with the specified window width and height.
		/// </summary>
		/// 
		/// <param name="width">Width of the window.</param>
		/// <param name="height">Height of the window.</param>
		Scene(int width, int height);

		/// <summary>
		/// Updates the scene (handles camera movement and object updates).
		/// </summary>
		void update();

		/// <summary>
		/// Renders the scene's objects (models, terrain, skybox, etc.).
		/// </summary>
		void render();

	public:

		/// <summary>
		/// Resizes the scene to the given width and height.
		/// </summary>
		/// 
		/// <param name="width">New width of the window.</param>
		/// <param name="height">New height of the window.</param>
		void resize(int width, int height);

		/// <summary>
		/// Handles mouse dragging (camera rotation).
		/// </summary>
		/// 
		/// <param name="pointerX">Current X position of the mouse pointer.</param>
		/// <param name="pointerY">Current Y position of the mouse pointer.</param>
		void onDrag(int pointerX, int pointerY);

		/// <summary>
		/// Handles mouse click events (sets the pointerPressed flag).
		/// </summary>
		/// 
		/// <param name="pointerX">X position of the mouse pointer when clicked.</param>
		/// <param name="pointerY">Y position of the mouse pointer when clicked.</param>
		/// <param name="down">True if the mouse button is pressed, false if released.</param>
		void onClick(int pointerX, int pointerY, bool down);
	};
}



#endif
