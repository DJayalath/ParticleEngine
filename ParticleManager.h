#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "Particle.h"

struct Pair {
	Particle* A;
	Particle* B;
};

struct Manifold {
	Particle* A;
	Particle* B;
	float penetration;
	glm::vec2 normal;
};

class ParticleManager
{
public:
	static const int MAX_PARTICLES = 3600;

	void update(double dt, glm::vec2* translations, glm::vec3* colours);
	Particle* getParticles();
	Particle& getUnusedParticle();
	int getParticlesCount();

private:
	Particle particlesContainer[MAX_PARTICLES];
	int lastUsedParticle = 0;
	int particlesCount = 0;

    int findUnusedParticle();
    void sortParticles();

	std::vector<Manifold> pairs;
	std::vector<Manifold> uniquePairs;

	static bool sortPairs(Manifold lhs, Manifold rhs);
	bool AABBvsAABB(Manifold* manifold);
	void broadPhaseGenPairs();
	void resolvePairs();
};

