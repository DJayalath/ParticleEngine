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

            // Delete particles out of scope
            if (p.getPosition().x > 6.70f || p.getPosition().x < -6.70f ||
                p.getPosition().y > 3.90f || p.getPosition().y < -3.90f) {
                p.setLife(0);
                continue;
            }

            if (p.getMass() > 0) {
                //p.setVelocity(p.getVelocity() + glm::vec2(0, -0.5) * (float) dt);
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

    // Broad phase collisions
    broadPhaseGenPairs();

    // Resolve collisions
    resolvePairs();

    // Generate new particles
    {
        if (particlesCount < 50) {
            Particle& p = particlesContainer[findUnusedParticle()];
            p.setLife(1.f);
            p.setPosition(glm::vec2((rand() % 1280) / 100.f - 6.40f, 3.7f));
            p.setVelocity(glm::vec2(0, - (rand() % 5 + 1)));
            p.setDensity(0.3f);
            p.setRestitution(0.8f);

            // TEMPORARY
            static const glm::vec3 colours[] = {
                glm::vec3(1.f, 0, 0), glm::vec3(1.f, 0.5f, 0),
                glm::vec3(1.f, 1.f, 0), glm::vec3(0, 1.f, 0),
                glm::vec3(0, 0, 1.f), glm::vec3(0.58f, 0, 1.f),
                glm::vec3(46.f / 256.f, 43.f / 256.f, 95 / 256.f)
            };


            p.setColour(colours[rand() % 7]);
        }
    }
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

bool ParticleManager::sortPairs(Manifold lhs, Manifold rhs)
{
    if (lhs.A < rhs.A)
        return true;

    if (lhs.A == rhs.A)
        return lhs.B < rhs.B;

    return false;
}

bool ParticleManager::AABBvsAABB(Manifold* manifold)
{
    Particle* A = manifold->A;
    Particle* B = manifold->B;

    AABB a = A->computeAABB();
    AABB b = B->computeAABB();

    //std::cout << "BR: " << a.bottomRight.x << " TL: " << a.topLeft.x << std::endl;

    // Exit with no intersection if found separated along an axis
    if (a.bottomRight.x < b.topLeft.x || a.topLeft.x > b.bottomRight.x) return false;
    if (a.bottomRight.y > b.topLeft.y || a.topLeft.y < b.bottomRight.y) return false;

    //std::cout << "Collision detected!" << std::endl;

    glm::vec2 n = B->getPosition() - A->getPosition();

    // Half extents
    float a_extent = (a.bottomRight.x - a.topLeft.x) / 2.f;
    float b_extent = (b.bottomRight.x - b.topLeft.x) / 2.f;

    // Overlap on x-axis
    float x_overlap = a_extent + b_extent - abs(n.x);

    // SAT on x-axis
    if (x_overlap > 0) {
        // Half extents along y-axis
        float a_extent = (a.topLeft.y - a.bottomRight.y) / 2.f;
        float b_extent = (b.topLeft.y - b.bottomRight.y) / 2.f;

        // Overlap on y-axis
        float y_overlap = a_extent + b_extent - abs(n.y);

        // SAT on y-axis
        if (y_overlap > 0) {

            // Find out which axis is axis of least penetration
            if (x_overlap > y_overlap) {

                // y-axis is axis of least penetration
                // so normal is along y-axis (resolve along y-axis)

                // Point towards B as n is from A to B
                if (n.y < 0) {
                    manifold->normal = glm::vec2(0, -1);
                }
                else {
                    // MODIFIED
                    manifold->normal = glm::vec2(0, 1);
                }

                manifold->penetration = y_overlap;

                //std::cout << manifold->normal.x << std::endl;

                return true;

            }
            else {

                if (n.x < 0) {
                    manifold->normal = glm::vec2(-1, 0);
                }
                else {
                    manifold->normal = glm::vec2(1, 0);
                }

                manifold->penetration = x_overlap;

                //std::cout << manifold->normal.y << std::endl;

                return true;

            }

        }
    }

    return false;

    std::cout << "WARNING: If leak in AABBvsAABB" << std::endl;
}

void ParticleManager::broadPhaseGenPairs()
{
    pairs.clear();

    for (int i = 0; i < particlesCount; i++) {

        // Skip particles attached to components
        if (particlesContainer[i].hasComponent())
            continue;

        for (int j = 0; j < particlesCount; j++) {

            // Skip particles attached to components
            if (particlesContainer[j].hasComponent())
                continue;

            // Skip self check
            if (i == j)
                continue;

            AABB a = particlesContainer[i].computeAABB();
            AABB b = particlesContainer[j].computeAABB();
            Manifold m = Manifold{ &particlesContainer[i], &particlesContainer[j] };
            if (AABBvsAABB(&m))
                pairs.push_back(m);

        }
    }

    // Sort to expose duplicates
    std::sort(pairs.begin(), pairs.end(), sortPairs);

    uniquePairs.clear();

    int i = 0;
    while (i < pairs.size()) {
        Manifold* pair = &pairs.at(i);
        uniquePairs.push_back(pairs.at(i));
        i++;

        // Skip duplicates
        while (i < pairs.size()) {
            Manifold* potentialDupe = &pairs.at(i);
            if (pair->A != potentialDupe->B || pair->B != potentialDupe->A)
                break;
            i++;
        }
    }
}

void ParticleManager::resolvePairs()
{
    for (Manifold& m : uniquePairs) {

        Particle* A = m.A;
        Particle* B = m.B;

        glm::vec2 rv = B->getVelocity() - A->getVelocity();

        float velNormal = glm::dot(rv, m.normal);

        // Don't resolve separating velocities
        if (velNormal > 0)
            return;

        float e = std::min(A->getRestitution(), B->getRestitution());

        float j = -(1.f + e) * velNormal;
        j /= A->getInvMass() + B->getInvMass();

        glm::vec2 impulse = j * m.normal;
        //std::cout << m.normal.x << std::endl;
        A->setVelocity(A->getVelocity() - A->getInvMass() * impulse);
        B->setVelocity(B->getVelocity() + B->getInvMass() * impulse);

        // Positional correction
        const float percent = 0.2f; // usually 20% to 80%
        const float slop = 0.01f; // usually 0.01 to 0.1
        glm::vec2 correction = (std::max(m.penetration - slop, 0.0f) / (A->getInvMass() + B->getInvMass())) * percent * m.normal;
        A->setPosition(A->getPosition() - A->getInvMass() * correction);
        B->setPosition(B->getPosition() + B->getInvMass() * correction);

    }


}

Particle& ParticleManager::getUnusedParticle() {
    return particlesContainer[findUnusedParticle()];
}