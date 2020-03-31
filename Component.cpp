#include "Component.h"

void Component::addChild(Particle* p)
{
	p->setComponent(this);
	children.push_back(p);
	computeMass();
}

void Component::releaseChildren()
{
	for (Particle* p : children) {

		// Update to make position and velocity independant of parent component
		p->setPosition(position + p->getPosition());
		p->setVelocity(velocity + p->getVelocity());

		// TEMPORARY. MAKE PARTICLES MOVE RANDOMLY BY GIVING THEM RANDOM VELOCITY.
		double x = ((double)rand() / (RAND_MAX));
		double y = ((double)rand() / (RAND_MAX));
		p->setVelocity(p->getVelocity() + glm::vec2(x, y));
		p->setDensity(0.3f);

		// Remove component reference
		p->removeComponent();
	}

	// Remove all children from vector
	children.clear();
}

void Component::update(double dt)
{
	if (mass > 0) {
		velocity += glm::vec2(0, -0.1) * (float) dt;
	}
	position += velocity * (float) dt;
}

void Component::computeMass()
{
	if (density != 0) {
		mass = density * children.size();

		if (mass == 0)
			invMass = 0;
		else
			invMass = 1.f / mass;
	}
	else {
		mass = 0;
		invMass = 0;
	}
}
