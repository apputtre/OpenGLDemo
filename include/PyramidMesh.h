#ifndef PYRAMIDMESH_H
#define PYRAMIDMESH_H

#include "Mesh.h"

#include "NormalVertex.h"
#include "NormalInstanceData.h"

class PyramidMesh : public Mesh<NormalVertex, NormalInstanceData>
{
public:
	PyramidMesh();
};

#endif