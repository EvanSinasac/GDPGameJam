#include "globalThings.h"
#include <sstream>
#include <iostream>
#include "cDungeonMeshBuilder.h"

// Modified from previous versions to take texture information
bool loadWorldFile(unsigned int& numberOfTransparentObjects)
{
	std::stringstream ss;
	std::stringstream sFile;


	ss << SOLUTION_DIR << "common\\assets\\worldFile.txt";

	std::ifstream theFile(ss.str());

	if (!theFile.is_open())
	{
		fprintf(stderr, "Could not open worldFile.txt");
		return false;
	}

	std::string nextToken;
	ss.str("");

	//Throwaway text describing the format of the file
	theFile >> nextToken;       //ModelFileName(extension)
	theFile >> nextToken;       //Position(x,y,z)
	theFile >> nextToken;       //Orientation(x,y,z)
	theFile >> nextToken;       //Scale(x,y,z)
	theFile >> nextToken;       //Colour(r,g,b,a)
	theFile >> nextToken;		//UseColour
	theFile >> nextToken;		//Light
	theFile >> nextToken;		//TextureOp
	theFile >> nextToken;		//TextureName1
	theFile >> nextToken;		//Ratio1
	theFile >> nextToken;		//TextureName2
	theFile >> nextToken;		//Ratio2
	theFile >> nextToken;		//TextureName3
	theFile >> nextToken;		//Ratio3
	theFile >> nextToken;		//DiscardTexture

	theFile >> nextToken;		//Camera(x,y,z)
	theFile >> nextToken;		//x position
	::cameraEye.x = std::stof(nextToken);
	theFile >> nextToken;		//y position
	::cameraEye.y = std::stof(nextToken);
	theFile >> nextToken;		//z position
	::cameraEye.z = std::stof(nextToken);

	theFile >> nextToken;		//number of transparent objects
	numberOfTransparentObjects = std::stoi(nextToken);

	//From here modify based on worldFile format
	while (theFile >> nextToken)    //this should always be the name of the model to load or end.  Potential error check, add a check for "ply" in the mdoel name
	{
		cMesh* curMesh = new cMesh;
		if (nextToken == "end")
		{
			break;
		}
		std::cout << nextToken << std::endl;        //Printing model names to console, just making sure we're loading ok.  Can be commented out whenever
		//First is the file name of model
		//ss << SOLUTION_DIR << "common\\assets\\models\\" << nextToken;		// don't need this with the setBasePath function for the loading intot the VAO
		curMesh->meshName = nextToken;
		//Next 3 are the position of the model
		theFile >> nextToken;                                               //x position for the model
		curMesh->positionXYZ.x = std::stof(nextToken);
		theFile >> nextToken;                                               //y position for the model
		curMesh->positionXYZ.y = std::stof(nextToken);
		theFile >> nextToken;                                               //z position for the model
		curMesh->positionXYZ.z = std::stof(nextToken);
		//Next 3 are the orientation of the model
		theFile >> nextToken;                                               //x orientation value
		//curMesh.orientationXYZ.x = std::stof(nextToken);
		curMesh->orientationXYZ.x = glm::radians(std::stof(nextToken));
		theFile >> nextToken;                                               //y orientation value
		//curMesh.orientationXYZ.y = std::stof(nextToken);
		curMesh->orientationXYZ.y = glm::radians(std::stof(nextToken));
		theFile >> nextToken;                                               //z orientation value
		//curMesh.orientationXYZ.z = std::stof(nextToken);
		curMesh->orientationXYZ.z = glm::radians(std::stof(nextToken));
		//Next is the scale to multiply the model by
		theFile >> nextToken;                                               //scale multiplier
		curMesh->scale.x = std::stof(nextToken);
		theFile >> nextToken;                                               //scale multiplier
		curMesh->scale.y = std::stof(nextToken);
		theFile >> nextToken;                                               //scale multiplier
		curMesh->scale.z = std::stof(nextToken);

		//Next 3 are the r, g, b values for the model
		curMesh->bUseWholeObjectDiffuseColour = false;
		theFile >> nextToken;													//RGB red value
		curMesh->wholeObjectDiffuseRGBA.r = std::stof(nextToken) / 255.0f;		//convert to nice shader value (between 0 and 1)
		theFile >> nextToken;													//RGB green value
		curMesh->wholeObjectDiffuseRGBA.g = std::stof(nextToken) / 255.0f;		//convert to nice shader value (between 0 and 1)
		theFile >> nextToken;													//RGB blue value
		curMesh->wholeObjectDiffuseRGBA.b = std::stof(nextToken) / 255.0f;		//convert to nice shader value (between 0 and 1)
		theFile >> nextToken;
		curMesh->wholeObjectDiffuseRGBA.a = std::stof(nextToken);
		curMesh->alphaTransparency = std::stof(nextToken);
		theFile >> nextToken;
		curMesh->bUseWholeObjectDiffuseColour = (nextToken == "1");				// 1 means use colour, anything else means don't

		// Light
		theFile >> nextToken;
		curMesh->bDontLight = (nextToken == "0");		// set to true or "0" in the worldFile to see textures without lighting

		// Texture operator
		theFile >> nextToken;
		curMesh->textureOperator = std::stoi(nextToken);

		// texture for mesh is in worldFile.txt now
		curMesh->clearTextureRatiosToZero();

		theFile >> nextToken;
		if (nextToken != "null")
		{
			curMesh->textureNames[1] = nextToken;
			theFile >> nextToken;
			curMesh->textureRatios[1] = std::stof(nextToken);
		}
		else
		{
			theFile >> nextToken;	// throw away the ratio number
		}

		theFile >> nextToken;
		if (nextToken != "null")
		{
			curMesh->textureNames[2] = nextToken;
			theFile >> nextToken;
			curMesh->textureRatios[2] = std::stof(nextToken);
		}
		else
		{
			theFile >> nextToken;	// throw away the ratio number
		}

		theFile >> nextToken;
		if (nextToken != "null")
		{
			curMesh->textureNames[3] = nextToken;
			theFile >> nextToken;
			curMesh->textureRatios[3] = std::stof(nextToken);
		}
		else
		{
			theFile >> nextToken;	// throw away the ratio number
		}

		if (curMesh->textureRatios[1] == 0.0f && curMesh->textureRatios[2] == 0.0f && curMesh->textureRatios[3] == 0.0f)
		{
			curMesh->textureRatios[0] = 1.0f;
		}

		// Discard texture
		theFile >> nextToken;
		if (nextToken != "null")
		{
			curMesh->discardTexture = nextToken;
			curMesh->bHasDiscardTexture = true;
		}

		::g_vec_pMeshes.push_back(curMesh);     //push the model onto our vector of meshes
		ss.str("");                         //reset the stringstream
	} //end of while
	theFile.close();
	return true;
}	//end of load world file

bool loadLightsFile()
{
	cDungeonMeshBuilder main_DungeonBuilder;
	std::stringstream ss;
	std::stringstream sFile;

	//std::vector<float> rotations;

	ss << SOLUTION_DIR << "common\\assets\\lights.txt";

	std::ifstream theFile(ss.str());

	if (!theFile.is_open())
	{
		fprintf(stderr, "Could not open lights.txr");
		return false;
	}

	std::string nextToken;
	ss.str("");

	// Throw away description 
	theFile >> nextToken;		// position(x,y,z)
	theFile >> nextToken;		// diffuse(r,g,b)
	theFile >> nextToken;		// specular(r,g,b)
	theFile >> nextToken;		// atten(x,y,z,w)
	theFile >> nextToken;		// direction(x,y,z)
	theFile >> nextToken;		// param1(x,y,z)
	theFile >> nextToken;		// param2(x)

	theFile >> nextToken;		// rotation (for the torch objects)

	::g_pTheLights->TurnOffLight(0);

	unsigned int index = 0;	// can't start at 0 because for some reason the 0 light over writes all other lights

	while (theFile >> nextToken)
	{
		if (nextToken == "end" || index >= cLightManager::NUMBER_OF_LIGHTS)
		{
			break;
		}
		glm::vec3 position;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec4 atten;
		glm::vec3 direction;
		glm::vec3 param1;
		float param2;

		float rotation;

		// Position
		position.x = std::stof(nextToken);
		theFile >> nextToken;
		position.y = std::stof(nextToken);
		theFile >> nextToken;
		position.z = std::stof(nextToken);

		// Diffuse
		theFile >> nextToken;
		diffuse.x = std::stof(nextToken);
		theFile >> nextToken;
		diffuse.y = std::stof(nextToken);
		theFile >> nextToken;
		diffuse.z = std::stof(nextToken);

		// Specular
		theFile >> nextToken;
		specular.x = std::stof(nextToken);
		theFile >> nextToken;
		specular.y = std::stof(nextToken);
		theFile >> nextToken;
		specular.z = std::stof(nextToken);

		// Atten
		theFile >> nextToken;
		atten.x = std::stof(nextToken);
		theFile >> nextToken;
		atten.y = std::stof(nextToken);
		theFile >> nextToken;
		atten.z = std::stof(nextToken);
		theFile >> nextToken;
		atten.w = std::stof(nextToken);

		// Direction
		theFile >> nextToken;
		direction.x = std::stof(nextToken);
		theFile >> nextToken;
		direction.y = std::stof(nextToken);
		theFile >> nextToken;
		direction.z = std::stof(nextToken);

		// Param1
		theFile >> nextToken;
		param1.x = std::stof(nextToken);;
		theFile >> nextToken;
		param1.y = std::stof(nextToken);
		theFile >> nextToken;
		param1.z = std::stof(nextToken);

		// Param2
		theFile >> nextToken;
		param2 = std::stof(nextToken);

		// Torches rotation
		theFile >> nextToken;
		rotation = std::stof(nextToken);

		// Load everything into the lights
		float ogX = position.x;
		float ogY = position.y;
		float ogZ = position.z;


		float intX = (int)position.x;
		float diffX = ogX - intX;
		if (diffX > 0.5f)
		{
			diffX = -1.0f + diffX;
		}
		if (diffX != 0.5f)
		{
			position.x += diffX * 3.0f;
			//position.x += 0.75f;
		}

		position.y += 0.575f;

		float intZ = (int)position.z;
		float diffZ = ogZ - intZ;
		if (diffZ > 0.5f)
		{
			diffZ = -1.0f + diffZ;
		}
		if (diffZ != 0.5f)
		{
			position.z += diffZ * 3.0f;
			//position.z += 0.75f;
		}



		::g_pTheLights->theLights[index].position = glm::vec4(position, 1.0f);


		::g_pTheLights->theLights[index].diffuse = glm::vec4(diffuse, 1.0f);
		::g_pTheLights->theLights[index].specular = glm::vec4(specular, 1.0f);
		::g_pTheLights->theLights[index].atten = atten;
		::g_pTheLights->theLights[index].direction = glm::vec4(direction, 1.0f);
		::g_pTheLights->theLights[index].param1 = glm::vec4(param1, 1.0f);
		::g_pTheLights->theLights[index].param2 = glm::vec4(param2, 0.0f, 0.0f, 1.0f);

		//rotations.push_back(rotation);

		if (::g_pTheLights->theLights[index].param1.x == 0)
		{
			// Make torch entities where there are point lights
			cMesh* torchSet = main_DungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::LIGHT_FIXTURE, glm::vec3(1.0f));
			torchSet->positionXYZ = glm::vec3(ogX, ogY, ogZ);
			torchSet->orientationXYZ = glm::vec3(0.0f, glm::radians(rotation), 0.0f);
			::vec_pTorches.push_back(new cTorchObject(torchSet, index));
			::g_vec_pMeshes.push_back(torchSet);


			cMesh* pointLightBall = new cMesh();
			pointLightBall->meshName = "Isosphere_Smooth_Normals.ply";
			pointLightBall->bIsWireframe = true;
			pointLightBall->bDontLight = true;
			pointLightBall->bUseWholeObjectDiffuseColour = true;
			pointLightBall->wholeObjectDiffuseRGBA = ::g_pTheLights->theLights[index].specular;
			pointLightBall->positionXYZ = ::g_pTheLights->theLights[index].position;
			pointLightBall->setUniformScale(0.1f);
			::g_vec_pPointLights.push_back(pointLightBall);
		}

		index++;
	} //end of while

	//mazeLightsStartIndex = index;
	//::g_currentLightIndex = index;

	theFile.close();

	for (unsigned int indexA = 0; indexA != index; indexA++)
	{

	}

	return true;

} //end of load lights

// Graphics 2 Midterm Models
void loadGraphicsMidtermModels(std::vector<std::string>& modelLocations)
{
	// Midterm Models
	modelLocations.push_back("Entire_Station_xyz_rgba_n_uv.ply");
	modelLocations.push_back("SM_Env_Consoles_01_xyz_n_rgba_uv.ply");
	modelLocations.push_back("SM_Env_Consoles_Corner_01_xyz_n_rgba_uv.ply");

	modelLocations.push_back("SM_Env_Construction_Wall_01_xyz_n_rgba_uv.ply");
	modelLocations.push_back("SM_Env_Floor_01_xyz_n_rgba_uv.ply");
	modelLocations.push_back("SM_Env_Floor_04_xyz_n_rgba_uv.ply");

	// yeah, the FBO might want this... lol
	modelLocations.push_back("Quad_1_sided_aligned_on_XY_plane.ply");
	modelLocations.push_back("Quad_2_sided_aligned_on_XY_plane.ply");
	modelLocations.push_back("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply");
	modelLocations.push_back("Isosphere_Smooth_Normals.ply");

	// space station
	modelLocations.push_back("Arms Small_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Arms_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Cooling Arms_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Hanger_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Parts_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Reactor_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Ring Small1_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Ring Small2_xyz_rgba_n_uv.ply");
	modelLocations.push_back("RingBig_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Solar Panels_xyz_rgba_n_uv.ply");
	modelLocations.push_back("Triangle Parts_xyz_rgba_n_uv.ply");

} //end of loadMidtermModels

// Graphics 2 Project 2 Models
void loadGraphicsProject2Models(std::vector<std::string>& modelLocations)
{
	modelLocations.push_back("SM_Prop_Beaker_01_xyz_n_rgba_uv.ply");

	// Follow along models
	modelLocations.push_back("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply");
	modelLocations.push_back("mod-bomber_xyz_n_rgba_uv.ply");

	// DFK Models
	modelLocations.push_back("dfk_bed_single_01_XYZ_N_RGBA_UV_tranformed.ply");
	modelLocations.push_back("dfk_bookshelf_XYZ_N_RGBA_UV.ply");
	modelLocations.push_back("dfk_chairs_XYZ_N_RGBA_UV.ply");
	modelLocations.push_back("dfk_crystal_01_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_crystal_02_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_floor_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_table_large_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_wall_1_XYZ_N_RGBA_N_transformed.ply");
	modelLocations.push_back("dfk_wardrobe_01_XYZ_N_RGBA_UV.ply");
	modelLocations.push_back("dfk_torch_holder_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_torch_XYZ_N_RGBA_UV_transformed.ply");

	modelLocations.push_back("Quad_1_sided_aligned_on_XY_plane.ply");
	modelLocations.push_back("Quad_2_sided_aligned_on_XY_plane.ply");
	modelLocations.push_back("Engine_Exhaust_Imposter.ply");
	//modelLocations.push_back("dfk_bed_single_01.ply");
	//modelLocations.push_back("dfk_bookshelf.ply");
	//modelLocations.push_back("dfk_chairs.ply");
	//modelLocations.push_back("dfk_crystal_01.ply");
	//modelLocations.push_back("dfk_crystal_02.ply");
	//modelLocations.push_back("dfk_floor.ply");
	//modelLocations.push_back("dfk_table_large.ply");
	//modelLocations.push_back("dfk_wall_1.ply");
	//modelLocations.push_back("dfk_wardrobe_01.ply");

	modelLocations.push_back("SM_Prop_Desk_04_xyz_n_rgba_uv.ply");
	modelLocations.push_back("SM_Prop_Monitor_03_xyz_n_rgba_uv.ply");
	modelLocations.push_back("SM_Prop_SwivelChair_01_xyz_n_rgba_uv.ply");

} //end of loadProject2Models

// AI Project 3 Models
void loadAIModels(std::vector<std::string>& modelLocations)
{
	modelLocations.push_back("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply");
	modelLocations.push_back("Isosphere_Smooth_Normals.ply");

	modelLocations.push_back("Quad_1_sided_aligned_on_XY_plane.ply");
	modelLocations.push_back("Quad_2_sided_aligned_on_XY_plane.ply");
}

// GameJam Models
void loadGameJamModels(std::vector<std::string>& modelLocations)
{
	modelLocations.push_back("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply");
	modelLocations.push_back("Isosphere_Smooth_Normals.ply");

	modelLocations.push_back("Quad_1_sided_aligned_on_XY_plane.ply");
	modelLocations.push_back("Quad_2_sided_aligned_on_XY_plane.ply");

	modelLocations.push_back("dfk_bed_single_01_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_ceiling_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_door_01_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_doorframe_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_floor_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_gate_small_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_stairs_wood_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_wall_1_bevel_door_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_wall_1_XYZ_N_RGBA_N_transformed.ply");

	modelLocations.push_back("dfk_torch_holder_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("dfk_torch_XYZ_N_RGBA_UV_transformed.ply");
	modelLocations.push_back("Engine_Exhaust_Imposter.ply");	// for the torch lights
	modelLocations.push_back("Quad_x3_2_sided_axial_imposter_base_on_XY_axis.ply");

	//modelLocations.push_back("SK_Anglerox_XYZ_N_RGBA_UV_converted_3.ply");
	modelLocations.push_back("Pokemon.ply");
}