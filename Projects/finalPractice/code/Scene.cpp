
// Public Domain Code
// Author: Xavier Canals

#include "Scene.hpp"



namespace finalPractice
{
	Scene::Scene(int width, int height) :
		table("../../../assets/table.fbx", "../../../assets/table_textureAlbedo.png"),
		beerMug("../../../assets/beerMug.fbx" , "../../../assets/beerMug_textureAlbedo.png" ),
		terrain(10.f, 10.f, 50, 50, "../../../assets/height_map.png"),
		skybox ("../../../assets/skybox_")
	{
		glEnable (GL_CULL_FACE);
		glEnable (GL_DEPTH_TEST);

		resize(width, height);

		pointerPressed = false;
	}



	void Scene::update()
	{
		glm::vec3 cameraDirection(
			cos(camera.getRotationX()) * sin(camera.getRotationY()),
			sin(camera.getRotationX()),
			cos(camera.getRotationX()) * cos(camera.getRotationY())
		);
		
		glm::vec3 right = glm::normalize(glm::cross(cameraDirection, glm::vec3(0.f, 1.f, 0.f)));
		glm::vec3 up    = glm::normalize(glm::cross(right, cameraDirection));

		glm::vec3 movement(0.f);

		if (keys[0]) movement += cameraDirection * 0.005f;
		if (keys[1]) movement -= cameraDirection * 0.005f;
		if (keys[2]) movement -= right * 0.005f;
		if (keys[3]) movement += right * 0.005f;

		camera.move(movement);

		terrain.update();
	}

	void Scene::render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		table  .render(camera, glm::vec3(0.f, -2.f, 0.f),  0.f  , glm::vec3(1, 1, 1), glm::vec3(0.5f, 0.5f, 0.5f));
		beerMug.render(camera, glm::vec3(0.f, 0.f, -1.f), -1.57f, glm::vec3(1, 0, 0), glm::vec3(1.f, 1.f, 1.f   ));

		terrain.render(camera);
		skybox .render(camera);
	}



	void Scene::resize(int newWidth, int newHeight)
	{
		width  =  newWidth;
		height = newHeight;

		camera.setRatio(float(width) / float(height));

		table  .resize(newWidth, newHeight);
		beerMug.resize(newWidth, newHeight);

		terrain.resize(newWidth, newHeight);

		glViewport(0, 0, width, height);
	}

	void Scene::onDrag(int pointerX, int pointerY)
	{
		if (pointerPressed)
		{
			float deltaX = (pointerX - lastPointerX) * 0.005f;
			float deltaY = (pointerY - lastPointerY) * 0.005f;

			camera.rotate(-deltaY, -deltaX);

			lastPointerX = pointerX;
			lastPointerY = pointerY;
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
