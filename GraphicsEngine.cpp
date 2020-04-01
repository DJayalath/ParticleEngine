#include "GraphicsEngine.h"
#include <iostream>

GraphicsEngine::GraphicsEngine()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // For MacOS systems
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "Particle Engine", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        exit(EXIT_FAILURE);
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
}

void GraphicsEngine::start()
{
    onInitialisation();

    // Time stepping
    const double fps = FPS;
    const double dt = 1.0 / fps;
    double accumulator = 0;
    double frameStart = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {

        // Time stepping
        const double currentTime = glfwGetTime();
        // Store time elapsed since last frame begain
        accumulator += currentTime - frameStart;
        // Record starting of this frame
        frameStart = currentTime;
        // Avoid spiral of death and clamp dt
        if (accumulator > 0.2)
            accumulator = 0.2;

        // Physics updates
        while (accumulator > dt) {
            update(dt);
            accumulator -= dt;
        }

        // TODO: linear interpolation

        // Rendering

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        render();

        // Swap buffers
        glfwSwapBuffers(window);

        // Poll events
        glfwPollEvents();
    }

    cleanUp();
    glfwTerminate();
}
