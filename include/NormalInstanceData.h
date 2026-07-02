#ifndef NORMALINSTANCEDATA_H
#define NORMALINSTANCEDATA_H

#include "SimpleInstanceData.h"
#include "linalg_types.h"

/*
Instance data containing a model matrix and material properties.
Intended for use with NormalVertex.
*/
class NormalInstanceData : public InstanceDataBase
{
public:
	NormalInstanceData()
		: model{ mat4(1) }
	{}

	static void setAttributes(unsigned int normal_buffer);

	float shininess = 0;
	mat4 model;
};

#endif
