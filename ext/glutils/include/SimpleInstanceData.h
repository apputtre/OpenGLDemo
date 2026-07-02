#ifndef DEFAULTMESHINSTANCEDATA_H
#define DEFAULTMESHINSTANCEDATA_H

#include "InstanceDataBase.h"
#include "linalg_types.h"

class SimpleInstanceData : public InstanceDataBase
{
public:
	mat4 model;
	vec4 color;

	SimpleInstanceData()
		: model(1.0f), color(1.0f)
	{}

	static void setAttributes(unsigned int instance_buffer);
};

#endif DEFAULTMESHINSTANCEDATA_H
