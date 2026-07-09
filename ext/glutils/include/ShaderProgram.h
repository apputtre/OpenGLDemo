#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <fstream>
#include <exception>
#include <string>
#include <sstream>
#include <iostream>
#include <exception>

#include "glad/glad.h"
#include "linalg/mat.h"

using std::ifstream;
using std::string;
using std::stringstream;

class ShaderProgram
{
private:
	string error;

	unsigned int ID = 0;
	unsigned int vshaderID = 0;
	unsigned int gshaderID = 0;
	unsigned int fshaderID = 0;
public:
	ShaderProgram(const char* vshader_path, const char* fshader_path);

	ShaderProgram(const char* vshader_path, const char* gshader_path, const char* fshader_path);

	~ShaderProgram();

	void use();

	unsigned int getID() { return ID; }

	string getError();

	// set the value of a uniform of type float
	int setUniform(string uniform_name, float value);

	// set the value of a uniform of type int
	int setUniform(string uniform_name, int value);

	// set the value of a uniform of 4x4 float matrix type
	int setUniform(string uniform_name, const linalg::mat<4, 4, float>& value);

	// set the value of a uniform of 2-element float vector type
	int setUniform(string uniform_name, const linalg::vec<2, float>& value);

	// set the value of a uniform of 3-element float vector type
	int setUniform(string uniform_name, const linalg::vec<3, float>& value);

	// set the value of a uniform of 4-element float vector type
	int setUniform(string uniform_name, const linalg::vec<4, float>& value);

	// get the location of a uniform
	unsigned int getUniform(string uniform_name);
};

#endif