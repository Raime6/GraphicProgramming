
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
			Point              target;

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

			float		  getFov	 () const { return      fov; }
			float		  getNearZ	 () const { return    nearZ; }
			float		  getFarZ	 () const { return     farZ; }
			float		  getRatio	 () const { return    ratio; }

			const Point & getLocation() const { return location; }
			const Point & getTarget  () const { return   target; }

		public:

			void setFov  (float   newFov) { fov   =   newFov; calculateProjectionMatrix(); }
			void setNearZ(float newNearZ) { nearZ = newNearZ; calculateProjectionMatrix(); }
			void setFarZ (float  newFarZ) { farZ  =  newFarZ; calculateProjectionMatrix(); }
			void setRatio(float newRatio) { ratio = newRatio; calculateProjectionMatrix(); }

			void setLocation(float x, float y, float z) { location[0] = x; location[1] = y; location[2] = z; }
			void setTarget  (float x, float y, float z) {   target[0] = x;   target[1] = y;   target[2] = z; }

			void reset(float newFov, float newNearZ, float newFarZ, float newRatio)
			{
				setFov     (        newFov);
				setNearZ   (      newNearZ);
				setFarZ    (       newFarZ);
				setRatio   (      newRatio);
				setLocation(0.f, 0.f,  0.f);
				setTarget  (0.f, 0.f, -1.f);
				calculateProjectionMatrix();
			}

		public:

			void move(const glm::vec3 & translation)
			{
				location += glm::vec4(translation, 1.f);
				target   += glm::vec4(translation, 1.f);
			}

			void rotate(const glm::mat4 & rotation)
			{
				target = location + rotation * (target - location);
			}

		public:

			const glm::mat4 & getProjectionMatrix() const
			{
				return projectionMatrix;
			}

			glm::mat4 getTransformMatrixInverse() const
			{
				return glm::lookAt
				(
					glm::vec3(location[0], location[1], location[2]),
					glm::vec3(  target[0],   target[1],   target[2]),
					glm::vec3(        0.f,         1.f,         0.f)
				);
			}

		private:

			void calculateProjectionMatrix()
			{
				projectionMatrix = glm::perspective(glm::radians(fov), ratio, nearZ, farZ);
			}
	};
}



#endif
