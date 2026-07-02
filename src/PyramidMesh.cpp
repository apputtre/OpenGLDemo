#include "PyramidMesh.h"

#include "linalg_types.h"

PyramidMesh::PyramidMesh()
{
	num_vertices = 16;
	num_indices = 18;

	float h = sqrt(2.0f) / 2.0f;

	vertex_data = new Vertex[num_vertices]
	{
		// front face
		{vec3{0.5, -0.5, -h / 2},	vec3{h, 0, 0.5}},	// 0
		{vec3{0.5, 0.5, -h / 2},	vec3{h, 0, 0.5}},	// 1
		{vec3{0, 0, h / 2},		vec3{h, 0, 0.5}},	// 2
		// right face
		{vec3{0.5, 0.5, -h / 2},	vec3{0, h, 0.5}},	// 3
		{vec3{-0.5, 0.5, -h / 2},	vec3{0, h, 0.5}},	// 4
		{vec3{0, 0, h / 2},		vec3{0, h, 0.5}},	// 5
		// back face
		{vec3{-0.5, 0.5, -h / 2},	vec3{-h, 0, 0.5}},	// 6
		{vec3{-0.5, -0.5, -h / 2},	vec3{-h, 0, 0.5}},	// 7
		{vec3{0, 0, h / 2},		vec3{-h, 0, 0.5}},	// 8
		// left face
		{vec3{-0.5, -0.5, -h / 2},	vec3{0, -h, 0.5}},	// 9
		{vec3{0.5, -0.5, -h / 2},	vec3{0, -h, 0.5}},	// 10
		{vec3{0, 0, h / 2},		vec3{0, -h, 0.5}},	// 11
		// base
		{vec3{0.5, -0.5, -h / 2},	vec3{0, 0, -1}},	// 12
		{vec3{-0.5, -0.5, -h / 2},	vec3{0, 0, -1}},	// 13
		{vec3{-0.5, 0.5, -h / 2},	vec3{0, 0, -1}},	// 14
		{vec3{0.5, 0.5, -h / 2},	vec3{0, 0, -1}}	// 15
	};

	indices = new unsigned int[num_indices]
	{
		0, 1, 2,		// side
		3, 4, 5,		// side
		6, 7, 8,		// side
		9, 10, 11,		// side
		12, 13, 14,		// base
		14, 15, 12		// base
	};
}
