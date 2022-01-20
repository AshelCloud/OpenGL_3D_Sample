#pragma once

#include "glm/glm.hpp"

class Camera
{
public:
	Camera();
	Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up);
	virtual ~Camera();

public:
	void SetPosition(const glm::vec3& pos) { position = pos; }
	const glm::vec3& GetPosition() { return position; }

	void Translate(const glm::vec3& v) { position += v; }

	const glm::vec3& GetUpVector() { return upVector; }
	const glm::vec3& GetRightVector() { return rightVector; }
	const glm::vec3& GetFrontVector() { return frontVector; }
	void SetFrontVector(const glm::vec3& v) { frontVector = v; }

	void SetProjection(const glm::mat4& projection) { this->projection = projection; }
	const glm::mat4& GetProjection() { return projection; }

protected:
	glm::vec3 position;
	glm::vec3 upVector;
	glm::vec3 rightVector;
	glm::vec3 frontVector;

	glm::mat4 projection;
};