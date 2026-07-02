#include "NormalInstanceData.h"

#include "glad/glad.h"
#include "linalg_types.h"
#include "glutils.h"

void NormalInstanceData::setAttributes(unsigned int instance_buffer)
{
	glBindBuffer(GL_ARRAY_BUFFER, instance_buffer);

	// shininess material property
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(NormalInstanceData), (void*)(offsetof(NormalInstanceData, shininess)));
	glVertexAttribDivisor(3, 1);
	glEnableVertexAttribArray(3);
	// 1st column of model matrix
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(NormalInstanceData), (void*)offsetof(NormalInstanceData, model));
	glVertexAttribDivisor(4, 1);
	glEnableVertexAttribArray(4);
	// 2nd column of model matrix
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(NormalInstanceData), (void*)(offsetof(NormalInstanceData, model) + sizeof(vec4)));
	glVertexAttribDivisor(5, 1);
	glEnableVertexAttribArray(5);
	// 3rd column of model matrix
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(NormalInstanceData), (void*)(offsetof(NormalInstanceData, model) + 2*sizeof(vec4)));
	glVertexAttribDivisor(6, 1);
	glEnableVertexAttribArray(6);
	// 4th column of model matrix
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(NormalInstanceData), (void*)(offsetof(NormalInstanceData, model) + 3*sizeof(vec4)));
	glVertexAttribDivisor(7, 1);
	glEnableVertexAttribArray(7);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
