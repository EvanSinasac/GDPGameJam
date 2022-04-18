#include "globalThings.h"
#include "cDungeonMeshBuilder.h"
#include <sstream>
#include <iostream>

#include "cPlayerEntity.h"

void MakeNodeMeshes();

void MakeFSMEntities(std::vector<glm::vec3> spawnPoints)
{
	for (unsigned int index = 0; index != spawnPoints.size(); index++)
	{
		cFSMEntity* tempEntity = new cFSMEntity(::g_Graph, spawnPoints[index]);
		vec_pFSMEntities.push_back(tempEntity);
	}
}

bool loadTSVGrid()
{
	cDungeonMeshBuilder main_DungeonBuilder;
	float scale = 1.0f;			// not sure if I need this, I don't think I do
	std::stringstream ss;
	ss << SOLUTION_DIR << "common\\assets\\The Catacombs of Horrendous Devastation 01 - Converted.tsv";
	std::ifstream theFile(ss.str());

	std::string grid[51][65];

	if (!theFile.is_open())
	{
		std::cout << "The tsv file didn't open!" << std::endl;
	}
	std::string nextLetter;
	for (unsigned int y = 0; y < 65; y++)
	{
		for (unsigned int x = 0; x < 51; x++)
		{
			nextLetter = theFile.get();
			grid[x][y] = nextLetter;
			if (nextLetter == "D")
			{
				nextLetter = theFile.get();
				if (nextLetter == "P" || nextLetter == "S")
				{
					grid[x][y].append(nextLetter);
				}
				else
				{
					//std::cout << grid[x][y] << " ";
					x++;
					grid[x][y] = nextLetter;
				}
			}
			else if (nextLetter == "S")
			{
				nextLetter = theFile.get();
				if (nextLetter == "D" || nextLetter == "U")
				{
					grid[x][y].append(nextLetter);
					nextLetter = theFile.get();
					if (nextLetter == "D" || nextLetter == "U")
					{
						grid[x][y].append(nextLetter);
					}
					else
					{
						//std::cout << grid[x][y] << " ";
						x++;
						grid[x][y] = nextLetter;
					}
				}
				else
				{
					//std::cout << grid[x][y] << " ";
					x++;
					grid[x][y] = nextLetter;
				}
			}
			//std::cout << grid[x][y] << " ";
		}	//end of for x
		// Newline
		//std::cout << std::endl;
		theFile.get();
	}	// end of for y

	theFile.close();

	// D - door
	// F - floor
	// DP - portcullis door
	// DS - secret door
	// SD/SDD - down stairs
	// SU/SUU - up stairs
	// - - wall

	//cDungeonMeshBuilder dungeonBuilder;

	// TODO: Either in these for loops or do it again, make the graph and nodes
	// 
	// Choose the mesh and location depending on the x/y values of the grid
	unsigned int nodeID = 0;
	for (unsigned int y = 0; y < 65; y++)
	{
		for (unsigned int x = 0; x < 51; x++)
		{
			cMesh* newMesh;

			// This is where the main changes from the Graphics Final start, instead of using the Dwarf stuff
			// I'm using the Dark Fantasy Kit
			// (plus I'll be adding a graph for enemies to move through the dungeon)


			// First, check what kind of model we need, depends on the letter stored in the grid
			if (grid[x][y] == "-")
			{
				// skip, we'll do the walls depending on the floor
				::g_Graph->CreateNode(nodeID, glm::vec3(0.0f + (2.5f * x), 0.0f + 0.75f, 0.0f + (2.5f * y)),
					grid[x][y], false, false, false);
				nodeID++;
			}
			else if (grid[x][y] == "D")
			{
				// We need to check what the orientation of the door is
				//newMesh = new cMesh();
				// Made a builder that does the nitty gritty for me, still have to set position and stuff of the floor itself afterwards

				newMesh = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::FLOOR, glm::vec3(1.0f));

				// Position
				newMesh->positionXYZ.x = 0.0f + (2.5f * x);
				newMesh->positionXYZ.y = 0.0f;
				newMesh->positionXYZ.z = 0.0f + (2.5f * y);

				cMesh* door = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::DOOR, glm::vec3(scale));
				newMesh->vec_pChildMeshes.push_back(door);

				// Check where the walls of the floor are
				if (grid[x - 1][y] == "-")	// East or right
				{
					cMesh* eastWall = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(scale));
					eastWall->positionXYZ = glm::vec3(-1.2f, 0.0f, 0.0f);
					eastWall->orientationXYZ = glm::vec3(0.0f, glm::radians(90.0f), 0.0f);
					newMesh->vec_pChildMeshes.push_back(eastWall);
				}
				if (grid[x + 1][y] == "-")	// West or left
				{
					cMesh* westWall = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(scale));
					westWall->positionXYZ = glm::vec3(1.2f, 0.0f, 0.0f);
					westWall->orientationXYZ = glm::vec3(0.0f, glm::radians(270.0f), 0.0f);
					newMesh->vec_pChildMeshes.push_back(westWall);
				}
				if (grid[x][y - 1] == "-")	// South or backwards
				{
					cMesh* southWall = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(scale));
					southWall->positionXYZ = glm::vec3(0.0f, 0.0f, -1.2f);
					southWall->orientationXYZ = glm::vec3(0.0f);				// I think these walls are double sided with normals so I shouldn't need to worry about what direction it's facing
					newMesh->vec_pChildMeshes.push_back(southWall);
				}
				if (grid[x][y + 1] == "-")	// North or forwards
				{
					cMesh* northWall = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(scale));
					northWall->positionXYZ = glm::vec3(0.0f, 0.0f, 1.2f);
					northWall->orientationXYZ = glm::vec3(0.0f, glm::radians(180.0f), 0.0f);				// I think these walls are double sided with normals so I shouldn't need to worry about what direction it's facing
					newMesh->vec_pChildMeshes.push_back(northWall);
				}

				// Check up/down and left/right, doors should only be in one orientation or the other
				if (grid[x - 1][y] == "F")	// if the left is floor, the right should also be floor
				{
					// so we need ot rotate the door frame ONLY
					newMesh->vec_pChildMeshes[0]->orientationXYZ += glm::vec3(0.0f, glm::radians(90.0f), 0.0f);
				}	// otherwise up/down is floor and we don't need to rotate


				::g_vec_pMeshes.push_back(newMesh);


				//newNode = new Node();
				//newNode->id = index;
				//index++;
				//newNode->hasGoal = false;
				//newNode->isHomeBase = false;
				//newNode->isExit = false;
				//newNode->position = newMesh->positionXYZ;
				//newNode->type = (char)grid[x][y].c_str();

				::g_Graph->CreateNode(nodeID, glm::vec3(newMesh->positionXYZ.x, newMesh->positionXYZ.y + 0.75f, newMesh->positionXYZ.z),
					grid[x][y], false, false, false);
				nodeID++;
			}	// end of Door logic
			else if (grid[x][y] == "DS" || grid[x][y] == "DP")		// TODO: DP are portcullis, for now they'll be treated the same as the secret doors
			{
				// Secret doors are just going to use a different model for the door itself, otherwise I think it's going to be the same logic as the regular door
				newMesh = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::FLOOR, glm::vec3(scale));

				// Position
				newMesh->positionXYZ.x = 0.0f + (2.5f * x);
				newMesh->positionXYZ.y = 0.0f;
				newMesh->positionXYZ.z = 0.0f + (2.5f * y);

				cMesh* gateMesh;
				if (grid[x][y] == "DS")	// in case I ever actually do make them different
				{
					gateMesh = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::SECRETDOOR, glm::vec3(1.0f));
				}
				else
				{
					gateMesh = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::PORTCULLIS, glm::vec3(1.0f));
				}

				newMesh->vec_pChildMeshes.push_back(gateMesh);

				// Check where the walls of the floor are
				if (grid[x - 1][y] == "-")	// East or right
				{
					cMesh* eastWall = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(1.0f));
					eastWall->positionXYZ = glm::vec3(-1.2f, 0.0f, 0.0f);
					eastWall->orientationXYZ = glm::vec3(0.0f, glm::radians(90.0f), 0.0f);
					newMesh->vec_pChildMeshes.push_back(eastWall);
				}
				if (grid[x + 1][y] == "-")	// West or left
				{
					cMesh* westWall = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(1.0f));
					westWall->positionXYZ = glm::vec3(1.2f, 0.0f, 0.0f);
					westWall->orientationXYZ = glm::vec3(0.0f, glm::radians(270.0f), 0.0f);
					newMesh->vec_pChildMeshes.push_back(westWall);
				}
				if (grid[x][y - 1] == "-")	// South or backwards
				{
					cMesh* southWall = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(1.0f));
					southWall->positionXYZ = glm::vec3(0.0f, 0.0f, -1.2f);
					southWall->orientationXYZ = glm::vec3(0.0f);				// I think these walls are double sided with normals so I shouldn't need to worry about what direction it's facing
					newMesh->vec_pChildMeshes.push_back(southWall);
				}
				if (grid[x][y + 1] == "-")	// North or forwards
				{
					cMesh* northWall = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(1.0f));
					northWall->positionXYZ = glm::vec3(0.0f, 0.0f, 1.2f);
					northWall->orientationXYZ = glm::vec3(0.0f, glm::radians(180.0f), 0.0f);				// I think these walls are double sided with normals so I shouldn't need to worry about what direction it's facing
					newMesh->vec_pChildMeshes.push_back(northWall);
				}

				// Check up/down and left/right, doors should only be in one orientation or the other
				if (grid[x - 1][y] == "F")	// if the left is floor, the right should also be floor
				{
					// so we need to rotate the door frame ONLY
					newMesh->vec_pChildMeshes[0]->orientationXYZ += glm::vec3(0.0f, glm::radians(90.0f), 0.0f);
				}	// otherwise up/down is floor and we don't need to rotate

				::g_vec_pMeshes.push_back(newMesh);


				//newNode = new Node();
				//newNode->id = index;
				//index++;
				//newNode->hasGoal = false;
				//newNode->isHomeBase = false;
				//newNode->isExit = false;
				//newNode->position = newMesh->positionXYZ;
				//newNode->type = (char)grid[x][y].c_str();

				::g_Graph->CreateNode(nodeID, glm::vec3(newMesh->positionXYZ.x, newMesh->positionXYZ.y + 0.75f, newMesh->positionXYZ.z),
					grid[x][y], false, false, false);
				nodeID++;
			}	// end of DS || DP
			else if (grid[x][y] == "SD"
				|| grid[x][y] == "SDD"
				|| grid[x][y] == "SU"
				|| grid[x][y] == "SUU")	// Stairs, logic only needs to be separated for the y position, so let's just wrap the y value
			{

				newMesh = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::STAIRS, glm::vec3(scale));
				newMesh->positionXYZ.x = 0.0f + (2.5f * x);
				if (grid[x][y] == "SD" || grid[x][y] == "SDD")
				{
					newMesh->positionXYZ.y = 0.0f - 1.4f;
					newMesh->positionXYZ.z = 0.0f + (2.5f * y) + 1.0f;
					newMesh->orientationXYZ = glm::vec3(0.0f);
					if (grid[x][y] == "SDD")
					{
						newMesh->positionXYZ.y -= 1.5f;
					}
				}
				else if (grid[x][y] == "SU" || grid[x][y] == "SUU")
				{
					newMesh->positionXYZ.y = 0.0f;
					newMesh->positionXYZ.z = 0.0f + (2.5f * y) + 1.0f;
					newMesh->orientationXYZ = glm::vec3(0.0f);
					if (grid[x][y] == "SUU")
					{
						newMesh->positionXYZ.y += 1.5;
					}
				}
				::g_vec_pMeshes.push_back(newMesh);
			}	// end of stairs
			else if (grid[x][y] == "F")		// regular floor, check and place walls if necessarry
			{
				newMesh = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::FLOOR, glm::vec3(scale));

				if (grid[x - 1][y] == "-")	// East or right
				{
					cMesh* eastWall = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(1.0f));
					eastWall->positionXYZ = glm::vec3(-1.2f, 0.0f, 0.0f);
					eastWall->orientationXYZ = glm::vec3(0.0f, glm::radians(90.0f), 0.0f);
					newMesh->vec_pChildMeshes.push_back(eastWall);
				}
				if (grid[x + 1][y] == "-")	// West or left
				{
					cMesh* westWall = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(1.0f));
					westWall->positionXYZ = glm::vec3(1.2f, 0.0f, 0.0f);
					westWall->orientationXYZ = glm::vec3(0.0f, glm::radians(270.0f), 0.0f);
					newMesh->vec_pChildMeshes.push_back(westWall);
				}
				if (grid[x][y - 1] == "-")	// South or backwards
				{
					cMesh* southWall = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(1.0f));
					southWall->positionXYZ = glm::vec3(0.0f, 0.0f, -1.2f);
					southWall->orientationXYZ = glm::vec3(0.0f);				// I think these walls are double sided with normals so I shouldn't need to worry about what direction it's facing
					newMesh->vec_pChildMeshes.push_back(southWall);
				}
				if (grid[x][y + 1] == "-")	// North or forwards
				{
					cMesh* northWall = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(1.0f));
					northWall->positionXYZ = glm::vec3(0.0f, 0.0f, 1.2f);
					northWall->orientationXYZ = glm::vec3(0.0f, glm::radians(180.0f), 0.0f);				// I think these walls are double sided with normals so I shouldn't need to worry about what direction it's facing
					newMesh->vec_pChildMeshes.push_back(northWall);
				}

				newMesh->positionXYZ.x = 0.0f + (2.5f * x);
				newMesh->positionXYZ.y = 0.0f;
				newMesh->positionXYZ.z = 0.0f + (2.5f * y);
				newMesh->orientationXYZ = glm::vec3(0.0f);

				// temp erase
				//newMesh->orientationXYZ.z = glm::radians(45.0f);

				::g_vec_pMeshes.push_back(newMesh);

				//newNode = new Node();
				//newNode->id = index;
				//index++;
				//newNode->hasGoal = false;
				//newNode->isHomeBase = false;
				//newNode->isExit = false;
				//newNode->position = newMesh->positionXYZ;
				//newNode->type = (char)grid[x][y].c_str();
				if (grid[x][y + 1] == "SD" || grid[x][y - 1] == "SD")
				{
					// SD is the stairs down, deeper into the dungeon
					//newNode->isExit = true;
					::g_Graph->CreateNode(nodeID, glm::vec3(newMesh->positionXYZ.x, newMesh->positionXYZ.y + 0.75f, newMesh->positionXYZ.z),
						grid[x][y], false, false, true);
				}
				else if (grid[x][y + 1] == "SU" || grid[x][y - 1] == "SU")
				{
					// SU is the stairs up, out of the dungeon
					//newNode->isHomeBase = true;
					::g_Graph->CreateNode(nodeID, glm::vec3(newMesh->positionXYZ.x, newMesh->positionXYZ.y + 0.75f, newMesh->positionXYZ.z),
						grid[x][y], true, false, false);

					::g_pPlayer = new cPlayerEntity(newMesh->positionXYZ, vec3NORTH, ::g_Graph->nodes[::g_Graph->nodes.size() - 1]);
					::vec_pAllEntities.push_back(::g_pPlayer);
				}
				else
				{
					::g_Graph->CreateNode(nodeID, glm::vec3(newMesh->positionXYZ.x, newMesh->positionXYZ.y + 0.75f, newMesh->positionXYZ.z),
						grid[x][y], false, false, false);
				}
				//else if (grid[x][y] == "FS")
				//{

				//}

				nodeID++;
			}	// end of floor
		}	// end of for x
	}	// end of for y

	// alright, the graph is filled out, let's make edges
	for (unsigned int indexA = 0; indexA != ::g_Graph->nodes.size() - 1; indexA++)
	{
		glm::vec3 posA = glm::vec3(::g_Graph->nodes[indexA]->position.x,
			::g_Graph->nodes[indexA]->position.y,
			::g_Graph->nodes[indexA]->position.z);

		// if the node at indexA is not traversable, then we don't want to make any edges out of it anyways 
		//if (::g_Graph->nodes[indexA]->type != '_' && ::g_Graph->nodes[indexA]->type != 'x')
		if (::g_Graph->nodes[indexA]->type != "-")	// tsv file, - is wall
		{
			for (unsigned int indexB = indexA + 1; indexB != ::g_Graph->nodes.size(); indexB++)
			{
				glm::vec3 posB = glm::vec3(::g_Graph->nodes[indexB]->position.x,
					::g_Graph->nodes[indexB]->position.y,
					::g_Graph->nodes[indexB]->position.z);

				if (glm::distance(posA, posB) <= 2.5f)
				{
					// there are no nodes in the walls or non-traversable locations, so all horizontal and vertical 
					// nodes are safe to traverse
					if (::g_Graph->nodes[indexB]->type != "-")
					{
						// node is white, red, green, blue or yellow, make an edge
						::g_Graph->AddEdge(::g_Graph->nodes[indexA],
							::g_Graph->nodes[indexB],
							10.0f,
							true);
					}
					else
					{
						::g_Graph->AddEdge(::g_Graph->nodes[indexA],
							::g_Graph->nodes[indexB],
							10000000.0f,
							true);
					}
				}
			} //end of for indexB
		} //end of if indexA is wall
	} //end of for indexA

	// all the horizontal and vertical nodes are made
	// second loop to handle diagonals
	for (unsigned int indexA = 0; indexA != ::g_Graph->nodes.size(); indexA++)
	{
		glm::vec3 posA = glm::vec3(::g_Graph->nodes[indexA]->position.x,
			::g_Graph->nodes[indexA]->position.y,
			::g_Graph->nodes[indexA]->position.z);

		// if the node at indexA is not traversable, then we don't want to make any edges out of it anyways 
		//if (::g_Graph->nodes[indexA]->type != '_' && ::g_Graph->nodes[indexA]->type != 'x')
		if (::g_Graph->nodes[indexA]->type != "-")	// tsv file, - is wall
		{
			for (unsigned int indexB = indexA + 1; indexB != ::g_Graph->nodes.size(); indexB++)
			{
				glm::vec3 posB = glm::vec3(::g_Graph->nodes[indexB]->position.x,
					::g_Graph->nodes[indexB]->position.y,
					::g_Graph->nodes[indexB]->position.z);

				// this isn't part of the else if so it's just re-adding
				if (glm::distance(posA, posB) <= 3.6f && glm::distance(posA, posB) > 2.5f)
				{
					// whoops, just because we can go diagonal, doesn't mean we should
					// wait... if I don't make nodes in the walls, I can't check the "nodes" above and below
					// a current spot for the diagonals...
					if (::g_Graph->nodes[indexB]->type != "-")
					{
						// node is white, red, green, blue or yellow, make an edge
						// alright, this takes a bit more storage to make nodes in the walls, but this way
						// I can check the edges from the current node and if any of the relevant ones are walls
						// THEN we don't make the edge
						glm::vec3 dir = posB - posA;
						// direction from current node to edge node
						// horizontal and vertical nodes are 2.5 units away
						Node* tempHori = nullptr;
						Node* tempVerti = nullptr;
						for (unsigned int indexC = 0; indexC != ::g_Graph->nodes[indexA]->edges.size(); indexC++)
						{
							glm::vec3 posC = glm::vec3(::g_Graph->nodes[indexA]->edges[indexC].first->position.x,
								::g_Graph->nodes[indexA]->edges[indexC].first->position.y,
								::g_Graph->nodes[indexA]->edges[indexC].first->position.z);

							if (posA.x + dir.x == posC.x)
							{
								tempHori = ::g_Graph->nodes[indexA]->edges[indexC].first;
							}
							if (posA.z + dir.z == posC.z)
							{
								tempVerti = ::g_Graph->nodes[indexA]->edges[indexC].first;
							}
						}
						if (tempHori != nullptr && tempVerti != nullptr)
						{
							if (tempHori->type != "-" && tempVerti->type != "-")
							{
								::g_Graph->AddEdge(::g_Graph->nodes[indexA],
									::g_Graph->nodes[indexB],
									15.0f,
									true);
							}
						} //end of nullptr check
					} //end of b isn't wall
				} //end of distance is diagonal
			} // end of for indexB
		} //end of A isn't wall
	} //end of for indexA

	// testing
	//::g_Graph->PrintGraph();

	MakeNodeMeshes();

	return true;
}	//end of loadTSVGrid

void MakeNodeMeshes()
{
	for (unsigned int index = 0; index != ::g_Graph->nodes.size(); index++)
	{
		cMesh* nodeMesh = new cMesh();
		nodeMesh->meshName = "Isosphere_Smooth_Normals.ply";
		nodeMesh->positionXYZ = glm::vec3(::g_Graph->nodes[index]->position.x,
			::g_Graph->nodes[index]->position.y,
			::g_Graph->nodes[index]->position.z);
		nodeMesh->setUniformScale(0.4f);
		nodeMesh->bUseWholeObjectDiffuseColour = true;
		if (::g_Graph->nodes[index]->type == "F")
		{
			nodeMesh->wholeObjectDiffuseRGBA = glm::vec4(0.0f, 0.8f, 0.2f, 1.0f);
		}
		else if (::g_Graph->nodes[index]->type == "D"
			|| ::g_Graph->nodes[index]->type == "DS"
			|| ::g_Graph->nodes[index]->type == "DP")
		{
			nodeMesh->wholeObjectDiffuseRGBA = glm::vec4(0.0f, 0.2f, 0.8f, 1.0f);
		}
		else if (::g_Graph->nodes[index]->type == "-")
		{
			nodeMesh->wholeObjectDiffuseRGBA = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
		}
		if (::g_Graph->nodes[index]->hasGoal)
		{
			nodeMesh->wholeObjectDiffuseRGBA = glm::vec4(0.8f, 0.1f, 0.1f, 1.0f);
		}
		else if (::g_Graph->nodes[index]->isExit)
		{
			nodeMesh->wholeObjectDiffuseRGBA = glm::vec4(0.6f, 0.6f, 0.2f, 1.0f);
		}
		else if (::g_Graph->nodes[index]->isHomeBase)
		{
			nodeMesh->wholeObjectDiffuseRGBA = glm::vec4(0.6f, 0.2f, 0.6f, 1.0f);
		}
		nodeMesh->bIsWireframe = true;
		//nodeMesh->bIsVisible = true;
		nodeMesh->bIsVisible = false;

		for (unsigned int indexB = 0; indexB != ::g_Graph->nodes[index]->edges.size(); indexB++)
		{
			cMesh* edgeMesh = new cMesh();
			edgeMesh->meshName = "Quad_x3_2_sided_axial_imposter_base_on_XY_axis.ply";
			glm::vec3 dir =
				glm::vec3(::g_Graph->nodes[index]->edges[indexB].first->position.x,
					::g_Graph->nodes[index]->edges[indexB].first->position.y,
					::g_Graph->nodes[index]->edges[indexB].first->position.z)
				-
				glm::vec3(::g_Graph->nodes[index]->position.x,
					::g_Graph->nodes[index]->position.y,
					::g_Graph->nodes[index]->position.z)
				;

			dir = glm::normalize(dir);
			dir += glm::vec3(::g_Graph->nodes[index]->position.x,
				::g_Graph->nodes[index]->position.y,
				::g_Graph->nodes[index]->position.z);

			if (dir.x > ::g_Graph->nodes[index]->position.x)
			{	// left
				edgeMesh->orientationXYZ = glm::vec3(0.0f, glm::radians(90.0f), 0.0f);
			}
			else if (dir.x < ::g_Graph->nodes[index]->position.x)
			{	// right
				edgeMesh->orientationXYZ = glm::vec3(0.0f, glm::radians(270.0f), 0.0f);
			}
			else if (dir.z < ::g_Graph->nodes[index]->position.z)
			{	// backwards
				edgeMesh->orientationXYZ = glm::vec3(0.0f, glm::radians(180.0f), 0.0f);
			}

			if (dir.x > ::g_Graph->nodes[index]->position.x
				&& dir.z > ::g_Graph->nodes[index]->position.z)
			{	// left and forwards
				edgeMesh->orientationXYZ = glm::vec3(0.0f, glm::radians(45.0f), 0.0f);
			}
			else if (dir.x < ::g_Graph->nodes[index]->position.x
				&& dir.z > ::g_Graph->nodes[index]->position.z)
			{	// right and forwards
				edgeMesh->orientationXYZ = glm::vec3(0.0f, glm::radians(270.0f + 45.0f), 0.0f);
			}
			else if (dir.x > ::g_Graph->nodes[index]->position.x
				&& dir.z < ::g_Graph->nodes[index]->position.z)
			{	// left and backwards 
				edgeMesh->orientationXYZ = glm::vec3(0.0f, glm::radians(180.0f - 45.0f), 0.0f);
			}
			else if (dir.x < ::g_Graph->nodes[index]->position.x
				&& dir.z < ::g_Graph->nodes[index]->position.z)
			{	// right and backwards
				edgeMesh->orientationXYZ = glm::vec3(0.0f, glm::radians(180.0f + 45.0f), 0.0f);
			}


			edgeMesh->bUseWholeObjectDiffuseColour = true;
			if (::g_Graph->nodes[index]->edges[indexB].second == 10.0f)
			{
				edgeMesh->wholeObjectDiffuseRGBA = glm::vec4(0.8f, 0.2f, 0.4f, 1.0f);
				edgeMesh->scale = glm::vec3(1.0f, 1.0f, 5.0f);
			}
			else if (::g_Graph->nodes[index]->edges[indexB].second == 15.0f)
			{
				edgeMesh->wholeObjectDiffuseRGBA = glm::vec4(0.2f, 0.2f, 0.8f, 1.0f);
				edgeMesh->scale = glm::vec3(1.0f, 1.0f, 7.07f);
			}
			nodeMesh->vec_pChildMeshes.push_back(edgeMesh);
		}

		::g_vec_pNodes.push_back(nodeMesh);
	}
}


void DrawObject(
	cMesh* pCurrentMesh,
	glm::mat4 matModel,
	GLint matModel_Location,
	GLint matModelInverseTranspose_Location,
	GLuint program,
	cVAOManager* pVAOManager);

void DrawScene1(GLuint program)
{
	glm::mat4 matModel = glm::mat4(1.0f);
	//if (::g_bStencilsOn)
	//{
	//	glStencilFunc(GL_ALWAYS, 1, 0xFF);	// enable the stencil buffer and update it for the player and non-torch entities
	//	glStencilMask(0xFF);
	//}
	//// Draw the player and non-torch entities with stencil on if it's on

	//glm::mat4 matModel = glm::mat4(1.0f);
	//DrawObject(((cPlayerEntity*)::g_pPlayer)->m_Mesh,
	//	matModel,
	//	pShaderProc->mapUniformName_to_UniformLocation["matModel"],
	//	pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"],
	//	program,
	//	::g_pVAOManager);



	//// Before moving on to the second pass, is the stencil on?
	//if (::g_bStencilsOn)
	//{
	//	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	//	glStencilMask(0x00);
	//	glDisable(GL_DEPTH_TEST);
	//	// I want to draw the stencils, go through the entities and any of type player, object or enemy need to be drawn with the stencil buffer
	//	// by which I mean instead of making a new shader, I just use a texture operator and pass in the colour
	//	for (unsigned int index = 0; index != ::vec_pAllEntities.size(); index++)
	//	{
	//		//if (::vec_pAllEntities[index]->type != iEntity::ENTITY_TYPE::TORCH)	// draw a stencil around anything that isn't a torch entity
	//		if (::vec_pAllEntities[index]->m_Mesh->bUseStencil)						// draw a stencil around anything that isn't a torch entity
	//		{
	//			glm::vec3 scale = ::vec_pAllEntities[index]->m_Mesh->scale;				// store the scale to reset after
	//			int normalTexOp = ::vec_pAllEntities[index]->m_Mesh->textureOperator;	// store texture operator to reset after
	//			::vec_pAllEntities[index]->m_Mesh->textureOperator = 20;				// set texture operator to 20 for the stencil

	//			glm::vec4 stencilColour;												// decide on colour of stencil depending on type of entity
	//			switch (::vec_pAllEntities[index]->type)
	//			{
	//			case iEntity::ENTITY_TYPE::PLAYER:
	//				stencilColour = glm::vec4(0.0f, 0.3f, 0.7f, 1.0f);
	//				::vec_pAllEntities[index]->m_Mesh->scale = glm::vec3(scale.x * 1.1f, scale.y * 1.05f, scale.z * 1.1f);
	//				break;
	//			case iEntity::ENTITY_TYPE::ENEMY:
	//				stencilColour = glm::vec4(0.8f, 0.2f, 0.0f, 1.0f);
	//				//::vec_pAllEntities[index]->m_Mesh->scale = glm::vec3(scale.x * 1.01f, scale.y * 1.01f, scale.z * 1.01f);
	//				::vec_pAllEntities[index]->m_Mesh->setUniformScale(1.1f);
	//				break;
	//			case iEntity::ENTITY_TYPE::OBJECT:
	//				stencilColour = glm::vec4(0.2f, 0.8f, 0.0f, 1.0f);
	//				//::vec_pAllEntities[index]->m_Mesh->scale = glm::vec3(scale.x * 1.01f, scale.y * 1.01f, scale.z * 1.01f);
	//				::vec_pAllEntities[index]->m_Mesh->setUniformScale(1.1f);
	//				break;
	//			case iEntity::ENTITY_TYPE::TREASURE:
	//				stencilColour = glm::vec4(1.0f, 1.0f, 0.6f, 1.0f);
	//				//::vec_pAllEntities[index]->m_Mesh->scale = glm::vec3(scale.x * 1.01f, scale.y * 1.01f, scale.z * 1.01f);
	//				::vec_pAllEntities[index]->m_Mesh->setUniformScale(1.1f);
	//				break;
	//			default:
	//				stencilColour = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
	//				::vec_pAllEntities[index]->m_Mesh->setUniformScale(1.1f);
	//			}

	//			// pass the colour to the shader
	//			glUniform4f(pShaderProc->mapUniformName_to_UniformLocation["stencilColour"],
	//				stencilColour.x, stencilColour.y, stencilColour.z, stencilColour.a);

	//			matModel = glm::mat4(1.0f);										// draw the stencil around the entity
	//			DrawObject(::vec_pAllEntities[index]->m_Mesh,
	//				matModel,
	//				pShaderProc->mapUniformName_to_UniformLocation["matModel"],
	//				pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"],
	//				program,
	//				::g_pVAOManager);

	//			// reset the texture operator and scale
	//			::vec_pAllEntities[index]->m_Mesh->textureOperator = normalTexOp;
	//			::vec_pAllEntities[index]->m_Mesh->scale = scale;
	//		}
	//	}
	//	// once all stencil objects are drawn, we can reset the stencil stuff
	//	glStencilMask(0xFF);
	//	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	//	glEnable(GL_DEPTH_TEST);
	//}	// now if we drawing the stencil outline there should be an outline around all my entities (minus the torches)


	for (unsigned int index = 0; index != vec_pFSMEntities.size(); index++)
	{
		matModel = glm::mat4(1.0f);

		DrawObject(vec_pFSMEntities[index]->m_Mesh,
			matModel,
			pShaderProc->mapUniformName_to_UniformLocation["matModel"],
			pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"],
			program,
			::g_pVAOManager);
	}


	if (::drawLightBalls)
	{
		for (unsigned int index = 0; index != ::g_vec_pPointLights.size(); index++)
		{
			matModel = glm::mat4(1.0f);
			DrawObject(::g_vec_pPointLights[index],
				matModel,
				pShaderProc->mapUniformName_to_UniformLocation["matModel"],
				pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"],
				program,
				::g_pVAOManager);
		}
	}

	for (unsigned int index = 0; index != ::g_vec_pNodes.size(); index++)
	{
		matModel = glm::mat4(1.0f);
		if (!::g_ObservationMode)
		{
			if (glm::distance(::g_vec_pNodes[index]->positionXYZ, ::cameraEye) < 75.0f)
			{
				DrawObject(::g_vec_pNodes[index],
					matModel,
					pShaderProc->mapUniformName_to_UniformLocation["matModel"],
					pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"],
					program,
					::g_pVAOManager);
			}
		}
		else
		{
			DrawObject(::g_vec_pNodes[index],
				matModel,
				pShaderProc->mapUniformName_to_UniformLocation["matModel"],
				pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"],
				program,
				::g_pVAOManager);
		}
	}

	// for whatever reason, with the FBO stuff, the last item drawn before the FBO full screen gets all flickery
	// so, there's an extra model without the model itself being loaded on the list of meshes that gets drawn last
	// viola, no more jittery black squares or missing vertices, cause it's not loaded to be drawn anyways lol
	for (unsigned int index = 0; index != ::g_vec_pMeshes.size(); index++)
	{
		// So the code is a little easier...
		cMesh* pCurrentMesh = ::g_vec_pMeshes[index];

		matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
		//mat4x4_identity(m);

		if (pCurrentMesh->bHasDiscardTexture)
		{
			// Discard texture
			{
				//GLuint discardTextureNumber = ::g_pTextureManager->getTextureIDFromName("Lisse_mobile_shipyard-mal1.bmp");
				GLuint discardTextureNumber = ::g_pTextureManager->getTextureIDFromName(pCurrentMesh->discardTexture);

				// I'm picking texture unit 30 since it's not in use.
				GLuint textureUnit = 30;
				glActiveTexture(textureUnit + GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, discardTextureNumber);
				//GLint discardTexture_LocID = glGetUniformLocation(program, "discardTexture");
				//glUniform1i(discardTexture_LocID, textureUnit);
				glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["discardTexture"], textureUnit);


				// Turn it on
				//glUniform1f(bDiscardTransparencyWindowsON_LocID, (GLfloat)GL_TRUE);
				glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bDiscardTransparencyWindowsOn"], (GLfloat)GL_TRUE);
				/*DrawObject(pCurrentMesh,
					matModel,
					matModel_Location,
					matModelInverseTranspose_Location,
					program,
					::g_pVAOManager);*/

				DrawObject(pCurrentMesh,
					matModel,
					pShaderProc->mapUniformName_to_UniformLocation["matModel"],
					pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"],
					program,
					::g_pVAOManager);

				// Turn it off
				glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bDiscardTransparencyWindowsOn"], (GLfloat)GL_FALSE);
			}
		}
		else
		{
			//DrawObject(pCurrentMesh,
			//	matModel,
			//	matModel_Location,
			//	matModelInverseTranspose_Location,
			//	program,
			//	::g_pVAOManager);

			// temp
			//if (glm::distance(pCurrentMesh->positionXYZ, ::cameraEye) < 50.0f)
			//{
				// this is a pretty huge speed up, by not drawing objects that are outside of the vision anyways, but it's not really LOD
				// so I gotta find some real low poly models and make entities that can switch between the models instead of just doing the current mesh
			DrawObject(pCurrentMesh,
				matModel,
				pShaderProc->mapUniformName_to_UniformLocation["matModel"],
				pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"],
				program,
				::g_pVAOManager);
			//}

		}


	}//for (unsigned int index
}


// Don't think I'm using this keeping it for reference

//void MakeGraphAndMeshes()
//{
//	char* data = bmp->GetData();
//	unsigned long imageWidth = bmp->GetImageWidth();
//	unsigned long imageHeight = bmp->GetImageHeight();
//	char name = 'a';
//
//	int index = 0;
//	for (unsigned long x = 0; x < imageWidth; x++) {
//		for (unsigned long y = 0; y < imageHeight; y++) {
//			char c = GetColourCharacter(data[index++], data[index++], data[index++]);
//			printf("%c", c);
//
//			cMesh* tempMesh = new cMesh();
//			tempMesh->meshName = "Quad_1_sided_aligned_on_XY_plane.ply";
//			tempMesh->positionXYZ = glm::vec3(y, x, 0.0f);
//			tempMesh->orientationXYZ = glm::vec3(0.0f);
//			tempMesh->setUniformScale(1.0f);
//			tempMesh->bUseWholeObjectDiffuseColour = true;
//			if (c == 'r')			// resource
//			{
//				tempMesh->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
//				gNumResources++;
//			}
//			else if (c == 'g')		// spawn point
//			{
//				tempMesh->wholeObjectDiffuseRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
//				spawnPoints.push_back(glm::vec3(y, x, 0.0f));
//			}
//			else if (c == 'b')		// home base
//			{
//				tempMesh->wholeObjectDiffuseRGBA = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
//			}
//			else if (c == 'y')		// difficult terrain
//			{
//				tempMesh->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
//			}
//			else if (c == 'w')		// regular terrain
//			{
//				tempMesh->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//			}
//			else if (c == '_')		// not traversable
//			{
//				tempMesh->wholeObjectDiffuseRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
//			}
//			else if (c == 'x')		// reading error, colour in BMP is not one of our tile colours
//			{
//				//std::cout << "Uh oh, something wrong with the graph" << std::endl;
//				//tempMesh->wholeObjectDiffuseRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
//				tempMesh->wholeObjectDiffuseRGBA = glm::vec4(0.6f, 0.8f, 0.4f, 1.0f);
//			}
//			else
//			{
//				std::cout << "SOMETHING REALLY WRONG WITH THE GRAPH" << std::endl;
//				tempMesh->wholeObjectDiffuseRGBA = glm::vec4(0.8f, 0.8f, 0.0f, 1.0f);
//			}
//			tempMesh->bDontLight = true;
//			tempMesh->clearTextureRatiosToZero();
//			::g_vec_pMeshes.push_back(tempMesh);
//
//			// if the colour of the node is blue, it's the home base, if the colour of the node is red, it's a resource (or it has a goal)
//			::g_Graph->CreateNode(name, Vertex(y, x, 0.0f), c, (c == 'b'), (c == 'r'));
//			name++;
//		}
//		printf("\n");
//	}
//	//system("pause");
//
//	// BMP has been read, maze/graph is generated, now we need to make edges for the graph...
//
//	//for (unsigned int index = 0; index != ::g_Graph->nodes.size(); index++)
//	//{
//	//	std::cout << "Node: " << index << " is " << ::g_Graph->nodes[index]->id
//	//		<< " at x: " << ::g_Graph->nodes[index]->position.x << " y: " << ::g_Graph->nodes[index]->position.y
//	//		<< std::endl;
// //	}
//	//system("pause");
//
//
//	// other option, takes longer but makes more sense in my head
//	// go through the nodes and find all the neighbhouring nodes
//	for (unsigned int indexA = 0; indexA != ::g_Graph->nodes.size() - 1; indexA++)
//	{
//		float posAX = ::g_Graph->nodes[indexA]->position.x;
//		float posAY = ::g_Graph->nodes[indexA]->position.y;
//
//		// if the node at indexA is not traversable, then we don't want to make any edges out of it anyways 
//		if (::g_Graph->nodes[indexA]->type != '_' && ::g_Graph->nodes[indexA]->type != 'x')
//		{
//			for (unsigned int indexB = indexA + 1; indexB != ::g_Graph->nodes.size(); indexB++)
//			{
//				// and then compare the position of each node at indexB to the position of the node at indexA
//				float posBX = ::g_Graph->nodes[indexB]->position.x;
//				float posBY = ::g_Graph->nodes[indexB]->position.y;
//
//				// Left
//				if (posBX == posAX - 1 && posBY == posAY)
//				{
//					// ok, so this node is to the left of our node
//					// now we need to check what type it is, if it's black we don't make an Edge
//					if (::g_Graph->nodes[indexB]->type != '_' && ::g_Graph->nodes[indexB]->type != 'x')
//					{
//						// node is white, red, green, blue or yellow, make an edge
//						::g_Graph->AddEdge(::g_Graph->nodes[indexA],
//							::g_Graph->nodes[indexB],
//							10.0f,
//							true);
//					}
//				}
//				if (posBX == posAX + 1 && posBY == posAY)	// right
//				{
//					// now we need to check what type it is, if it's black we don't make an Edge
//					if (::g_Graph->nodes[indexB]->type != '_' && ::g_Graph->nodes[indexB]->type != 'x')
//					{
//						// node is white, red, green, blue or yellow, make an edge
//						::g_Graph->AddEdge(::g_Graph->nodes[indexA],
//							::g_Graph->nodes[indexB],
//							10.0f,
//							true);
//					}
//				}
//
//				if (posBY == posAY + 1 && posBX == posAX)	// up
//				{
//					// now we need to check what type it is, if it's black we don't make an Edge
//					if (::g_Graph->nodes[indexB]->type != '_' && ::g_Graph->nodes[indexB]->type != 'x')
//					{
//						// node is white, red, green, blue or yellow, make an edge
//						::g_Graph->AddEdge(::g_Graph->nodes[indexA],
//							::g_Graph->nodes[indexB],
//							10.0f,
//							true);
//					}
//				}
//				if (posBY == posAY - 1 && posBX == posAX)	// down
//				{
//					// now we need to check what type it is, if it's black we don't make an Edge
//					if (::g_Graph->nodes[indexB]->type != '_' && ::g_Graph->nodes[indexB]->type != 'x')
//					{
//						// node is white, red, green, blue or yellow, make an edge
//						::g_Graph->AddEdge(::g_Graph->nodes[indexA],
//							::g_Graph->nodes[indexB],
//							10.0f,
//							true);
//					}
//				}
//
//				// Now the fun part... Diagonals
//				// which are dependent on the nodes left, right, above and below the current node... uggh
//
//				// Left and Above
//				if (posBX == posAX - 1
//					&& posBY == posAY + 1)
//				{
//					// first find the nodes to the left and above the current node
//					// holy cow this is inefficient
//					Node* tempNodeA = nullptr;
//					Node* tempNodeB = nullptr;
//					for (unsigned int indexC = 0; indexC != ::g_Graph->nodes.size(); indexC++)
//					{
//						float posCX = ::g_Graph->nodes[indexC]->position.x;
//						float posCY = ::g_Graph->nodes[indexC]->position.y;
//						if (posCX == posAX - 1 && posCY == posAY)	// node to the left of current node
//						{
//							tempNodeA = ::g_Graph->nodes[indexC];
//						}
//						if (posCX == posAX && posCY == posAY + 1)	// node above the current node
//						{
//							tempNodeB = ::g_Graph->nodes[indexC];
//						}
//					}
//					if (tempNodeA != nullptr && tempNodeB != nullptr)
//					{
//						if (tempNodeA->type != '_' && tempNodeA->type != 'x'
//							&& tempNodeB->type != '_' && tempNodeB->type != 'x')
//						{
//							if (::g_Graph->nodes[indexB]->type != '_' && ::g_Graph->nodes[indexB]->type != 'x')
//							{
//								::g_Graph->AddEdge(::g_Graph->nodes[indexA],
//									::g_Graph->nodes[indexB],
//									14.0f,
//									true);
//							}
//						}
//					}
//				}
//
//				// Left and Below
//				if (posBX == posAX - 1
//					&& posBY == posAY - 1)
//				{
//					Node* tempNodeA = nullptr;
//					Node* tempNodeB = nullptr;
//					for (unsigned int indexC = 0; indexC != ::g_Graph->nodes.size(); indexC++)
//					{
//						float posCX = ::g_Graph->nodes[indexC]->position.x;
//						float posCY = ::g_Graph->nodes[indexC]->position.y;
//						if (posCX == posAX - 1 && posCY == posAY)	// node to the left of current node
//						{
//							tempNodeA = ::g_Graph->nodes[indexC];
//						}
//						if (posCX == posAX && posCY == posAY - 1)	// node below the current node
//						{
//							tempNodeB = ::g_Graph->nodes[indexC];
//						}
//					}
//					if (tempNodeA != nullptr && tempNodeB != nullptr)
//					{
//						if (tempNodeA->type != '_' && tempNodeA->type != 'x'
//							&& tempNodeB->type != '_' && tempNodeB->type != 'x')
//						{
//							if (::g_Graph->nodes[indexB]->type != '_' && ::g_Graph->nodes[indexB]->type != 'x')
//							{
//								::g_Graph->AddEdge(::g_Graph->nodes[indexA],
//									::g_Graph->nodes[indexB],
//									14.0f,
//									true);
//							}
//						}
//					}
//				}
//
//				// Right and Above
//				if (posBX == posAX + 1
//					&& posBY == posAY + 1)
//				{
//					// first find the nodes to the left and above the current node
//					// holy cow this is inefficient
//					Node* tempNodeA = nullptr;
//					Node* tempNodeB = nullptr;
//					for (unsigned int indexC = 0; indexC != ::g_Graph->nodes.size(); indexC++)
//					{
//						float posCX = ::g_Graph->nodes[indexC]->position.x;
//						float posCY = ::g_Graph->nodes[indexC]->position.y;
//						if (posCX == posAX + 1 && posCY == posAY)	// node to the right of current node
//						{
//							tempNodeA = ::g_Graph->nodes[indexC];
//						}
//						if (posCX == posAX && posCY == posAY + 1)	// node above the current node
//						{
//							tempNodeB = ::g_Graph->nodes[indexC];
//						}
//					}
//					if (tempNodeA != nullptr && tempNodeB != nullptr)
//					{
//						if (tempNodeA->type != '_' && tempNodeA->type != 'x'
//							&& tempNodeB->type != '_' && tempNodeB->type != 'x')
//						{
//							if (::g_Graph->nodes[indexB]->type != '_' && ::g_Graph->nodes[indexB]->type != 'x')
//							{
//								// node is  yellow, double edge weight
//								::g_Graph->AddEdge(::g_Graph->nodes[indexA],
//									::g_Graph->nodes[indexB],
//									14.0f,
//									true);
//							}
//						}
//					}
//				}
//
//				// Right and Below
//				if (posBX == posAX - 1
//					&& posBY == posAY - 1)
//				{
//					Node* tempNodeA = nullptr;
//					Node* tempNodeB = nullptr;
//					for (unsigned int indexC = 0; indexC != ::g_Graph->nodes.size(); indexC++)
//					{
//						float posCX = ::g_Graph->nodes[indexC]->position.x;
//						float posCY = ::g_Graph->nodes[indexC]->position.y;
//						if (posCX == posAX + 1 && posCY == posAY)	// node to the right of current node
//						{
//							tempNodeA = ::g_Graph->nodes[indexC];
//						}
//						if (posCX == posAX && posCY == posAY - 1)	// node below the current node
//						{
//							tempNodeB = ::g_Graph->nodes[indexC];
//						}
//					}
//					if (tempNodeA != nullptr && tempNodeB != nullptr)
//					{
//						if (tempNodeA->type != '_' && tempNodeA->type != 'x'
//							&& tempNodeB->type != '_' && tempNodeB->type != 'x')
//						{
//							if (::g_Graph->nodes[indexB]->type != '_' && ::g_Graph->nodes[indexB]->type != 'x')
//							{
//								// node is  yellow, double edge weight
//								::g_Graph->AddEdge(::g_Graph->nodes[indexA],
//									::g_Graph->nodes[indexB],
//									14.0f,
//									true);
//							}
//						}
//					}
//				}
//
//			}
//		}
//		
//	}
//
//	//::g_Graph->PrintGraph();
//
//	//system("pause");
//
//	MakeFSMEntities();
//
//	return;
//}