#pragma once

#include <glm/glm.hpp>

struct Particle
{
    // No need for scale as all particles are the same! Keep this constant in the shader

    glm::vec2 position, velocity;

    glm::vec3 colour; // Float RGB in [0, 1]

    float size, angle, weight;

    float life;
};


struct ParticleManager
{
	static const int MaxParticles = 100;
	Particle ParticlesContainer[MaxParticles];
	int LastUsedParticle = 0;

    void update(glm::vec2* translations, glm::vec3* colours) {
        // Simulate all particles
        int ParticlesCount = 0;
        for (int i = 0; i < MaxParticles; i++) {

            Particle& p = ParticlesContainer[i]; // shortcut

            if (p.life > 0.0f) {

                // Simulate simple physics : gravity only, no collisions
                //p.velocity += glm::vec3(0.0f, -9.81f, 0.0f) * (float)delta * 0.5f;
                p.position += p.velocity;
                //ParticlesContainer[i].pos += glm::vec3(0.0f,10.0f, 0.0f) * (float)delta;

                // Fill the GPU buffer
                translations[ParticlesCount] = p.position;
                colours[ParticlesCount] = p.colour;

                //g_particule_position_size_data[4 * ParticlesCount + 3] = p.size;

                //g_particule_color_data[4 * ParticlesCount + 0] = p.r;
                //g_particule_color_data[4 * ParticlesCount + 1] = p.g;
                //g_particule_color_data[4 * ParticlesCount + 2] = p.b;
                //g_particule_color_data[4 * ParticlesCount + 3] = p.a;

                ParticlesCount++;

            }
        }
    }

    // Finds a Particle in ParticlesContainer which isn't used yet.
    // (i.e. life < 0);
    int findUnusedParticle() {

        for (int i = LastUsedParticle; i < MaxParticles; i++) {
            if (ParticlesContainer[i].life < 0) {
                LastUsedParticle = i;
                return i;
            }
        }

        for (int i = 0; i < LastUsedParticle; i++) {
            if (ParticlesContainer[i].life < 0) {
                LastUsedParticle = i;
                return i;
            }
        }

        return 0; // All particles are taken, override the first one
    }
};
