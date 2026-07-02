#include "shaderutils.h"
#include <glad/glad.h>
#include <stdlib.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <exception>
#include <vector>

unsigned int shaderutils::createShader(GLenum shader_type, const char* source_path)
{
	{
		std::string str;
		const char* shader_source;

		std::ifstream file(source_path);

		if (file.fail())
		{
			throw std::invalid_argument("file could not be opened");
		}

		std::stringstream buf;
		buf << file.rdbuf();
		str = buf.str();
		shader_source = str.c_str();

		unsigned int shader_id = glCreateShader(shader_type);
		glShaderSource(shader_id, 1, &shader_source, NULL);
		glCompileShader(shader_id);

		int shader_compiled = 0;
		glGetShaderiv(shader_id, GL_COMPILE_STATUS, &shader_compiled);

		if (shader_compiled == GL_FALSE)
		{
			int report_length = 0;
			glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &report_length);

			std::vector<GLchar> report(report_length);
			glGetShaderInfoLog(shader_id, report_length, &report_length, &report[0]);

			throw std::invalid_argument(&report[0]);
		}

		file.close();

		return shader_id;
	}
}

unsigned int shaderutils::createShaderProgram(unsigned int vertex_shader, unsigned int fragment_shader)
{
	unsigned int program_id = glCreateProgram();

	glAttachShader(program_id, vertex_shader);
	glAttachShader(program_id, fragment_shader);

	glLinkProgram(program_id);

	int program_linked = 0;
	glGetProgramiv(program_id, GL_LINK_STATUS, &program_linked);

	if (program_linked == GL_FALSE)
	{
		int report_length = 0;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &report_length);

		std::vector<GLchar> report(report_length);

		glGetProgramInfoLog(program_id, report_length, &report_length, &report[0]);

		throw std::invalid_argument(&report[0]);
	}

	return program_id;
}