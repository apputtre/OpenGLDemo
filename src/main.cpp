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

	ShaderProgram lightsource_shader(
		"shaders\\phong\\vertex.vert",
		"shaders\\phong\\lightsource.frag"
	);

	if (lightsource_shader.getError() != "")
	{
		std::cout << lightsource_shader.getError();
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

	Camera cam;

	cam.setPos(vec3 {0, 0, 50});

	lightsource_shader.use();
	lightsource_shader.setUniform("view", cam.getTransform());
	lightsource_shader.setUniform("projection", perspective_projection);
	lightsource_shader.setUniform("lightColor", lightColor);

	bool use_debug_shader = false;

	float last_tick = (float) SDL_GetTicks();
	float last_draw_time_report = last_tick;
	bool quit = false;
	SDL_Event e;
	while (!quit)
	{
		float delta = (float) (SDL_GetTicks() - last_tick);
		last_tick = (float) SDL_GetTicks();

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