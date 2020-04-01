#pragma once

#include "GraphicsEngine.h"
#include "ParticleManager.h"

class GraphicsEngineRunner : public GraphicsEngine
{
public:
    ParticleManager* particleManager;
    Component* player;

private:

    void onInitialisation() override;
    void update(double dt) override;
    void render() override;
    void cleanUp() override;

    glm::mat4 mv;
    GLuint instanceTranslationsVBO, instanceColoursVBO;
    GLuint quadVAO;

    glm::vec2 translations[ParticleManager::MAX_PARTICLES];
    glm::vec3 colours[ParticleManager::MAX_PARTICLES];

    GLuint particleShader;
    GLuint mvMatrixID;

    static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

};

