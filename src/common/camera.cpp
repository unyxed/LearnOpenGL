#include "camera.h"

Camera::Camera(
	glm::vec3 position,
	glm::vec3 up,
	float yaw, float pitch
): 
	Front(glm::vec3(0.0f, 0.0f, -1.0f)),
	MovementSpeed(SPEED),
	MovementSensitivity(SENSITIVITY),
	Zoom(ZOOM)
{
	Position = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;	
}

void Camera::ProcessKeyboard(CameraMovement direction, float delta_time) {
	float velocity = MovementSpeed * delta_time;		
	if (direction == CameraMovement::FORWARD)
		Position += Front * velocity;
	if (direction == CameraMovement::BACKWARD)
		Position -= Front * velocity;
	if (direction == CameraMovement::RIGHT)
		Position += Right * velocity;
	if (direction == CameraMovement::LEFT) 
		Position -= Right * velocity;
	if (direction == CameraMovement::UP)
		Position += Up * velocity;
	if (direction == CameraMovement::DOWN)
		Position -= Up * velocity;
}

void Camera::updateCameraVectors() {
	// calculate the new "Front" vector
	glm::vec3 front;

	// re-calculate the "Right" and "Up" vectors
	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up    = glm::normalize(glm::cross(Right, Front));
}