#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const char* vshader_path, const char* fshader_path)
{
	ifstream vshader_file(vshader_path);

	if (!vshader_file.is_open())
		throw std::runtime_error(std::string("Could not open vertex shader file (" + std::string(vshader_path) + ")").c_str());

	ifstream fshader_file(fshader_path);

	if (!fshader_file.is_open())
		throw std::runtime_error(std::string("Could not open fragment shader file (" + std::string(fshader_path) + ")").c_str());

	vshaderID = glCreateShader(GL_VERTEX_SHADER);
	fshaderID = glCreateShader(GL_FRAGMENT_SHADER);
	ID = glCreateProgram();

	char info_log[512];
	int success;

	stringstream sstream;
	string str;

	// compile the vertex shader
	sstream << vshader_file.rdbuf();
	str = sstream.str();
	const char* vshader_source = str.c_str();
	glShaderSource(vshaderID, 1, &vshader_source, NULL);
	glCompileShader(vshaderID);
	glGetShaderiv(vshaderID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vshaderID, 512, NULL, info_log);
		error = string("COULD NOT COMPILE VERTEX SHADER:\n") + info_log + "\n";
		error += "SHADER SOURCE:\n" + string(vshader_source);
		return;
	}

	vshader_file.close();

	sstream.str("");
	str = "";

	// compile the fragment shader
	sstream << fshader_file.rdbuf();
	str = sstream.str();
	const char* fshader_source = str.c_str();
	glShaderSource(fshaderID, 1, &fshader_source, NULL);
	glCompileShader(fshaderID);
	glGetShaderiv(fshaderID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fshaderID, 512, NULL, info_log);
		error = string("COULD NOT COMPILE FRAGMENT SHADER\n") + info_log + "\n";
		error += string("SHADER SOURCE:\n") + fshader_source;
		return;
	}

	fshader_file.close();

	// link the shaders
	glAttachShader(ID, vshaderID);
	glAttachShader(ID, fshaderID);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, info_log);
		error = string("COULD NOT LINK SHADERS\n") + info_log;
		return;
	}
}

ShaderProgram::ShaderProgram(const char* vshader_path, const char* gshader_path, const char* fshader_path)
{
	ifstream vshader_file(vshader_path);

	if (!vshader_file.is_open())
		throw std::runtime_error(std::string("Could not open vertex shader file (" + std::string(vshader_path) + ")").c_str());

	ifstream gshader_file(gshader_path);

	if (!gshader_file.is_open())
		throw std::runtime_error(std::string("Could not open geometry shader file (" + std::string(gshader_path) + ")").c_str());

	ifstream fshader_file(fshader_path);

	if (!fshader_file.is_open())
		throw std::runtime_error(std::string("Could not open fragment shader file (" + std::string(fshader_path) + ")").c_str());

	vshaderID = glCreateShader(GL_VERTEX_SHADER);
	gshaderID = glCreateShader(GL_GEOMETRY_SHADER);
	fshaderID = glCreateShader(GL_FRAGMENT_SHADER);
	ID = glCreateProgram();

	char info_log[512];
	int success;

	stringstream sstream;
	string str;

	// compile the vertex shader
	sstream << vshader_file.rdbuf();
	str = sstream.str();
	const char* vshader_source = str.c_str();
	glShaderSource(vshaderID, 1, &vshader_source, NULL);
	glCompileShader(vshaderID);
	glGetShaderiv(vshaderID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vshaderID, 512, NULL, info_log);
		error = string("COULD NOT COMPILE VERTEX SHADER:\n") + info_log + "\n";
		error += "SHADER SOURCE:\n" + string(vshader_source);
		return;
	}

	vshader_file.close();

	sstream.str("");
	str = "";

	// compile the geometry shader
	sstream << gshader_file.rdbuf();
	str = sstream.str();
	const char* gshader_source = str.c_str();
	glShaderSource(gshaderID, 1, &gshader_source, NULL);
	glCompileShader(gshaderID);
	glGetShaderiv(gshaderID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(gshaderID, 512, NULL, info_log);
		error = string("COULD NOT COMPILE GEOMETRY SHADER\n") + info_log + "\n";
		error += string("SHADER SOURCE:\n") + gshader_source;
		return;
	}

	gshader_file.close();

	sstream.str("");
	str = "";

	// compile the fragment shader
	sstream << fshader_file.rdbuf();
	str = sstream.str();
	const char* fshader_source = str.c_str();
	glShaderSource(fshaderID, 1, &fshader_source, NULL);
	glCompileShader(fshaderID);
	glGetShaderiv(fshaderID, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fshaderID, 512, NULL, info_log);
		error = string("COULD NOT COMPILE FRAGMENT SHADER\n") + info_log + "\n";
		error += string("SHADER SOURCE:\n") + fshader_source;
		return;
	}

	fshader_file.close();

	// link the shaders
	glAttachShader(ID, vshaderID);
	glAttachShader(ID, gshaderID);
	glAttachShader(ID, fshaderID);
	glLinkProgram(ID);

	glGetProgramiv(ID, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, info_log);
		error = string("COULD NOT LINK SHADERS\n") + info_log;
		return;
	}
}


ShaderProgram::~ShaderProgram()
{
	glDeleteShader(vshaderID);
	glDeleteShader(fshaderID);
	glDeleteProgram(ID);
}

void ShaderProgram::use()
{
	if (ID == NULL)
		throw std::runtime_error("Attempted to use shader before compiling it");

	glUseProgram(ID);
}

string ShaderProgram::getError()
{
	return error;
}

int ShaderProgram::setUniform(string uniform_name, float value)
{
	try {
		unsigned int uniform_location = getUniform(uniform_name);
		glUniform1f(uniform_location, value);
	}
	catch (std::invalid_argument e) {
		error = e.what();
		throw e;
	}
	return 0;
}

int ShaderProgram::setUniform(string uniform_name, int value)
{
	try{
		unsigned int uniform_location = getUniform(uniform_name.c_str());
		glUniform1i(uniform_location, value);
	}
	catch (std::invalid_argument e)
	{
		error = e.what();
		throw e;
	}
	return 0;
}

int ShaderProgram::setUniform(string uniform_name, const linalg::vec<2, float>& value)
{
	try{
		unsigned int uniform_location = getUniform(uniform_name.c_str());
		glUniform2fv(uniform_location, 1, (GLfloat*) &value);
	}
	catch (std::invalid_argument e)
	{
		error = e.what();
		throw e;
	}
	return 0;
}

int ShaderProgram::setUniform(string uniform_name, const linalg::vec<3, float>& value)
{
	try{
		unsigned int uniform_location = getUniform(uniform_name.c_str());
		glUniform3fv(uniform_location, 1, (GLfloat*) &value);
	}
	catch (std::invalid_argument e)
	{
		error = e.what();
		throw e;
	}
	return 0;
}

int ShaderProgram::setUniform(string uniform_name, const linalg::vec<4, float>& value)
{
	try{
		unsigned int uniform_location = getUniform(uniform_name.c_str());
		glUniform4fv(uniform_location, 1, (GLfloat*) &value);
	}
	catch (std::invalid_argument e)
	{
		error = e.what();
		throw e;
	}
	return 0;
}


int ShaderProgram::setUniform(string uniform_name, const linalg::mat<4, 4, float>& value)
{
	try{
		unsigned int uniform_location = getUniform(uniform_name.c_str());
		glUniformMatrix4fv(uniform_location, 1, GL_TRUE, (GLfloat*) &value);
	}
	catch (std::invalid_argument e)
	{
		error = e.what();
		throw e;
	}

	return 0;
}

unsigned int ShaderProgram::getUniform(string uniform_name)
{
	unsigned int uniform_location = -1;
	uniform_location = glGetUniformLocation(ID, uniform_name.c_str());
	if (uniform_location == -1)
	{
		error = "COULD NOT ACCESS UNIFORM " + uniform_name;
		throw std::invalid_argument(error);
	}

	return uniform_location;
}
