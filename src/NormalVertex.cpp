#include "NormalVertex.h"

#include "glad/glad.h"

void NormalVertex::setAttributes(unsigned int vertex_buffer)
{
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(NormalVertex), (void*)offsetof(NormalVertex, pos));
	glEnableVertexAttribArray(0);
	// normal vector
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(NormalVertex), (void*)offsetof(NormalVertex, normal));
	glEnableVertexAttribArray(1);
	// texture coordinates
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(NormalVertex), (void*)offsetof(NormalVertex, texCoords));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}