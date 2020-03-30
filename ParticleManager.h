#pragma once

#include <glm/glm.hpp>
#include "Particle.h"

class ParticleManager
{
public:
	static const int MAX_PARTICLES = 3600;

	void update(double dt, glm::vec2* translations, glm::vec3* colours);
	Particle* getParticles();
	int getParticlesCount();

private:
	Particle particlesContainer[MAX_PARTICLES];
	int lastUsedParticle = 0;
	int particlesCount = 0;

    int findUnusedParticle();

    void sortParticles();
};

