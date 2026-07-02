#include "CubeMesh.h"

using namespace linalg;

CubeMesh::CubeMesh()
{
	num_vertices = 8;
	num_indices = 36;

	vertex_data = new Vertex[num_vertices] {
		// front vertices
		{-0.5, -0.5, 0.5},	// 0
		{0.5, -0.5, 0.5},	// 1
		{0.5, 0.5, 0.5},	// 2
		{-0.5, 0.5, 0.5},	// 3
		// back vertices
		{0.5, -0.5, -0.5},	// 4
		{-0.5, -0.5, -0.5},	// 5
		{-0.5, 0.5, -0.5},	// 6
		{0.5, 0.5, -0.5}	// 7
	};

	// define the order in which the vertices are to be rendered
	indices = new unsigned int[num_indices] {
		// front face
		0, 1, 2,
		2, 3, 0,
		// right face
		//1, 4, 7,
		//7, 2, 1,
		// back face
		4, 5, 6,
		6, 7, 4,
		// left face
		//5, 0, 3,
		//3, 6, 5,
		// top face
		//3, 2, 7,
		//7, 6, 3,
		// bottom face
		//5, 4, 1,
		//1, 0, 5
	};
}
