#ifndef SHADERUTILS_H
#define SHADERUTILS_H

#include <glad/glad.h>

namespace shaderutils
{
	unsigned int createShader(GLenum shader_type, const char* source_path);

	unsigned int createShaderProgram(unsigned int vertex_shader, unsigned int fragment_shader);
}

#endif