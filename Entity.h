#pragma once

#include <glm/glm.hpp>

struct AABB {
	glm::vec2 topLeft;
	glm::vec2 bottomRight;
};


class Entity
{

public:

	glm::vec2 getPosition();
	glm::vec2 getVelocity();
	glm::vec2 getForce();
	float getMass();
	float getInvMass();
	float getDensity();
	float getRestitution();

	void setPosition(glm::vec2 position);
	void setVelocity(glm::vec2 velocity);
	void applyForce(glm::vec2 force);
	void setDensity(float density);
	void setRestitution(float restitution);

	virtual AABB computeAABB() = 0;

protected:
	glm::vec2 position = glm::vec2(0, 0);
	glm::vec2 velocity = glm::vec2(0, 0);
	glm::vec2 force = glm::vec2(0, 0);

	float mass = 0.f;
	float invMass = 0.f;
	float density = 0.f;
	float restitution = 0.f;

	virtual void computeMass() = 0;
};

