#include "Particle.h"

#include "Component.h"

bool Particle::operator<(Particle& that) {
	// Sort such that dead particles are at the end
	return this->life > that.life;
}

glm::vec3 Particle::getColour() {
	return colour;
}

float Particle::getLife() {
	return life;
}

void Particle::setColour(glm::vec3 colour) {
	this->colour = colour;
}

void Particle::setLife(float life) {
	this->life = life;
}

bool Particle::hasComponent() {
	return component != nullptr;
}

Component* Particle::getComponent() {
	return component;
}

void Particle::setComponent(Component* component) {
	this->component = component;
}

void Particle::removeComponent() {
	this->component = nullptr;
}

AABB Particle::computeAABB()
{
	return AABB {
		position + glm::vec2(-0.05, 0.05),
		position + glm::vec2(0.05, -0.05),
	};
}

void Particle::computeMass()
{
	if (density == 0) {
		mass = 0;
		invMass = 0;
	}
	else {
		mass = density;
		invMass = 1.f / density;
	}
}
