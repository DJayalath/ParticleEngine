#pragma once

#include <vector>
#include "Entity.h"
#include "Particle.h"

class Component : public Entity
{

public:

	void addChild(Particle* p);
	void releaseChildren();
	void update(double dt);
	void computeMass();
	void releaseChild(Particle* p);
	glm::vec2 getTopPosition();

	// TODO: Override this with actual functionality
	AABB computeAABB() { return AABB(); }

private:

	std::vector<Particle*> children;

};

