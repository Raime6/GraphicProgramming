
// Public Domain Code
// Author: Xavier Canals

#pragma once



#ifndef SCENE_HEADER
#define SCENE_HEADER



#include "Camera.hpp"
#include "Skybox.hpp"



namespace finalPractice
{
	class Scene
	{
		private:

			Camera		 camera;
			Skybox		 skybox;

			int		   	  width;
			int          height;

			float  angleAroundX;
			float  angleAroundY;
			float   angleDeltaX;
			float   angleDeltaY;

			bool pointerPressed;
			int	   lastPointerX;
			int    lastPointerY;

		public:

			Scene(int width, int height);

			void update();
			void render();

		public:

			void resize (int width, int height);
			void onDrag (int pointerX, int pointerY);
			void onClick(int pointerX, int pointerY, bool down);
	};
}



#endif
