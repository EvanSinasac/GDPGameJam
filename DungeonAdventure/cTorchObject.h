#pragma once

#include "iEntity.h"

#include "cMesh.h"

class cTorchObject : iEntity
{
public:
	cTorchObject(cMesh* pMesh, unsigned int index);
	~cTorchObject();

	virtual void Update(float deltaTime);

protected:

private:
	cMesh* m_Mesh;
	unsigned int lightIndex;

	float baseLightAtten;
	float baseMeshAtten;
	float timer;
};