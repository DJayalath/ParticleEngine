#include "ParticleManager.h"
#include "Component.h"

#include <algorithm>
#include <iostream>

void ParticleManager::update(double dt, glm::vec2* translations, glm::vec3* colours) {
    // Simulate all particles
    particlesCount = 0;
    for (int i = 0; i < MAX_PARTICLES; i++) {

        Particle& p = particlesContainer[i]; // shortcut

        if (p.getLife() > 0.0f) {

            if (p.getMass() > 0) {
                p.setVelocity(p.getVelocity() + glm::vec2(0, -0.5) * (float) dt);
            }

            p.setPosition(p.getPosition() + p.getVelocity() * (float) dt);

            // Fill the GPU buffer
            if (p.hasComponent()) {
                translations[particlesCount] = p.getPosition() + p.getComponent()->getPosition();
            }
            else {
                translations[particlesCount] = p.getPosition();
            }

            colours[particlesCount] = p.getColour();

            particlesCount++;

        }
    }

    sortParticles();
}

Particle* ParticleManager::getParticles()
{
    return particlesContainer;
}

int ParticleManager::getParticlesCount()
{
    return particlesCount;
}

// Used to find new particles to bring to life
int ParticleManager::findUnusedParticle() {

    for (int i = lastUsedParticle; i < MAX_PARTICLES; i++) {
        if (particlesContainer[i].getLife() <= 0) {
            lastUsedParticle = i;
            return i;
        }
    }

    for (int i = 0; i < lastUsedParticle; i++) {
        if (particlesContainer[i].getLife() <= 0) {
            lastUsedParticle = i;
            return i;
        }
    }

    std::cout << "WARNING: All particles taken. Overriding first particle!" << std::endl;
    return 0; // All particles are taken, override the first one
}

void ParticleManager::sortParticles() {
    std::sort(&particlesContainer[0], &particlesContainer[MAX_PARTICLES]);
}

Particle& ParticleManager::getUnusedParticle() {
    return particlesContainer[findUnusedParticle()];
}