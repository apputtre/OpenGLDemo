#include <SDL.h>
#include <glad/glad.h>

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <ctime>
#include <chrono>

#include "linalg/vec.h"
#include "linalg/mat.h"
#include "tl3d/transformations.h"
#include "linalg_types.h"
#include "CubeMesh.h"
#include "NormalVertex.h"
#include "NormalInstanceData.h"
#include "PyramidMesh.h"
#include "CubeMeshNormals.h"
#include "Camera.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "ShaderProgram.h"

#define SHADER_PATH "..\shaders"

#define WINDOW_WIDTH 960
#define WINDOW_HEIGHT 540
#define PI 3.141592653

using namespace linalg;

void seedRand(int seed = std::time(nullptr));

double dRandInRange(double min, double max);

int window_width = 960;
int window_height = 540;
int viewport_width = window_width;
int viewport_height = window_height;
int viewport_pos_x = 0;
int viewport_pos_y = 0;
const float aspect = (float) window_width / window_height;

int main(int argc, char* argv[])
{
	SDL_Init(SDL_INIT_EVERYTHING);

	// create the game window

	SDL_Window* window = NULL;
	window = SDL_CreateWindow (
		"Lighting Demo 3",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);

	if (window == NULL)
	{
		std::cout << "window creation failed: " << SDL_GetError() << "\n";
		return 1;
	}

	SDL_SetRelativeMouseMode(SDL_bool::SDL_TRUE);

	// set some attributes for opengl

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// enable debugging
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

	// create the opengl context
	SDL_GLContext gl_context;
	gl_context = SDL_GL_CreateContext(window);
	if (gl_context == NULL)
	{
		std::cout << "gl context creation failed: " << SDL_GetError() << "\n";
		return 1;
	}

	// initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return 1;
	}

	int flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glutils::glDebugOutput, nullptr);

		// ids of messages to ignore
		const int BUFFER_OBJECT_BOUND = 131185;
		const int SHADER_PERFORMANCE_WARNING = 131218;

		// disable unimportant messages
		glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 1, (const GLuint*) &BUFFER_OBJECT_BOUND, GL_FALSE);
		glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_PERFORMANCE, GL_DONT_CARE, 1, (const GLuint*) &SHADER_PERFORMANCE_WARNING, GL_FALSE);
	}

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	ShaderProgram object_shader(
		"shaders\\phong\\vertex.vert",
		"shaders\\phong\\object.frag"
	);

	if (object_shader.getError() != "")
	{
		std::cout << object_shader.getError();
		return 1;
	}

	ShaderProgram lightsource_shader(
		"shaders\\phong\\vertex.vert",
		"shaders\\phong\\lightsource.frag"
	);

	if (lightsource_shader.getError() != "")
	{
		std::cout << lightsource_shader.getError();
		return 1;
	}

	ShaderProgram debug_shader(
		"shaders\\debug\\vertex.vert",
		"shaders\\debug\\fragment.frag"
	);

	if (debug_shader.getError() != "")
	{
		std::cout << debug_shader.getError();
		return 1;
	}

	// create the projection matrix
	float fov = PI / 4.0;
	float aspect = 16.0 / 9.0;
	float near_plane = 0.1;
	float far_plane = 1000.0;
	mat4 perspective_projection = tl3d::perspective(fov, aspect, near_plane, far_plane);

	CubeMeshNormals cMesh;
	cMesh.initialize();

	// create the mesh that represents the light source
	CubeMeshNormals::InstanceData lightsource_cube;

	vec3 lightColor = vec3{ 1, 0.8, 0.4 };
	vec3 lightPos = vec3{ 0, 0, 0 };
	vec3 lightScale = vec3{ 10, 10, 10 };

	lightsource_cube.model = mat4(1);
	tl3d::trans(tl3d::rotate(tl3d::scale(lightsource_cube.model, lightScale), (float) PI / 6.0f, vec3 {1, 1, 1}), lightPos);

	Camera cam(vec3{ -82.91, -4.81, 43.72 }, vec3{ 1, 0, 0 }, vec3{-0.03, 0.91, -0.42});

	// create the material maps
	unsigned int texDiffuse, texSpecular;
	{
		glGenTextures(1, &texDiffuse);
		glGenTextures(1, &texSpecular);

		stbi_set_flip_vertically_on_load(true);

		int width, height, nrChannels;
		unsigned char* data = stbi_load("assets\\container_diffuse.png", &width, &height, &nrChannels, 0);

		if (data == nullptr)
			throw std::runtime_error("Failed to load texture from disk");

		/*
		1: Texture target
			GL_TEXTURE_2D indicates that the operation will generate a texture on the currently bound texture object
		2: mipmap level
			0 indicates the base mipmap level
		3: Texture format
			RGB indicates that the texture has only red, green, and blue values
		4, 5: width and height of texture
		6: Always 0 (legacy)
		7: image data
		*/
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texDiffuse);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		// generate mipmaps for the currently bound texture
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);

		data = stbi_load("assets\\container_specular.png", &width, &height, &nrChannels, 0);

		if (data == nullptr)
			throw std::runtime_error("Failed to load texture from disk");

		/*
		1: Texture target
			GL_TEXTURE_2D indicates that the operation will generate a texture on the currently bound texture object
		2: mipmap level
			0 indicates the base mipmap level
		3: Texture format
			RGB indicates that the texture has only red, green, and blue values
		4, 5: width and height of texture
		6: Always 0 (legacy)
		7: image data
		*/
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texSpecular);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		// generate mipmaps for the currently bound texture
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(data);
	}

	object_shader.use();
	object_shader.setUniform("view", cam.getTransform());
	object_shader.setUniform("projection", perspective_projection);
	object_shader.setUniform("viewPos", cam.getPos());
	// set the location of the diffuse map texture
	object_shader.setUniform("matDiffuse", 0);
	object_shader.setUniform("matSpecular", 1);

	// set light properties
	object_shader.setUniform("light.position", lightPos);
	object_shader.setUniform("light.ambient", lightColor * 0.1f);
	object_shader.setUniform("light.diffuse", lightColor);
	object_shader.setUniform("light.specular", lightColor * 0.75f);

	lightsource_shader.use();
	lightsource_shader.setUniform("view", cam.getTransform());
	lightsource_shader.setUniform("projection", perspective_projection);
	lightsource_shader.setUniform("lightColor", lightColor);

	debug_shader.use();
	debug_shader.setUniform("view", cam.getTransform());
	debug_shader.setUniform("projection", perspective_projection);

	int num_cubes = 1000;
	CubeMeshNormals::InstanceData* cubes = (CubeMeshNormals::InstanceData*)malloc(sizeof(CubeMeshNormals::InstanceData) * num_cubes);

	struct CubeTransform
	{
		vec3 pos;
		vec3 scale = vec3(1);
		quat orientation = quat();
		quat angular_velocity = quat();
		// false: counterclockwise, true: clockwise
		bool rotation_direction = false;
		float orbit_velocity;
	};

	CubeTransform* transforms = (CubeTransform*)malloc(sizeof(CubeTransform) * num_cubes);

	float radius = 75;
	float position_variance = radius * 0.2;
	float cube_scale = 1;
	float scale_variance = cube_scale * 0.25;
	float angular_velocity = 0;
	float angular_velocity_variance = 0.5;
	float clockwise_chance = 0;
	float orbit_velocity = 0.01;
	float orbit_velocity_variance = orbit_velocity * 0.75;
	for (int i = 0; i < num_cubes; ++i)
	{
		transforms[i] = CubeTransform();

		transforms[i].pos = {
			(radius + dRandInRange(-position_variance, position_variance)) * cos(i * PI * 1.0 / num_cubes),
			dRandInRange(-position_variance, position_variance),
			(radius + dRandInRange(-position_variance, position_variance)) * sin(i * PI * 2.0 / num_cubes)
		};

		transforms[i].scale = vec3(cube_scale + dRandInRange(-scale_variance, scale_variance));

		vec3 rotation_axis = vec3(
			dRandInRange(0, 1),
			dRandInRange(0, 1),
			dRandInRange(0, 1)
		).norm();

		float ang_vel_mag = angular_velocity + dRandInRange(-angular_velocity_variance, angular_velocity_variance);

		transforms[i].angular_velocity = quat(0, rotation_axis.x * ang_vel_mag, rotation_axis.y * ang_vel_mag, rotation_axis.z * ang_vel_mag);

		if (dRandInRange(0, 1) <= clockwise_chance)
			transforms[i].rotation_direction = true;

		transforms[i].orbit_velocity = orbit_velocity + dRandInRange(-orbit_velocity_variance, orbit_velocity_variance);
	}

	for (int i = 0; i < num_cubes; ++i)
	{
		cubes[i] = CubeMeshNormals::InstanceData();

		//cubes[i].model = trans(mat4(1), transforms[i].pos) * rotate(mat4(1), transforms[i].orientation) * scale(mat4(1), transforms[i].scale);
		cubes[i].model = mat4(1);
		tl3d::trans(tl3d::rotate(tl3d::scale(cubes[i].model, transforms[i].scale), transforms[i].orientation), transforms[i].pos);
		//trans(cubes[i].model, transforms[i].pos) * rotate(mat4(1), transforms[i].orientation) * scale(mat4(1), transforms[i].scale);
		cubes[i].shininess = 64;
	}

	bool use_debug_shader = false;

	float last_tick = (float) SDL_GetTicks();
	float last_draw_time_report = last_tick;
	bool quit = false;
	SDL_Event e;
	while (!quit)
	{
		float delta = (float) (SDL_GetTicks() - last_tick);
		last_tick = (float) SDL_GetTicks();

		for (int i = 0; i < num_cubes; ++i)
		{
			// rotate about the origin
			/*
			if (!transforms[i].rotation_direction)
			{
				transforms[i].pos = rotate(trans(mat4(1), transforms[i].pos), transforms[i].orbit_velocity * delta / 1000, { 0, 1, 0 }).col(3);
			}
			else
			{
				transforms[i].pos = rotate(trans(mat4(1), transforms[i].pos), -transforms[i].orbit_velocity * delta / 1000, { 0, 1, 0 }).col(3);
			}
			*/

			// update orientation by angular velocity
			transforms[i].orientation += (delta / 1000) / 2 * transforms[i].angular_velocity * transforms[i].orientation;

			// update the model matrix
			cubes[i].model = mat4(1);
			tl3d::trans(tl3d::rotate(tl3d::scale(cubes[i].model, transforms[i].scale), transforms[i].orientation), transforms[i].pos);
		}

		object_shader.use();
		object_shader.setUniform("view", cam.getTransform());
		object_shader.setUniform("viewPos", cam.getPos());

		lightsource_shader.use();
		lightsource_shader.setUniform("view", cam.getTransform());

		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_WINDOWEVENT)
			{
				switch (e.window.event)
				{
				case SDL_WINDOWEVENT_CLOSE:
					quit = true;
					break;
				case SDL_WINDOWEVENT_RESIZED:
					window_width = e.window.data1;
					window_height = e.window.data2;

					if (window_width / aspect > window_height)
					{
						viewport_height = window_height;
						viewport_width = viewport_height * aspect;
					}
					else
					{
						viewport_width = window_width;
						viewport_height = viewport_width / aspect;
					}

					viewport_pos_x = (window_width - viewport_width) / 2;
					viewport_pos_y = (window_height - viewport_height) / 2;

					glViewport((window_width - viewport_width) / 2, (window_height - viewport_height) / 2, viewport_width, viewport_height);
					break;
				}
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				if (e.motion.yrel != 0)
					cam.handleInput(Camera::Pitch, -e.motion.yrel);
				if (e.motion.xrel != 0)
					cam.handleInput(Camera::Yaw, -e.motion.xrel);
			}
		}

		cam.update(delta);

		lightsource_shader.use();
		lightsource_shader.setUniform("view", cam.getTransform());

		auto keyboard_state = SDL_GetKeyboardState(NULL);

		if (keyboard_state[SDL_SCANCODE_W])
			cam.handleInput(Camera::Front, 1);
		if (keyboard_state[SDL_SCANCODE_S])
			cam.handleInput(Camera::Front, -1);
		if (keyboard_state[SDL_SCANCODE_A])
			cam.handleInput(Camera::Right, -1);
		if (keyboard_state[SDL_SCANCODE_D])
			cam.handleInput(Camera::Right, 1);
		if (keyboard_state[SDL_SCANCODE_SPACE])
			cam.handleInput(Camera::Up, 1);
		if (keyboard_state[SDL_SCANCODE_LSHIFT])
			cam.handleInput(Camera::Up, -1);
		if (keyboard_state[SDL_SCANCODE_Q])
			cam.handleInput(Camera::Roll, -1);
		if (keyboard_state[SDL_SCANCODE_E])
			cam.handleInput(Camera::Roll, 1);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		auto draw_time_start = std::chrono::system_clock::now();

		if (use_debug_shader)
			debug_shader.use();
		else
			object_shader.use();

		cMesh.draw(cubes, num_cubes);

		// draw the light source
		lightsource_shader.use();
		cMesh.draw(&lightsource_cube, 1);

		auto draw_time_end = std::chrono::system_clock::now();

		if (last_tick - last_draw_time_report >= 1000)
		{
			auto draw_time = std::chrono::duration_cast<std::chrono::microseconds>(draw_time_end - draw_time_start);

			std::cout << std::format("Draw time: {}", draw_time) << std::endl;

			last_draw_time_report = SDL_GetTicks();
		}

		SDL_GL_SwapWindow(window);

		std::string errors = glutils::getGLErrors(__FILE__, __LINE__);
		if (errors != "")
			std::cout << errors << "\n";
	}

	free(cubes);
	free(transforms);

	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}

void seedRand(int seed)
{
	std::srand(seed);
}

double dRandInRange(double min, double max)
{
	return min + (double)std::rand() / (RAND_MAX / (max - min));
}