#pragma once

#include <vector>
#include "Entity.h"
#include "Particle.h"

class Component : public Entity
{

public:

	void addChild(Particle* p);
	void releaseChildren();
	void update();

private:

	std::vector<Particle*> children;

};

