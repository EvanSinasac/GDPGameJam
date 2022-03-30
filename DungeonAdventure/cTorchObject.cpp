#include "cTorchObject.h"
#include "globalThings.h"

cTorchObject::cTorchObject(cMesh* pMesh, unsigned int index)
	: m_Mesh(pMesh)
	, lightIndex(index)
{
	baseLightAtten = 0.4f;
	baseMeshAtten = 0.1f;
}

cTorchObject::~cTorchObject()
{

}

void cTorchObject::Update(float deltaTime)
{
	float randomAddedAtten = ((rand() % 100) / 100.0f + 1.0f) * gGetRandBetween<float>(1.0f, 2.0f);
	::g_pTheLights->theLights[lightIndex].atten.y = baseLightAtten + randomAddedAtten;
	this->m_Mesh->scale.z = baseMeshAtten+ randomAddedAtten;
	return;
}