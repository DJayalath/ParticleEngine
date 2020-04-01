#include "Entity.h"

glm::vec2 Entity::getPosition() {
	return position;
}

glm::vec2 Entity::getVelocity() {
	return velocity;
}

glm::vec2 Entity::getForce()
{
	return force;
}

float Entity::getMass() {
	return mass;
}

float Entity::getInvMass()
{
	return invMass;
}

float Entity::getDensity()
{
	return density;
}

float Entity::getRestitution()
{
	return restitution;
}

void Entity::setPosition(glm::vec2 position) {
	this->position = position;
}

void Entity::setVelocity(glm::vec2 velocity) {
	this->velocity = velocity;
}

void Entity::applyForce(glm::vec2 force)
{
	this->force += force;
}

void Entity::setDensity(float density)
{
	this->density = density;
	computeMass();
}

void Entity::setRestitution(float restitution)
{
	this->restitution = restitution;
}
