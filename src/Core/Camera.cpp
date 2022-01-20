#include "Camera.hpp"

Camera::Camera()
{

}

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up):
	position(pos), frontVector(front), upVector(up)
{
}

Camera::~Camera()
{

}
