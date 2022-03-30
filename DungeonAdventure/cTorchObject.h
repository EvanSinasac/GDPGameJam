#pragma once

#include "cMesh.h"

class cTorchObject
{
public:
	cTorchObject(cMesh* pMesh, unsigned int index);
	~cTorchObject();

	void Update(float deltaTime);

protected:

private:
	cMesh* m_Mesh;
	unsigned int lightIndex;

	float baseLightAtten;
	float baseMeshAtten;
};