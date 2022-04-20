#include "cChairObject.h"

cChairObject::cChairObject(glm::vec3 startPosition, Node* startNode)
{
	this->type = iEntity::ENTITY_TYPE::OBJECT;
	this->m_Node = startNode;
	this->m_Node->isOccupied = true;
	this->m_Node->occupiedBy = (int)this->type;

	this->m_Mesh = new cMesh();
	this->m_Mesh->meshName = "dfk_chairs_XYZ_N_RGBA_UV.ply";		// lol, I just noticed that there's multiple chair models in this
	this->m_Mesh->positionXYZ = glm::vec3(startPosition.x, 0.0f, startPosition.z);
	// orientation outside
	this->m_Mesh->setUniformScale(1.0f);
	this->m_Mesh->bDontLight = false;
	this->m_Mesh->bUseStencil = true;
	this->m_Mesh->clearTextureRatiosToZero();
	this->m_Mesh->textureOperator = 4;
	this->m_Mesh->textureNames[1] = "dfk_chair_01_basecolor.bmp";
	this->m_Mesh->textureNames[2] = "dfk_chair_01_normal.bmp";
	this->m_Mesh->textureNames[3] = "dfk_chair_01_SmMetAO.bmp";
	this->m_Mesh->textureRatios[1] = 1.0f;
	this->m_Mesh->textureRatios[2] = 1.0f;
	this->m_Mesh->textureRatios[3] = 0.3f;

	this->m_LowResMesh = new cMesh();
	this->m_LowResMesh->meshName = "Isosphere_Smooth_Normals.ply";
	this->m_LowResMesh->positionXYZ = startPosition;
	this->m_LowResMesh->setUniformScale(1.0f);
	this->m_LowResMesh->bDontLight = false;
	this->m_LowResMesh->bUseStencil = true;
	this->m_LowResMesh->clearTextureRatiosToZero();
	this->m_LowResMesh->textureOperator = 1;
	this->m_LowResMesh->textureNames[1] = "dfk_chair_01_basecolor.bmp";
	this->m_Mesh->textureRatios[1] = 1.0f;
}
cChairObject::~cChairObject()
{

}

void cChairObject::Update(float deltaTime)
{

}