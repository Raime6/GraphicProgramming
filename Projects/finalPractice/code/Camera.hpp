
// Public Domain Code
// Author: Xavier Canals

#pragma once



#ifndef CAMERA_HEADER
#define CAMERA_HEADER



#include <glm.hpp>
#include <gtc/matrix_transform.hpp>



namespace finalPractice
{
	class Camera
	{
			using Point    = glm::vec4;
			using Vector   = glm::vec4;
			using Matrix44 = glm::mat4;

		private:

			float				  fov;
			float				nearZ;
			float				 farZ;
			float				ratio;

			Point            location;
			float           rotationY;
			float           rotationX;

			Matrix44 projectionMatrix;

		public:

			Camera(float ratio = 1.f)
			{
				reset(60.f, 0.1f, 1000.f, ratio);
			}

			Camera(float nearZ, float farZ, float ratio = 1.f)
			{
				reset(60.f, nearZ, farZ, ratio);
			}

			Camera(float fovDegrees, float nearZ, float farZ, float ratio)
			{
				reset(fovDegrees, nearZ, farZ, ratio);
			}

		public:

			float		  getFov	  () const { return          fov; }
			float		  getNearZ	  () const { return        nearZ; }
			float		  getFarZ	  () const { return         farZ; }
			float		  getRatio    () const { return        ratio; }
			float		  getRotationX() const { return    rotationX; }
			float		  getRotationY() const { return    rotationY; }

			const Point & getLocation() const { return location; }

		public:

			void setFov      (float       newFov) { fov       =       newFov; calculateProjectionMatrix(); }
			void setNearZ    (float     newNearZ) { nearZ     =     newNearZ; calculateProjectionMatrix(); }
			void setFarZ     (float      newFarZ) { farZ      =      newFarZ; calculateProjectionMatrix(); }
			void setRatio    (float     newRatio) { ratio     =     newRatio; calculateProjectionMatrix(); }
			void setRotationX(float newRotationX) { rotationX = newRotationX; calculateProjectionMatrix(); }
			void setRotationY(float newRotationY) { rotationY = newRotationY; calculateProjectionMatrix(); }

			void setLocation(float x, float y, float z) { location[0] = x; location[1] = y; location[2] = z; }

			void reset(float newFov, float newNearZ, float newFarZ, float newRatio)
			{
				setFov      (         newFov);
				setNearZ    (       newNearZ);
				setFarZ     (        newFarZ);
				setRatio    (       newRatio);
				setLocation (0.f, 0.f, -2.5f);
				setRotationX(            0.f);
				setRotationY(            0.f);
				calculateProjectionMatrix  ();
			}

		public:

			void move(const glm::vec3 & translation)
			{
				location += glm::vec4(translation, 1.f);
			}

			void rotate(float deltaRotationX, float deltaRotationY)
			{
				rotationY += deltaRotationY;
				rotationX += deltaRotationX;

				if (rotationX > glm::radians( 89.f)) rotationX = glm::radians( 89.f);
				if (rotationX < glm::radians(-89.f)) rotationX = glm::radians(-89.f);
			}

		public:

			const glm::mat4 & getProjectionMatrix() const
			{
				return projectionMatrix;
			}

			glm::mat4 getTransformMatrixInverse() const
			{
				glm::vec3 direction
				(
					cos(rotationX) * sin(rotationY),
					sin(rotationX),
					cos(rotationX) * cos(rotationY)
				);
				
				glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.f, 1.f, 0.f), direction));
				glm::vec3 up    = glm::normalize(glm::cross(direction, right));
				
				return glm::lookAt(glm::vec3(location), glm::vec3(location) + direction, up);
			}

		private:

			void calculateProjectionMatrix()
			{
				projectionMatrix = glm::perspective(glm::radians(fov), ratio, nearZ, farZ);
			}
	};
}



#endif
