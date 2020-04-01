#include "Component.h"
#include <algorithm> 
#include <iostream>

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
		//double x = ((double)rand() / (RAND_MAX));
		//double y = ((double)rand() / (RAND_MAX));
		//p->setVelocity(p->getVelocity() + glm::vec2(x, y));
		//p->setDensity(0.3f);

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

void Component::releaseChild(Particle* p)
{
	// Update to make position and velocity independant of parent component
	p->setPosition(position + p->getPosition());
	p->setVelocity(velocity + p->getVelocity());

	// Remove component reference
	p->removeComponent();

	// Erase from vector
	children.erase(std::find(children.begin(), children.end(), p));

	if (children.empty()) {
		std::cout << "Game over! Component has no particles left!" << std::endl;
	}
}

bool comp(Particle* a, Particle* b)
{
	return (a->getPosition().y < b->getPosition().y);
}

glm::vec2 Component::getTopPosition()
{
	if (!children.empty()) {
		Particle* yMaxP = *std::max_element(children.begin(), children.end(), comp);
		return yMaxP->getWorldPosition();
	}
	else {
		return getPosition();
	}
}
