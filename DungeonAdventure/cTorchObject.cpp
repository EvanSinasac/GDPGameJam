#include "cTorchObject.h"
#include "globalThings.h"
#include <iostream>

cTorchObject::cTorchObject(cMesh* pMesh, unsigned int index)
	: m_Mesh(pMesh)
	, lightIndex(index)
{
	baseLightAtten = 0.4f;
	baseMeshAtten = 0.1f;
	timer = 0.0f;
}

cTorchObject::~cTorchObject()
{

}

void cTorchObject::Update(float deltaTime)
{
	timer += deltaTime;
	//std::cout << "timer: " << timer << std::endl;
	if (timer > 0.05f)
	{
		float randomAddedAttenLight = ((rand() % 100) / 100.0f + 1.0f) * gGetRandBetween<float>(1.0f, 2.0f);
		float randomAddedAttenMesh = randomAddedAttenLight / 5.0f;
		if (lightIndex < g_pTheLights->NUMBER_OF_LIGHTS)
		{
			::g_pTheLights->theLights[lightIndex].atten.y = baseLightAtten + randomAddedAttenLight;
		}
		this->m_Mesh->scale.z = baseMeshAtten + randomAddedAttenMesh;
		timer = 0.0f;
	}
	
	return;
}