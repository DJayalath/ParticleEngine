#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define WIDTH 1280
#define HEIGHT 720
#define FPS 144

class GraphicsEngine
{
public:

	GraphicsEngine();
	void start();

protected:

	GLFWwindow* window = nullptr;

private:

	virtual void onInitialisation() = 0;
	virtual void update(double dt) = 0;
	virtual void render() = 0;
	virtual void cleanUp() = 0;

	// SET THESE MANUALLY
	//static void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
	//static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) = 0;
};

