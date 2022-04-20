#include "cTorchObject.h"
#include "globalThings.h"
#include <iostream>

cTorchObject::cTorchObject(cMesh* pMesh, unsigned int index)
	: lightIndex(index)
{
	baseLightAtten = 0.4f;
	baseLightAtten = 0.08f;
	baseMeshAtten = 0.75f;
	timer = 0.0f;

	type = ENTITY_TYPE::TORCH;
	this->m_Mesh = pMesh;

	//this->m_HiResMesh = pMesh;

	this->m_LowResMesh = new cMesh();
	this->m_LowResMesh->meshName = "Isosphere_Smooth_Normals.ply";
	this->m_LowResMesh->positionXYZ = pMesh->positionXYZ;
	this->m_LowResMesh->bDontLight = true;
	this->m_LowResMesh->bIsWireframe = false;
	this->m_LowResMesh->bUseWholeObjectDiffuseColour = true;
	this->m_LowResMesh->wholeObjectDiffuseRGBA = glm::vec4(0.3f, 0.0f, 0.0f, 1.0f);
	this->m_LowResMesh->setUniformScale(0.1f);
	this->m_LowResMesh->clearTextureRatiosToZero();
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
		//float randomAddedAttenLight = ((rand() % 100) / 100.0f + 1.0f) * gGetRandBetween<float>(1.0f, 2.0f);
		float randomAddedAttenLight = ((rand() % 10) + 1.0f) / 100.0f * gGetRandBetween<float>(1.0f, 2.0f);
		float randomAddedAttenMesh = randomAddedAttenLight / 5.0f;
		if (lightIndex < g_pTheLights->NUMBER_OF_LIGHTS)
		{
			::g_pTheLights->theLights[lightIndex].atten.y = baseLightAtten + randomAddedAttenLight;
		}
		this->m_Mesh->vec_pChildMeshes[0]->vec_pChildMeshes[0]->scale.z = baseMeshAtten + randomAddedAttenMesh * 3.0f;
		//this->m_Mesh->scale.z = baseMeshAtten + randomAddedAttenMesh;
		timer = 0.0f;
	}
	
	return;
}