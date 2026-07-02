#ifndef SIMPLEVERTEX_H
#define SIMPLEVERTEX_H

#include "VertexBase.h"
#include "linalg_types.h"

class SimpleVertex : public VertexBase
{
public:
	vec3 pos;

	SimpleVertex()
		: pos{ 0, 0, 0 }
	{}

	SimpleVertex(vec3 pos)
		: pos{ pos }
	{}

	SimpleVertex(float x, float y, float z)
		: pos{ x, y, z }
	{}

	static void setAttributes(unsigned int vertex_buffer);
};

#endif