#ifndef	MESH_H
#define MESH_H

#include<vector>
#include<type_traits>

#include "SimpleInstanceData.h"
#include "SimpleVertex.h"
#include "glutils.h"

template<std::derived_from<VertexBase> T = SimpleVertex, std::derived_from<InstanceDataBase> U = SimpleInstanceData>
class Mesh
{
public:
	using Vertex = T;
	using InstanceData = U;

protected:
	bool initialized = false;

	Vertex* vertex_data = nullptr;
	unsigned int num_vertices = 0;
	unsigned int* indices = nullptr;
	unsigned int num_indices = 0;

	unsigned int vertex_buffer = 0;
	unsigned int element_buffer = 0;
	unsigned int instance_buffer = 0;
	unsigned int vertex_array = 0;

public:
	Mesh() {};
	~Mesh();

	// Draw mesh with default instance data values
	void draw();
	// Draw 'num_instances' meshes using the instance data array starting at 'instance_data'
	void draw(InstanceData* instance_data, size_t num_instances);
	void draw(std::vector<InstanceData> instances);
	// Allocate space on GPU for mesh and configure attributes
	void initialize();
	// Free space allocated on GPU for mesh
	void free();
	int getNumVertices() const;
	int getNumIndices() const;
	Vertex* getVertexData() const;
};

#include "Mesh.tpp"

#endif
