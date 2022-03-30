#pragma once

#include <string>
#include "cMesh.h"

class cDungeonMeshBuilder
{
public:
	enum class TypeOfMesh
	{
		FLOOR = 0,
		WALL = 1,
		DOOR = 2,
		SECRETDOOR = 3,
		PORTCULLIS = 4,
		STAIRS = 5,
	};

	cDungeonMeshBuilder();
	virtual ~cDungeonMeshBuilder();

	cMesh* MakeMesh(TypeOfMesh type, glm::vec3 scale);

protected:

private:

};