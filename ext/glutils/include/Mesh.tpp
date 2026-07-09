#include "Mesh.h"

#include <glad/glad.h>
#include <iostream>
#include <format>

template<std::derived_from<VertexBase> T, std::derived_from<InstanceDataBase> U>
Mesh<T, U>::~Mesh()
{
	free();

	delete(vertex_data);
	delete(indices);
}

template<std::derived_from<VertexBase> T, std::derived_from<InstanceDataBase> U>
void Mesh<T, U>::initialize()
{
	if (initialized)
		return;

	// create the buffers
	glGenBuffers(1, &vertex_buffer);
	glGenBuffers(1, &element_buffer);
	glGenBuffers(1, &instance_buffer);
	glGenVertexArrays(1, &vertex_array);

	// send over the vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * num_vertices, vertex_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// send over the index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * num_indices, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	// begin configuring attributes
	glBindVertexArray(vertex_array);

	InstanceData::setAttributes(instance_buffer);
	Vertex::setAttributes(vertex_buffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);

	glBindVertexArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	initialized = true;
}

template<std::derived_from<VertexBase> T, std::derived_from<InstanceDataBase> U>
void Mesh<T, U>::free()
{
	if (initialized)
	{
		glDeleteBuffers(1, &vertex_buffer);
		glDeleteBuffers(1, &element_buffer);
		glDeleteBuffers(1, &instance_buffer);
		glDeleteVertexArrays(1, &vertex_array);
	}

	initialized = false;
}

template<std::derived_from<VertexBase> T, std::derived_from<InstanceDataBase> U>
int Mesh<T, U>::getNumVertices() const
{
	return num_vertices;
}

template<std::derived_from<VertexBase> T, std::derived_from<InstanceDataBase> U>
int Mesh<T, U>::getNumIndices() const
{
	return num_indices;
}

template<std::derived_from<VertexBase> T, std::derived_from<InstanceDataBase> U>
void Mesh<T, U>::draw()
{
	InstanceData idata;

	draw(&idata, 1);
}

template<std::derived_from<VertexBase> T, std::derived_from<InstanceDataBase> U>
void Mesh<T, U>::draw(InstanceData* instance_data, size_t num_instances)
{
	glBindBuffer(GL_ARRAY_BUFFER, instance_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceData) * num_instances, instance_data, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(vertex_array);
	glDrawElementsInstanced(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, 0, num_instances);
	glBindVertexArray(0);
}

template<std::derived_from<VertexBase> T, std::derived_from<InstanceDataBase> U>
void Mesh<T, U>::draw(std::vector<InstanceData> instances)
{
	draw(instances.data(), instances.size());
}

template<std::derived_from<VertexBase> T, std::derived_from<InstanceDataBase> U>
Mesh<T, U>::Vertex* Mesh<T, U>::getVertexData() const
{
	return vertex_data;
}