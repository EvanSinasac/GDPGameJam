#pragma once

#include "iEntity.h"
#include "GLMCommon.h"
#include "cMesh.h"
#include "Graph.h"

static const glm::vec3 vec3NORTH = glm::vec3(0.0f, 0.0f, 2.5f);
static const glm::vec3 vec3NORTH_EAST = glm::vec3(-2.5f, 0.0f, 2.5f);
static const glm::vec3 vec3EAST = glm::vec3(-2.5f, 0.0f, 0.0f);
static const glm::vec3 vec3SOUTH_EAST = glm::vec3(-2.5f, 0.0f, -2.5f);
static const glm::vec3 vec3SOUTH = glm::vec3(0.0f, 0.0f, -2.5f);
static const glm::vec3 vec3SOUTH_WEST = glm::vec3(2.5f, 0.0f, -2.5f);
static const glm::vec3 vec3WEST = glm::vec3(2.5f, 0.0f, 0.0f);
static const glm::vec3 vec3NORTH_WEST = glm::vec3(2.5f, 0.0f, 2.5f);

class cPlayerEntity : public iEntity
{
public:
	cPlayerEntity();
	cPlayerEntity(glm::vec3 startPos, glm::vec3 startLookAt, Node* startNode);
	virtual ~cPlayerEntity();

	virtual void Update(float dt);

	void Move(std::string directionToMove);
	void Rotate(std::string directionToRotate);

	enum class DIRECTIONS : int
	{
		NORTH = 0,
		NORTH_EAST = 1,
		EAST = 2,
		SOUTH_EAST = 3,
		SOUTH = 4,
		SOUTH_WEST = 5,
		WEST = 6,
		NORTH_WEST = 7,
	};

	cMesh* m_Mesh;

	glm::vec3 position;
	glm::vec3 lookAt;
protected:

private:
	
	//DIRECTIONS direction;
	int direction;

	Node* m_CurrNode;
};