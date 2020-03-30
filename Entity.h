#pragma once

#include <glm/glm.hpp>

class Entity
{

public:

	glm::vec2 getPosition();
	glm::vec2 getVelocity();
	float getAngle();
	float getWeight();

	void setPosition(glm::vec2 position);
	void setVelocity(glm::vec2 velocity);
	void setAngle(float angle);
	void setWeight(float weight);

protected:
	glm::vec2 position = glm::vec2(0, 0);
	glm::vec2 velocity = glm::vec2(0, 0);

	float angle = 0.f;
	float weight = 0.f;
};

