#include "glutils.h"

namespace glutils
{
	std::string getGLErrors(std::string file, int line)
	{
		std::string err = "";
		unsigned int errorCode;

		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			switch (errorCode)
			{
			case (GL_INVALID_ENUM):
			{
				err += "GL_INVALID_ENUM ";
				break;
			}
			case (GL_INVALID_VALUE):
			{
				err += "GL_INVALID_VALUE ";
				break;
			}
			case (GL_INVALID_OPERATION):
			{
				err += "GL_INVALID_OPERATION ";
				break;
			}
			case (GL_INVALID_FRAMEBUFFER_OPERATION):
			{
				err += "GL_INVALID_FRAMEBUFFER_OPERATION ";
				break;
			}
			case (GL_OUT_OF_MEMORY):
			{
				err += "GL_OUT_OF_MEMORY ";
				break;
			}
			case (GL_STACK_UNDERFLOW):
			{
				err += "GL_STACK_UNDERFLOW ";
				break;
			}
			case (GL_STACK_OVERFLOW):
			{
				err += "GL_STACK_OVERFLOW ";
				break;
			}
			}
		}

		if (err != "")
			err += std::format("(file {}, line {})", file, line);

		return err;
	}

	void APIENTRY glDebugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char* message, const void* userparam)
	{
		std::cout << "OpenGL Debug Message: " << message << " ID: (" << id << ")" << std::endl;

		std::cout << "Source: ";
		switch (source)
		{
		case GL_DEBUG_SOURCE_API:
			std::cout << "API";
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			std::cout << "Window System";
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			std::cout << "Shader Compiler";
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			std::cout << "Application";
			break;
		case GL_DEBUG_SOURCE_OTHER:
			std::cout << "Other";
			break;
		}

		std::cout << ", Type: ";
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:
			std::cout << "Error";
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			std::cout << "Deprecated Behaviour";
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			std::cout << "Undefined Behaviour";
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			std::cout << "Portability";
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			std::cout << "Performance";
			break;
		case GL_DEBUG_TYPE_MARKER:
			std::cout << "Marker";
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			std::cout << "Push Group";
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			std::cout << "Pop Group";
			break;
		case GL_DEBUG_TYPE_OTHER:
			std::cout << "Other";
			break;
		}
		std::cout << ", ";

		std::cout << "Severity: ";
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			std::cout << "high";
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			std::cout << "medium";
			break;
		case GL_DEBUG_SEVERITY_LOW:
			std::cout << "Severity: low";
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			std::cout << "notification";
			break;
		}
		std::cout << std::endl << std::endl;
	}
}
