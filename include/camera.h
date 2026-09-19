#ifndef CAMERA_H
#define CAMERA_H

#include <glad/gl.h>
#include <glm/glm.hpp>

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

const float YAW           = -90.0f;
const float PITCH         = 0.0f;
const float SPEED         = 2.5f;
const float SENSITIVITY   = 0.1f;
const float ZOOM          = 45.0f;

class Camera {
	public:
		// camera attributes
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;	

		// euler angles
		float Yaw;
		float Pitch;

		// camera options
		float MovementSpeed;
		float MovementSensitivity;
		float Zoom;

		// constructor with vectors
		Camera(
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
			float yaw = YAW, float pitch = PITCH
		);

		// constructor with scalar values
		Camera(
			float posX, float posY, float posZ,
			float upX,  float upY,  float upZ,
			float yaw,  float pitch
		);


		void ProcessKeyboard(CameraMovement direction, float delta_time);

	private:
		void updateCameraVectors(); // calculates the front vector from the Camera's (updated) Euler Angles
};

#endif
