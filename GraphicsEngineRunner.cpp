#include "GraphicsEngineRunner.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Component.h"
#include "ShaderLoader.h" // Final include

void GraphicsEngineRunner::onInitialisation()
{
    // Set callbacks
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);
    glfwSetKeyCallback(window, keyCallback);


	// Create and compile GLSL particle shaders
	particleShader = ShaderLoader::LoadShaders("ParticleVS.vertexshader", "ParticleFS.fragmentshader");

    // Set ortho camera (2D) - If 1280x720, world space -> x in [-6.4, 6.4], y in [-3.6, 3.6]
    // EVERYTHING IS CENTERED IN OPENGL
    float scaled_width = WIDTH / 200.f;
    float scaled_height = HEIGHT / 200.f;
    glm::mat4 Projection = glm::ortho(-scaled_width, scaled_width, -scaled_height, scaled_height, 0.0f, 100.0f); // In world coordinates
    // Camera matrix
    glm::mat4 View = glm::lookAt(
        glm::vec3(0, 0, 1), // Camera just behind origin, in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    mv = Projection * View; // Remember, matrix multiplication is the other way around

    mvMatrixID = glGetUniformLocation(particleShader, "MV");

    // store instance data in an array buffer
    // --------------------------------------
    instanceTranslationsVBO;
    glGenBuffers(1, &instanceTranslationsVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceTranslationsVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * ParticleManager::MAX_PARTICLES, &translations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    instanceColoursVBO;
    glGenBuffers(1, &instanceColoursVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceColoursVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * ParticleManager::MAX_PARTICLES, &colours[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    const float quadVertices[] = {
        // positions     // colors
        -0.05f,  0.05f,
         0.05f, -0.05f,
        -0.05f, -0.05f,
        -0.05f,  0.05f,
         0.05f, -0.05f,
         0.05f,  0.05f,
    };

    GLuint quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    // Instance colours
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, instanceColoursVBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(1, 1); // tell OpenGL this is an instanced vertex attribute.

    // also set instance data
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, instanceTranslationsVBO); // this attribute comes from a different vertex buffer
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribDivisor(2, 1); // tell OpenGL this is an instanced vertex attribute.

    particleManager = new ParticleManager();

    // Create player
    player = new Component();
    player->setPosition(glm::vec2(0, -3.0f));
    for (int i = 3; i >= 0; i--) {
        for (int j = i; j >= 0; j--) {
            Particle& p = particleManager->getUnusedParticle();
            p.setLife(1.f);
            float offset = (i + 1.f) / 2.f * 0.1f;
            p.setPosition(glm::vec2(0.1f * j - offset, -0.1f * i)); // Relative to component centre
            p.setColour(glm::vec3(abs(i) / 3.f, abs(j) / 3.f, 1.f));
            player->addChild(&p);
        }
    }

    glfwSetWindowUserPointer(window, (void*)this);
}

void GraphicsEngineRunner::update(double dt)
{
    // Exit protocol
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    // Temporary player handling
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            player->setPosition(player->getPosition() + glm::vec2(0, 0.025f));

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            player->setPosition(player->getPosition() + glm::vec2(-0.025f, 0));

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            player->setPosition(player->getPosition() + glm::vec2(0, -0.025f));

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            player->setPosition(player->getPosition() + glm::vec2(0.025f, 0));
        // Spacebar handled in key callback
    }

    player->update(dt);

    // Update particles
    particleManager->update(dt, translations, colours);
}

void GraphicsEngineRunner::render()
{
    // Render particles
    if (particleManager->getParticlesCount() > 0) {

        // Use particle shader
        glUseProgram(particleShader);

        // Send model * view matrix to shader
        // Doesn't need to be in loop, but in case future updates involve moving the camera...
        glUniformMatrix4fv(mvMatrixID, 1, GL_FALSE, &mv[0][0]);

        // Send updated data to shader
        glBindBuffer(GL_ARRAY_BUFFER, instanceTranslationsVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * particleManager->getParticlesCount(), &translations[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, instanceColoursVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * particleManager->getParticlesCount(), &colours[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Instanced rendering of all particles
        glBindVertexArray(quadVAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, particleManager->getParticlesCount()); // particlesCount triangles of 6 vertices each (a quad)
        glBindVertexArray(0);
    }
}

void GraphicsEngineRunner::cleanUp()
{
    delete player;
    delete particleManager;
}

void GraphicsEngineRunner::frameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void GraphicsEngineRunner::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    GraphicsEngineRunner* graphicsEngineRunner = (GraphicsEngineRunner*) glfwGetWindowUserPointer(window);
    ParticleManager* particleManager = graphicsEngineRunner->particleManager;
    Component* player = graphicsEngineRunner->player;

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        Particle& p = particleManager->getUnusedParticle();
        p.setLife(1.f);
        p.setColour(glm::vec3(1.f, 0, 0));
        p.setPosition(player->getPosition() + glm::vec2(0, 0.3f));
        p.setVelocity(glm::vec2(0, 2.f));
        p.setDensity(0.8f);
        p.setRestitution(0.8f);
    }
}