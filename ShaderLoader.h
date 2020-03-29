#pragma once

#include <glad/glad.h>

class ShaderLoader
{
public:
	static GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);
private:
	ShaderLoader() {}
};

