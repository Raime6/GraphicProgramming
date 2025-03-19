
/* 
	Public Domain Code
	
	Author: Xavier Canals
*/

#pragma once



#ifndef CAMERA_HEADER
#define CAMERA_HEADER



#include <glm.hpp>
#include <gtc/matrix_transform.hpp>



namespace finalPractice
{
	/// <summary>
	/// The Camera class is responsible for handling the camera's position, rotation, and projection in a 3D scene.
	/// It provides methods to adjust the camera's field of view (FOV), aspect ratio, and clipping planes (near and far planes).
	/// Additionally, it supports camera movement and rotation as well as the calculation of projection and view transformation matrices.
	/// </summary>
	class Camera
	{
		using Point    = glm::vec4;		 ///< Type alias for a point (4D vector) used for camera location.
		using Vector   = glm::vec4;		 ///< Type alias for a vector (4D vector).
		using Matrix44 = glm::mat4;		 ///< Type alias for a 4x4 matrix (used for transformations).

		private:

			float				    fov; ///< Field of view in degrees.
			float				  nearZ; ///< Near clipping plane distance.
			float				   farZ; ///< Far clipping plane distance.
			float				  ratio; ///< Aspect ratio (width/height).
			
			Point			   location; ///< Camera's location in 3D space (x, y, z).
			float			  rotationY; ///< Rotation angle around the Y-axis (vertical).
			float			  rotationX; ///< Rotation angle around the X-axis (horizontal).
			
			Matrix44   projectionMatrix; ///< The projection matrix used for rendering.

		public:

			/// <summary>
			/// Default constructor that initializes the camera with a ratio of 1 and a field of view of 60 degrees.
			/// </summary>
			/// 
			/// <param name="ratio">The aspect ratio (width/height) of the camera view. Default is 1.</param>
			Camera(float ratio = 1.f)
			{
				reset(60.f, 0.1f, 1000.f, ratio);
			}

			/// <summary>
			/// Constructor with custom near and far clipping planes, and a default field of view of 60 degrees.
			/// </summary>
			/// 
			/// <param name="nearZ">The near clipping plane distance.</param>
			/// <param name="farZ">The far clipping plane distance.</param>
			/// <param name="ratio">The aspect ratio (width/height) of the camera view. Default is 1.</param>
			Camera(float nearZ, float farZ, float ratio = 1.f)
			{
				reset(60.f, nearZ, farZ, ratio);
			}

			/// <summary>
			/// Constructor that allows for full customization of FOV, near and far planes, and the aspect ratio.
			/// </summary>
			/// 
			/// <param name="fovDegrees">The field of view in degrees.</param>
			/// <param name="nearZ">The near clipping plane distance.</param>
			/// <param name="farZ">The far clipping plane distance.</param>
			/// <param name="ratio">The aspect ratio (width/height) of the camera view.</param>
			Camera(float fovDegrees, float nearZ, float farZ, float ratio)
			{
				reset(fovDegrees, nearZ, farZ, ratio);
			}

		public:

			/// <summary>
			/// Getter methods used to get the camera's field of view (FOV), near clipping plane, far clipping plane,
			/// aspect ratio and rotation angles for both the X and Y axes, as well as its location in 3D space.
			/// </summary>
			/// 
			/// <return>Returns the respective values for FOV, nearZ, farZ, ratio, rotationX, rotationY, and location as a 4D vector (x, y, z, w).</return>
			float		  getFov	  () const { return          fov; }
			float		  getNearZ	  () const { return        nearZ; }
			float		  getFarZ	  () const { return         farZ; }
			float		  getRatio    () const { return        ratio; }
			float		  getRotationX() const { return    rotationX; }
			float		  getRotationY() const { return    rotationY; }

			const Point & getLocation() const { return location; }

		public:

			/// <summary>
			/// Setter methods used to Set the camera's field of view (FOV), near and far clipping planes, aspect ratio,
			/// rotation angles for both X and Y axes, recalculating the projection matrix as needed.
			/// </summary>
			/// 
			/// <param name="newFov">The new field of view in degrees.</param>
			/// <param name="newNearZ">The new near clipping plane distance.</param>
			/// <param name="newFarZ">The new far clipping plane distance.</param>
			/// <param name="newRatio">The new aspect ratio (width/height).</param>
			/// <param name="newRotationX">Rotation angle around the Y-axis (vertical).</param>
			/// <param name="newRotationY">Rotation angle around the X-axis (horizontal).</param>
			void setFov      (float       newFov) { fov       =       newFov; calculateProjectionMatrix(); }
			void setNearZ    (float     newNearZ) { nearZ     =     newNearZ; calculateProjectionMatrix(); }
			void setFarZ     (float      newFarZ) { farZ      =      newFarZ; calculateProjectionMatrix(); }
			void setRatio    (float     newRatio) { ratio     =     newRatio; calculateProjectionMatrix(); }
			void setRotationX(float newRotationX) { rotationX = newRotationX; calculateProjectionMatrix(); }
			void setRotationY(float newRotationY) { rotationY = newRotationY; calculateProjectionMatrix(); }

			/// <summary>
			/// Sets a new 3 location for the camera
			/// </summary>
			/// 
			/// <param name="x">X coord position.</param>
			/// <param name="y">Y coord position.</param>
			/// <param name="z">Z coord position.</param>
			void setLocation(float x, float y, float z) { location[0] = x; location[1] = y; location[2] = z; }

			/// <summary>
			/// Resets the camera's parameters to the provided values and recalculates the projection matrix.
			/// </summary>
			/// 
			/// <param name="newFov">The new field of view in degrees.</param>
			/// <param name="newNearZ">The new near clipping plane distance.</param>
			/// <param name="newFarZ">The new far clipping plane distance.</param>
			/// <param name="newRatio">The new aspect ratio (width/height).</param>
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

			/// <summary>
			/// Moves the camera by the specified translation vector.
			/// </summary>
			/// 
			/// <param name="translation">The translation vector (x, y, z) to move the camera.</param>
			void move(const glm::vec3 & translation)
			{
				location += glm::vec4(translation, 1.f);
			}

			/// <summary>
			/// Rotates the camera based on the specified rotation deltas for the X and Y axes.
			/// </summary>
			/// 
			/// <param name="deltaRotationX">The change in rotation around the X-axis in radians.</param>
			/// <param name="deltaRotationY">The change in rotation around the Y-axis in radians.</param>
			void rotate(float deltaRotationX, float deltaRotationY)
			{
				rotationY += deltaRotationY;
				rotationX += deltaRotationX;

				if (rotationX > glm::radians( 89.f)) rotationX = glm::radians( 89.f);
				if (rotationX < glm::radians(-89.f)) rotationX = glm::radians(-89.f);
			}

		public:

			/// <summary>
			/// Returns the current projection matrix used by the camera.
			/// </summary>
			/// 
			/// <return>Returns the camera's projection matrix (4x4 matrix).</return>
			const glm::mat4 & getProjectionMatrix() const
			{
				return projectionMatrix;
			}

			/// <summary>
			/// Returns the inverse of the camera's transform matrix, used for view transformations.
			/// </summary>
			/// 
			/// <return>Returns the inverse of the camera's transform matrix.</return>
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

			/// <summary>
			/// Calculates the projection matrix based on the camera's FOV, aspect ratio, and clipping planes.
			/// </summary>
			void calculateProjectionMatrix()
			{
				projectionMatrix = glm::perspective(glm::radians(fov), ratio, nearZ, farZ);
			}
	};
}



#endif
