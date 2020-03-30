#pragma once

#include <glm/glm.hpp>
#include "Entity.h"

class Component;
class Particle : public Entity
{
public:

    glm::vec3 getColour();
    float getLife();

    void setColour(glm::vec3 colour);
    void setLife(float life);

    bool operator<(Particle& that);

    bool hasComponent();
    Component* getComponent();
    void setComponent(Component* component);
    void removeComponent();

    AABB computeAABB();

private:
    // No need for scale as all particles are the same! Keep this constant in the shader
    glm::vec3 colour = glm::vec3(0, 0, 0); // Float RGB in [0, 1]
    float life = 0.f;

    Component* component = nullptr;
};