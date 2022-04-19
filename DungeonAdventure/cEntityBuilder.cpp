#include "cEntityBuilder.h"
#include "cWanderEnemy.h"

cEntityBuilder::cEntityBuilder()
{

}
cEntityBuilder::~cEntityBuilder()
{

}

iEntity* cEntityBuilder::MakeEntity(TypeOfEntity type, glm::vec3 startPos, Node* node)
{
	iEntity* newEntity = nullptr;
	if (type == TypeOfEntity::PLAYER)
	{
		// haha, not using this
	}
	else if (type == TypeOfEntity::WANDER_ENEMY)
	{
		newEntity = new cWanderEnemy(startPos, vec3NORTH, node);
	}

	return newEntity;
}