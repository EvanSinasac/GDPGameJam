#include "cPlayerEntity.h"
#include <iostream>

cPlayerEntity::cPlayerEntity()
{
	this->position = glm::vec3(0.0f);
	this->lookAt = vec3NORTH;
	this->direction = (int)DIRECTIONS::NORTH;

	this->m_CurrNode = NULL;

	this->m_Mesh = new cMesh();
	//this->m_Mesh->meshName = "dalek2005_XYZ_N_RGBA_UV_hi_res.ply";
	this->m_Mesh->meshName = "hollowknight_XYZ_N_RGBA_UV.ply";
	this->m_Mesh->positionXYZ = this->position;
	this->m_Mesh->orientationXYZ = glm::vec3(0.0f);
	//this->m_Mesh->setUniformScale(0.03f);
	this->m_Mesh->setUniformScale(0.4f);
	this->m_Mesh->bDontLight = false;
	this->m_Mesh->friendlyName = "Player";
	this->m_Mesh->clearTextureRatiosToZero();
	this->m_Mesh->textureNames[1] = "uv_hollow.bmp";
	this->m_Mesh->textureRatios[1] = 1.0f;


	type = ENTITY_TYPE::PLAYER;
}
cPlayerEntity::cPlayerEntity(glm::vec3 startPos, glm::vec3 startLookAt, Node* startNode)
	: position(startPos)
	, lookAt(startLookAt)
	, m_CurrNode(startNode)
{
	this->m_Mesh = new cMesh();
	//this->m_Mesh->meshName = "dalek2005_XYZ_N_RGBA_UV_hi_res.ply";
	this->m_Mesh->meshName = "hollowknight_XYZ_N_RGBA_UV.ply";
	this->m_Mesh->positionXYZ = this->position;
	// orientation in lookAt
	//this->m_Mesh->setUniformScale(0.03f);
	this->m_Mesh->setUniformScale(0.4f);
	this->m_Mesh->bDontLight = true;
	this->m_Mesh->friendlyName = "Player";
	this->m_Mesh->clearTextureRatiosToZero();
	this->m_Mesh->textureNames[1] = "uv_hollow.bmp";
	this->m_Mesh->textureRatios[1] = 1.0f;

	this->m_Mesh->bUseWholeObjectDiffuseColour = false;
	this->m_Mesh->wholeObjectDiffuseRGBA = glm::vec4(0.6f, 0.2f, 0.6f, 1.0f);

	if (lookAt == vec3NORTH)
	{
		this->direction = (int)DIRECTIONS::NORTH;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	else if (lookAt == vec3NORTH_EAST)
	{
		this->direction = (int)DIRECTIONS::NORTH_EAST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(45.0f), 0.0f);
	}
	else if (lookAt == vec3EAST)
	{
		this->direction = (int)DIRECTIONS::EAST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(90.0f), 0.0f);
	}
	else if (lookAt == vec3SOUTH_EAST)
	{
		this->direction = (int)DIRECTIONS::SOUTH_EAST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(135.0f), 0.0f);
	}
	else if (lookAt == vec3SOUTH)
	{
		this->direction = (int)DIRECTIONS::SOUTH;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(180.0f), 0.0f);
	}
	else if (lookAt == vec3SOUTH_WEST)
	{
		this->direction = (int)DIRECTIONS::SOUTH_WEST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(225.0f), 0.0f);
	}
	else if (lookAt == vec3WEST)
	{
		this->direction = (int)DIRECTIONS::WEST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(270.0f), 0.0f);
	}
	else if (lookAt == vec3NORTH_WEST)
	{
		this->direction = (int)DIRECTIONS::NORTH_WEST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(315.0f), 0.0f);
	}
	else 
	{
		this->direction = (int)DIRECTIONS::NORTH;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(45.0f), 0.0f);
	}

	
	type = ENTITY_TYPE::PLAYER;
}
cPlayerEntity::~cPlayerEntity()
{

}

void cPlayerEntity::Update(float dt)
{
	this->m_Mesh->positionXYZ = this->position;
}

void cPlayerEntity::Move(std::string directionToMove)
{
	if (directionToMove == "FORWARD")
	{
		for (pair<Node*, float>& neighbour : this->m_CurrNode->edges)
		{
			glm::vec3 pos = glm::vec3(neighbour.first->position.x, neighbour.first->position.y, neighbour.first->position.z);
			if (glm::distance(pos, this->position + this->lookAt) <= 0.75f)
			{
				if (neighbour.first->type != "-")
				{
					this->position = glm::vec3(pos.x, this->position.y, pos.z);
					this->m_CurrNode = neighbour.first;
				}
				break;
			}
		}
	}
	else if (directionToMove == "BACKWARDS")
	{
		for (pair<Node*, float>& neighbour : this->m_CurrNode->edges)
		{
			glm::vec3 pos = glm::vec3(neighbour.first->position.x, neighbour.first->position.y, neighbour.first->position.z);
			if (glm::distance(pos, this->position - this->lookAt) <= 0.75f)
			{
				if (neighbour.first->type != "-")
				{
					this->position = glm::vec3(pos.x, this->position.y, pos.z);
					this->m_CurrNode = neighbour.first;
				}
				break;
			}
		}
	}
	else
	{
		std::cout << "directionToMove is wrong!" << std::endl;
	}
	
}
void cPlayerEntity::Rotate(std::string directionToRotate)
{
	if (directionToRotate == "LEFT")
	{
		direction++;
		if (direction > (int)DIRECTIONS::NORTH_WEST)
		{
			direction = (int)DIRECTIONS::NORTH;
		}
	}
	else if (directionToRotate == "RIGHT")
	{
		direction--;
		if (direction < (int)DIRECTIONS::NORTH)
		{
			direction = (int)DIRECTIONS::NORTH_WEST;
		}
	}
	else
	{
		std::cout << "NOT A VALID DIRECTION" << std::endl;
	}

	switch (direction)
	{
	case (int)DIRECTIONS::NORTH:
		this->lookAt = vec3NORTH;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
		break;
	case(int)DIRECTIONS::NORTH_EAST:
		this->lookAt = vec3NORTH_EAST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-45.0f), 0.0f);
		break;
	case(int)DIRECTIONS::EAST:
		this->lookAt = vec3EAST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-90.0f), 0.0f);
		break;
	case(int)DIRECTIONS::SOUTH_EAST:
		this->lookAt = vec3SOUTH_EAST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-135.0f), 0.0f);
		break;
	case(int)DIRECTIONS::SOUTH:
		this->lookAt = vec3SOUTH;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-180.0f), 0.0f);
		break;
	case(int)DIRECTIONS::SOUTH_WEST:
		this->lookAt = vec3SOUTH_WEST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-225.0f), 0.0f);
		break;
	case(int)DIRECTIONS::WEST:
		this->lookAt = vec3WEST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-270.0f), 0.0f);
		break;
	case(int)DIRECTIONS::NORTH_WEST:
		this->lookAt = vec3NORTH_WEST;
		this->m_Mesh->orientationXYZ = glm::vec3(0.0f, glm::radians(-315.0f), 0.0f);
		break;
	default:
		std::cout << "Direction is somehow invalid (the switch statement didn't work)" << std::endl;
		break;
	}
}