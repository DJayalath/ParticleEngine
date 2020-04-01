#include "GraphicsEngineRunner.h"


//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main()
{
    GraphicsEngineRunner engine = GraphicsEngineRunner();
    engine.start();
    return EXIT_SUCCESS;
}
//
//void framebuffer_size_callback(GLFWwindow* window, int width, int height)
//{
//    glViewport(0, 0, width, height);
//}
//
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//    //if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
//    //{
//    //    Particle& p = particleManager.getUnusedParticle();
//    //    p.setLife(1.f);
//    //    p.setColour(glm::vec3(1.f, 0, 0));
//    //    p.setPosition(player->getPosition() + glm::vec2(0, 0.3f));
//    //    p.setVelocity(glm::vec2(0, 2.f));
//    //    p.setDensity(0.8f);
//    //    p.setRestitution(0.8f);
//    //}
//}