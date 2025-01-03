
// Public Domain Code
// Author: Xavier Canals

#include "Scene.hpp"



namespace finalPractice
{
	Scene::Scene(int width, int height) : skybox("../../../shared/assets/skybox ")
	{
		glEnable (GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);

		resize(width, height);

		angleAroundX   = angleDeltaX = 0.f;
		angleAroundY   = angleDeltaY = 0.f;
		pointerPressed = false;
	}



	void Scene::update()
	{
		angleAroundX += angleDeltaX;
		angleAroundY += angleDeltaY;

		if (angleAroundX < -1.5f)
			angleAroundX = -1.5f;			
		else if (angleAroundX > +1.5f)
			angleAroundX = +1.5f;

		glm::mat4 cameraRotation(1);

		cameraRotation = glm::rotate(cameraRotation, angleAroundY, glm::vec3(0.f, 1.f, 0.f));
		cameraRotation = glm::rotate(cameraRotation, angleAroundX, glm::vec3(1.f, 0.f, 0.f));

		camera.setTarget(0, 0, -1);
		camera.rotate   (cameraRotation);
	}

	void Scene::render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		skybox.render(camera);
	}



	void Scene::resize(int newWidth, int newHeight)
	{
		width  =  newWidth;
		height = newHeight;

		camera.setRatio(float(width) / float(height));

		glViewport(0, 0, width, height);
	}

	void Scene::onDrag(int pointerX, int pointerY)
	{
		if (pointerPressed)
		{
			angleDeltaX = 0.025f * float(lastPointerY - pointerY) / float(height);
			angleDeltaY = 0.025f * float(lastPointerX - pointerX) / float( width);
		}
	}

	void Scene::onClick(int pointerX, int pointerY, bool down)
	{
		if ((pointerPressed = down) == true)
		{
			lastPointerX = pointerX;
			lastPointerY = pointerY;
		}
		else
			angleDeltaX = angleDeltaY = 0.f;
	}
}
