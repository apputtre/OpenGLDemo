#ifndef NORMALVERTEX_H
#define NORMALVERTEX_H

#include "SimpleVertex.h"
#include "linalg_types.h"

/*
This class represents all of the data that each is associated with each vertex of a mesh.
It inherits a position attribute from SimpleVertex and defines a normal vector attribute (thus the name 'NormalVertex').
*/
class NormalVertex : public SimpleVertex
{
public:
	vec3 normal;
	vec2 texCoords;

	NormalVertex(vec3 pos = { 0, 0, 0 }, vec3 normal = { 0, 0, 0 }, vec2 texCoords = { 0, 0 })
		: SimpleVertex(pos), normal{ normal }, texCoords{ texCoords }
	{}

	NormalVertex(float pos_x, float pos_y, float pos_z, float norm_x, float norm_y, float norm_z, float tex_x, float tex_y)
		: SimpleVertex(pos_x, pos_y, pos_z), normal{ norm_x, norm_y, norm_z }, texCoords{tex_x, tex_y}
	{}

	static void setAttributes(unsigned int vertex_buffer);
};

#endif