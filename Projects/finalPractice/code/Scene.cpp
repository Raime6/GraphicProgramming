
/*
	Public Domain Code

	Author: Xavier Canals
	Author: Ángel Rodríguez
*/

#include "Scene.hpp"



namespace finalPractice
{
	Scene::Scene(int width, int height) :
		table      ("../../binaries/assets/table.fbx"  , "../../binaries/assets/table_textureAlbedo.png",   1.f ),
		beerMug01  ("../../binaries/assets/beerMug.fbx", "../../binaries/assets/beerMug_textureAlbedo.png", 1.f),
		beerMug02  ("../../binaries/assets/beerMug.fbx", "../../binaries/assets/beerMug_textureAlbedo.png", 1.f),
		beerMug03  ("../../binaries/assets/beerMug.fbx", "../../binaries/assets/beerMug_textureAlbedo.png", 1.f),
		chair01    ("../../binaries/assets/chair.fbx", "../../binaries/assets/chair_textureAlbedo.png", 1.f),
		chair02    ("../../binaries/assets/chair.fbx"  , "../../binaries/assets/chair_textureAlbedo.png",   1.f ),
		fishBowl   ("../../binaries/assets/fishBowl.fbx", .5f),
		crystal    ("../../binaries/assets/crystal.fbx", "../../binaries/assets/crystal_textureAlbedo.png",  .8f),
		terrain    (10.f, 10.f, 50, 50, "../../binaries/assets/height_map.png"),
		skybox     ("../../binaries/assets/skybox_"),
		postprocess(width, height)
	{
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		resize(width, height);

		pointerPressed = false;
	}



	void Scene::update()
	{
		// Camera's behaviour update
		glm::vec3 cameraDirection(
			cos(camera.getRotationX()) * sin(camera.getRotationY()),
			sin(camera.getRotationX()),
			cos(camera.getRotationX()) * cos(camera.getRotationY())
		);

		glm::vec3 right = glm::normalize(glm::cross(cameraDirection, glm::vec3(0.f, 1.f, 0.f)));
		glm::vec3 up = glm::normalize(glm::cross(right, cameraDirection));

		glm::vec3 movement(0.f);

		if (keys[0]) movement += cameraDirection * 0.005f;
		if (keys[1]) movement -= cameraDirection * 0.005f;
		if (keys[2]) movement -= right * 0.005f;
		if (keys[3]) movement += right * 0.005f;

		camera.move(movement);

		// Animation update
		crystal.update();
		terrain.update();
	}

	void Scene::render()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Render the meshes
		table    .render(camera, glm::vec3( 0.f , -2.f  , 0.f) ,  0.f  , glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.5f, 0.5f, 0.5f));
		beerMug01.render(camera, glm::vec3(  .5f,  -.39f, 0.f) , -1.57f, glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.f , 1.f , 1.f ));
		beerMug02.render(camera, glm::vec3( -.4f,  -.39f,  .4f), -1.57f, glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.f , 1.f , 1.f ));
		beerMug03.render(camera, glm::vec3( -.3f,  -.33f, -.8f),  0.f  , glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.f , 1.f , 1.f ));
		chair01  .render(camera, glm::vec3(-1.f , -2.05f, 1.f) ,  2.5f , glm::vec3(0.f, 1.f, 0.f), glm::vec3(2.2f, 2.2f, 2.2f));
		chair02  .render(camera, glm::vec3( 1.f , -2.05f, 1.f) , -2.5f , glm::vec3(0.f, 1.f, 0.f), glm::vec3(2.2f, 2.2f, 2.2f));
		
		// Transparency meshes
		fishBowl.render(camera, glm::vec3(0.f, -.22f, 0.f), -1.57f, glm::vec3(1.f, 0.f, 0.f), glm::vec3(2.f, 2.f, 2.f));
		crystal .render(camera, glm::vec3(0.f, crystal.getPosY(), 0.f),  crystal.getAngle(), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.2f, 0.2f, 0.2f));

		// Render the rest of the scene's components
		terrain.render(camera);
		skybox .render(camera);

		// Render a postprocess (not working)
		//postprocess.renderFramebuffer();
	}



	void Scene::resize(int newWidth, int newHeight)
	{
		width  = newWidth;
		height = newHeight;

		camera.setRatio(float(width) / float(height));

		// Resize meshes
		table    .resize(newWidth, newHeight);
		beerMug01.resize(newWidth, newHeight);
		beerMug02.resize(newWidth, newHeight);
		beerMug03.resize(newWidth, newHeight);
		chair01  .resize(newWidth, newHeight);
		chair02  .resize(newWidth, newHeight);
		fishBowl .resize(newWidth, newHeight);
		crystal  .resize(newWidth, newHeight);

		// Resize the rest of the scene's components
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
