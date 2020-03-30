#pragma once

#include <vector>
#include "Entity.h"
#include "Particle.h"

class Component : public Entity
{

public:

private:

	std::vector<Particle*> children;

};

