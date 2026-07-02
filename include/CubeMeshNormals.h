#ifndef CUBEMESHNORMALS_H
#define CUBEMESHNORMALS_H 

#include <Mesh.h>

#include "NormalVertex.h"
#include "NormalInstanceData.h"

class CubeMeshNormals : public Mesh<NormalVertex, NormalInstanceData>
{
public:
	CubeMeshNormals();
};

#endif
