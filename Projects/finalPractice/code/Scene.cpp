
// Public Domain Code
// Author: Xavier Canals

#include "Scene.hpp"



namespace finalPractice
{
	Scene::Scene(int width, int height) :
		table  ("../../../assets/table.fbx"  , "../../../assets/table_textureAlbedo.png"),
		//capsule("../../../assets/capsule.fbx"),
		terrain(10.f, 10.f, 50, 50, "../../../assets/height_map.png"),
		skybox ("../../../assets/skybox_")
	{
		glEnable (GL_CULL_FACE);
		glEnable (GL_DEPTH_TEST);

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

		camera.setTarget(0, 0, 1);
		
		camera.rotate   (cameraRotation);

		glm::vec3 front = glm::normalize(camera.getTarget() - camera.getLocation());
		glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.f, 1.f, 0.f)));
		glm::vec3 up    = glm::normalize(glm::cross(right, front));

		glm::vec3 movement(0.f);

		if (keys[0]) movement += front * 0.005f;
		if (keys[1]) movement -= front * 0.005f;
		if (keys[2]) movement -= right * 0.005f;
		if (keys[3]) movement += right * 0.005f;

		camera.move(movement);

		terrain.update();
	}

	void Scene::render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		table  .render(camera, glm::vec3(0.f, -2.f, 0.f), glm::vec3(0.5f, 0.5f, 0.5f));
		//capsule.render(camera, glm::vec3(0.f, 0.f, -1.f));

		terrain.render(camera);
		skybox .render(camera);
	}



	void Scene::resize(int newWidth, int newHeight)
	{
		width  =  newWidth;
		height = newHeight;

		camera.setRatio(float(width) / float(height));

		table  .resize(newWidth, newHeight);
		//capsule.resize(newWidth, newHeight);
		terrain.resize(newWidth, newHeight);

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
		{
			angleDeltaX = angleDeltaY = 0.f;
		}
	}
}
