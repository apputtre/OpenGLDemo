#include "SimpleInstanceData.h"

#include <glad/glad.h>
#include "linalg_types.h"

void SimpleInstanceData::setAttributes(unsigned int instance_buffer)
{
	// instance data
	glBindBuffer(GL_ARRAY_BUFFER, instance_buffer);

	// color
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(SimpleInstanceData), (void*)offsetof(SimpleInstanceData, color));
	glVertexAttribDivisor(1, 1);
	glEnableVertexAttribArray(1);

	// 1st column of model matrix
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(SimpleInstanceData), (void*)offsetof(SimpleInstanceData, model));
	glVertexAttribDivisor(2, 1);
	glEnableVertexAttribArray(2);
	// 2nd column of model matrix
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(SimpleInstanceData), (void*)(offsetof(SimpleInstanceData, model) + sizeof(vec4)));
	glVertexAttribDivisor(3, 1);
	glEnableVertexAttribArray(3);
	// 3rd column of model matrix
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(SimpleInstanceData), (void*)(offsetof(SimpleInstanceData, model) + 2*sizeof(vec4)));
	glVertexAttribDivisor(4, 1);
	glEnableVertexAttribArray(4);
	// 4th column of model matrix
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(SimpleInstanceData), (void*)(offsetof(SimpleInstanceData, model) + 3*sizeof(vec4)));
	glVertexAttribDivisor(5, 1);
	glEnableVertexAttribArray(5);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
