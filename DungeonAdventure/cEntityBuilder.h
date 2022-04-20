#pragma once

#include <string>
#include "iEntity.h"
#include "Graph.h"

class cEntityBuilder
{
public:
	enum class TypeOfEntity
	{
		PLAYER = 0,
		WANDER_ENEMY = 1,
		LISTEN_ENEMY = 2,
	};

	cEntityBuilder();
	virtual ~cEntityBuilder();

	iEntity* MakeEntity(TypeOfEntity type, glm::vec3 startPos, Node* node);
protected:

private:

};