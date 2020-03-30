#include "Component.h"

void Component::addChild(Particle* p)
{
	p->setComponent(this);
	children.push_back(p);
}

void Component::releaseChildren()
{
	for (Particle* p : children) {

		// TEMPORARY. MAKE PARTICLES FALL BY GIVING THEM WEIGHT.
		p->setWeight(1.f);

		// Update to make position and velocity independant of parent component
		p->setPosition(position + p->getPosition());
		p->setVelocity(velocity + p->getVelocity());

		// Remove component reference
		p->removeComponent();
	}

	// Remove all children from vector
	children.clear();
}

void Component::update(double dt)
{
	if (weight > 0) {
		velocity += glm::vec2(0, -0.1) * (float) dt;
	}
	position += velocity * (float) dt;
}
