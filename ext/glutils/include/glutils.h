#ifndef GLUTILS_H
#define GLUTILS_H

#include <iostream>
#include <glad/glad.h>
#include <format>

namespace glutils
{
	std::string getGLErrors(std::string file, int line);

	void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userparam);
}


#endif
