#include "Entity.h"

glm::vec2 Entity::getPosition() {
	return position;
}

glm::vec2 Entity::getVelocity() {
	return velocity;
}

float Entity::getAngle() {
	return angle;
}

float Entity::getWeight() {
	return weight;
}

void Entity::setPosition(glm::vec2 position) {
	this->position = position;
}

void Entity::setVelocity(glm::vec2 velocity) {
	this->velocity = velocity;
}

void Entity::setAngle(float angle) {
	this->angle = angle;
}

void Entity::setWeight(float weight) {
	this->weight = weight;
}
