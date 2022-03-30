// Evan Sinasac - 1081418
// Game Jam
// Description:
//			Honestly, not entirely certain what I'm making with this.  I've been thinking about what we've done in class
//			over the last year and the things I'm most interested in were Graphics and A.I.  So, I decided to go back
//			to some of our old projects for inspiration, and make a sort of dungeon crawler, or simulate one (not sure yet)
//			And I'll try to add in some Physics and Animation stuff where I can

#include "GLCommon.h"
#include "GLMCommon.h"

#include "globalThings.h"
#include "MapInfo.h"
#include "BMPImage.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>

#include "cDungeonMeshBuilder.h"


float lastX = 600.0f;
float lastY = 320.0f;
bool firstMouse = true;
float cameraYaw = 90.0f;
float cameraPitch = 0.0f;
// am I even using these anymore
float doorLightDirCurX = 1.0f;
float doorLightDirCurZ = 0.0f;
float doorLightRotateX = 1.0f;
float doorLightRotateZ = 1.0f;

double deltaTime = 0.0;

int numberOfTransparentObjects = 0;

std::vector<std::string> modelLocations;

//std::vector<cFSMEntity*> vec_pFSMEntities;
std::vector<glm::vec3> spawnPoints;

//Function signature for DrawObject()
void DrawObject(
	cMesh* pCurrentMesh,
	glm::mat4 matModel,
	GLint matModel_Location,
	GLint matModelInverseTranspose_Location,
	GLuint program,
	cVAOManager* pVAOManager);

bool loadWorldFile();
bool loadLightsFile();
unsigned int mazeLightsStartIndex;

void loadGraphicsMidtermModels();
void loadGraphicsProject2Models();
void loadAIModels();
void loadGameJamModels();

void MakeGraphAndMeshes();
void MakeFSMEntities();

bool loadTSVGrid();			// modified version from Graphics 1 Final

// AI Project 3
char GetColourCharacter(unsigned char r, unsigned char g, unsigned char b);

int gNumResources;
BMPImage* bmp;

//callbacks
static void mouse_callback(GLFWwindow* window, double xpos, double ypos);




int main(int argv, char** argc)
{
	gNumResources = 0;

	//cFSMEntity* testEntity = new cFSMEntity();

	srand(time(NULL));	// learned this recently, only supposed to use srand once per program
	GLFWwindow* pWindow;

	GLuint program = 0;

	//GLint mvp_location = -1;

	std::stringstream ss;

	float baseAtten = 0.4f;
	float randomAddedAtten;

	// Testing for AI Project 3
	// TODO: Remove/Modify for project
	//ss << SOLUTION_DIR << "common\\assets\\textures\\example.bmp";
	ss << SOLUTION_DIR << "common\\assets\\textures\\resourceMap.bmp";
	//ss << SOLUTION_DIR << "common\\assets\\textures\\resourceMap2.bmp";
	//ss << SOLUTION_DIR << "common\\assets\\textures\\resourceMap2V2.bmp";
	//ss << SOLUTION_DIR << "common\\assets\\textures\\resourceMap3.bmp";		// DO NOT USE
	//ss << SOLUTION_DIR << "common\\assets\\textures\\resourceMap3V2.bmp";		// Resized to 56x56
	//ss << SOLUTION_DIR << "common\\assets\\textures\\resourceMap4.bmp";
	//ss << SOLUTION_DIR << "common\\assets\\textures\\resourceMap5.bmp";
	//ss << SOLUTION_DIR << "common\\assets\\textures\\resourceMap6.bmp";
	bmp = new BMPImage(ss.str());
	//bmp = new BMPImage("resourceMap.bmp");

	//MakeGraphAndMeshes();

	//system("pause");

	ss.str("");

	// ********************************************************************************************

	glfwSetErrorCallback(GLFW_error_callback);

	if (!glfwInit())
	{
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	pWindow = glfwCreateWindow(1200, 640, "GDP Game Jam!", NULL, NULL);

	if (!pWindow)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetKeyCallback(pWindow, GLFW_key_callback);

	glfwSetCursorEnterCallback(pWindow, GLFW_cursor_enter_callback);
	glfwSetCursorPosCallback(pWindow, GLFW_cursor_position_callback);
	glfwSetScrollCallback(pWindow, GLFW_scroll_callback);
	glfwSetMouseButtonCallback(pWindow, GLFW_mouse_button_callback);
	glfwSetWindowSizeCallback(pWindow, GLFW_window_size_callback);

	// These are to hide the mouse cursor
	glfwSetCursorPosCallback(pWindow, mouse_callback);

	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwMakeContextCurrent(pWindow);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	glfwSwapInterval(1);


	GLint max_uniform_location = 0;
	GLint* p_max_uniform_location = NULL;
	p_max_uniform_location = &max_uniform_location;
	glGetIntegerv(GL_MAX_UNIFORM_LOCATIONS, p_max_uniform_location);

	std::cout << "GL_MAX_UNIFORM_LOCATIONS: " << max_uniform_location << std::endl;

	// non-issue
	GLint max_vertex_uniform_components = 0;
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &max_vertex_uniform_components);
	std::cout << "GL_MAX_VERTEX_UNIFORM_COMPONENTS: " << max_vertex_uniform_components << std::endl;

	GLint max_fragment_uniform_components = 0;
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &max_fragment_uniform_components);
	std::cout << "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS: " << max_fragment_uniform_components << std::endl;

	GLint max_compute_uniform_components = 0;
	glGetIntegerv(GL_MAX_COMPUTE_UNIFORM_COMPONENTS, &max_compute_uniform_components);
	std::cout << "GL_MAX_COMPUTE_UNIFORM_COMPONENTS: " << max_compute_uniform_components << std::endl;

	GLint max_geometry_uniform_components = 0;
	glGetIntegerv(GL_MAX_GEOMETRY_UNIFORM_COMPONENTS, &max_geometry_uniform_components);
	std::cout << "GL_MAX_GEOMETRY_UNIFORM_COMPONENTS: " << max_geometry_uniform_components << std::endl;

	// Create global things
	::g_StartUp(pWindow);

	// This is where Feeney sets up animation stuff

	// Then camera

	// THEN shader stuff
	cShaderManager::cShader vertShader;
	ss.str("");
	ss << SOLUTION_DIR << "common\\assets\\shaders\\vertShader_01.glsl";
	vertShader.fileName = ss.str();

	cShaderManager::cShader geomShader;
	ss.str("");
	ss << SOLUTION_DIR << "common\\assets\\shaders\\geomShader.glsl";
	geomShader.fileName = ss.str();;

	cShaderManager::cShader fragShader;
	ss.str("");
	ss << SOLUTION_DIR << "common\\assets\\shaders\\fragShader_01.glsl";
	fragShader.fileName = ss.str();

	if (::g_pShaderManager->createProgramFromFile("Shader#1", vertShader, geomShader, fragShader))
	{
		std::cout << "Shader compiled OK" << std::endl;
		// 
		// Set the "program" variable to the one the Shader Manager used...
		program = ::g_pShaderManager->getIDFromFriendlyName("Shader#1");
	}
	else
	{
		std::cout << "Error making shader program: " << std::endl;
		std::cout << ::g_pShaderManager->getLastError() << std::endl;
	}

	// Select the shader program we want to use
	// (Note we only have one shader program at this point)
	glUseProgram(program);


	// *******************************************************
	// Now, I'm going to "load up" all the uniform locations
	// (This was to show how a map could be used)
	//cShaderManager::cShaderProgram* pShaderProc = ::g_pShaderManager->pGetShaderProgramFromFriendlyName("Shader#1");
	pShaderProc = ::g_pShaderManager->pGetShaderProgramFromFriendlyName("Shader#1");
	//int theUniformIDLoc = -1;
	//theUniformIDLoc = glGetUniformLocation(program, "matModel");

	//pShaderProc->mapUniformName_to_UniformLocation["matModel"] = theUniformIDLoc;

	// Or...
	pShaderProc->mapUniformName_to_UniformLocation["matModel"] = glGetUniformLocation(program, "matModel");

	pShaderProc->mapUniformName_to_UniformLocation["matView"] = glGetUniformLocation(program, "matView");
	pShaderProc->mapUniformName_to_UniformLocation["matProjection"] = glGetUniformLocation(program, "matProjection");
	pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"] = glGetUniformLocation(program, "matModelInverseTranspose");

	pShaderProc->mapUniformName_to_UniformLocation["bDiscardTransparencyWindowsOn"] = glGetUniformLocation(program, "bDiscardTransparencyWindowsOn");

	pShaderProc->mapUniformName_to_UniformLocation["MVP"] = glGetUniformLocation(program, "MVP");

	pShaderProc->mapUniformName_to_UniformLocation["renderPassNumber"] = glGetUniformLocation(program, "renderPassNumber");

	pShaderProc->mapUniformName_to_UniformLocation["bDrawDebugNormals"] = glGetUniformLocation(program, "bDrawDebugNormals");

	pShaderProc->mapUniformName_to_UniformLocation["debugNormalLength"] = glGetUniformLocation(program, "debugNormalLength");

	pShaderProc->mapUniformName_to_UniformLocation["bIsSkyBox"] = glGetUniformLocation(program, "bIsSkyBox");

	pShaderProc->mapUniformName_to_UniformLocation["discardTexture"] = glGetUniformLocation(program, "discardTexture");

	pShaderProc->mapUniformName_to_UniformLocation["screenWidthHeight"] = glGetUniformLocation(program, "screenWidthHeight");

	pShaderProc->mapUniformName_to_UniformLocation["texVertexMaterialColour"] = glGetUniformLocation(program, "texVertexMaterialColour");

	pShaderProc->mapUniformName_to_UniformLocation["texVertexNormal"] = glGetUniformLocation(program, "texVertexNormal");

	pShaderProc->mapUniformName_to_UniformLocation["texVertexWorldPos"] = glGetUniformLocation(program, "texVertexWorldPos");

	pShaderProc->mapUniformName_to_UniformLocation["texVertexSpecular"] = glGetUniformLocation(program, "texVertexSpecular");

	pShaderProc->mapUniformName_to_UniformLocation["texLightPassColourBuffer"] = glGetUniformLocation(program, "texLightPassColourBuffer");

	pShaderProc->mapUniformName_to_UniformLocation["texScope"] = glGetUniformLocation(program, "texScope");

	pShaderProc->mapUniformName_to_UniformLocation["texWindow"] = glGetUniformLocation(program, "texWindow");

	pShaderProc->mapUniformName_to_UniformLocation["twoDEffectOperator"] = glGetUniformLocation(program, "twoDEffectOperator");

	// DrawObject uniform locations
	// Textures
	pShaderProc->mapUniformName_to_UniformLocation["textureOperator"] = glGetUniformLocation(program, "textureOperator");
	pShaderProc->mapUniformName_to_UniformLocation["texture_00"] = glGetUniformLocation(program, "texture_00");
	pShaderProc->mapUniformName_to_UniformLocation["texture_01"] = glGetUniformLocation(program, "texture_01");
	pShaderProc->mapUniformName_to_UniformLocation["texture_02"] = glGetUniformLocation(program, "texture_02");
	pShaderProc->mapUniformName_to_UniformLocation["texture_03"] = glGetUniformLocation(program, "texture_03");
	pShaderProc->mapUniformName_to_UniformLocation["texture_04"] = glGetUniformLocation(program, "texture_04");
	pShaderProc->mapUniformName_to_UniformLocation["texture2DRatios0to3"] = glGetUniformLocation(program, "texture2DRatios0to3");
	pShaderProc->mapUniformName_to_UniformLocation["texture2DRatios4to7"] = glGetUniformLocation(program, "texture2DRatios4to7");
	pShaderProc->mapUniformName_to_UniformLocation["cubeMap_00"] = glGetUniformLocation(program, "cubeMap_00");
	pShaderProc->mapUniformName_to_UniformLocation["cubeMap_01"] = glGetUniformLocation(program, "cubeMap_01");
	pShaderProc->mapUniformName_to_UniformLocation["cubeMap_02"] = glGetUniformLocation(program, "cubeMap_02");
	pShaderProc->mapUniformName_to_UniformLocation["cubeMap_03"] = glGetUniformLocation(program, "cubeMap_03");
	pShaderProc->mapUniformName_to_UniformLocation["cubeMap_Ratios0to3"] = glGetUniformLocation(program, "cubeMap_Ratios0to3");
	// DrawObject
	pShaderProc->mapUniformName_to_UniformLocation["bIsImposter"] = glGetUniformLocation(program, "bIsImposter");
	pShaderProc->mapUniformName_to_UniformLocation["wholeObjectAlphaTransparency"] = glGetUniformLocation(program, "wholeObjectAlphaTransparency");
	pShaderProc->mapUniformName_to_UniformLocation["bUseDebugColour"] = glGetUniformLocation(program, "bUseDebugColour");
	pShaderProc->mapUniformName_to_UniformLocation["objectDebugColour"] = glGetUniformLocation(program, "objectDebugColour");
	pShaderProc->mapUniformName_to_UniformLocation["bDontLightObject"] = glGetUniformLocation(program, "bDontLightObject");
	pShaderProc->mapUniformName_to_UniformLocation["wholeObjectDiffuseColour"] = glGetUniformLocation(program, "wholeObjectDiffuseColour");
	pShaderProc->mapUniformName_to_UniformLocation["bUseWholeObjectDiffuseColour"] = glGetUniformLocation(program, "bUseWholeObjectDiffuseColour");
	pShaderProc->mapUniformName_to_UniformLocation["wholeObjectSpecularColour"] = glGetUniformLocation(program, "wholeObjectSpecularColour");
	


	//pShaderProc->mapUniformName_to_UniformLocation["wholeObjectSpecularColour"] = glGetUniformLocation(program, "wholeObjectSpecularColour");
	// .. and so on...
	//pShaderProc->mapUniformName_to_UniformLocation["theLights[0].position"] = glGetUniformLocation(program, "wholeObjectSpecularColour");

	// *******************************************************

	// TODO: GO through all these and load them into the map instead, and then go through the errors and pull the location from the map instead
	//GLint mvp_location = -1;
	//mvp_location = glGetUniformLocation(program, "MVP");

	// Get "uniform locations" (aka the registers these are in)
	//GLint matModel_Location = glGetUniformLocation(program, "matModel");
	//	GLint matView_Location = glGetUniformLocation(program, "matView");
	//GLint matProjection_Location = glGetUniformLocation(program, "matProjection");
	//GLint matModelInverseTranspose_Location = glGetUniformLocation(program, "matModelInverseTranspose");

	//GLint bDiscardTransparencyWindowsON_LocID = glGetUniformLocation(program, "bDiscardTransparencyWindowsOn");


	//    _      _____ _____ _    _ _______ _____ 
	//   | |    |_   _/ ____| |  | |__   __/ ____|
	//   | |      | || |  __| |__| |  | | | (___  
	//   | |      | || | |_ |  __  |  | |  \___ \ 
	//   | |____ _| || |__| | |  | |  | |  ____) |
	//   |______|_____\_____|_|  |_|  |_| |_____/ 
	//                                            
         
	//Lights stuff here

//    	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
//	                // 0 = pointlight
//					// 1 = spot light
//					// 2 = directional light

	// Large scene light	CAN'T BE INDEX 0!!!!  Index 0 seems to overwrite all other lights, so it can be the spotlight or something, not sure why,
	// might be something in the shader

	// loads all the lights from a file instead of individually setting them here
	if (loadLightsFile())
	{
		std::cout << "loadLightsFile finished ok!" << std::endl;
	}
	else
	{
		std::cout << "loadLightsFile did NOT finish ok!  Aborting!" << std::endl;
		return -1;
	}

	// Get the uniform locations of the light shader values
	::g_pTheLights->SetUpUniformLocations(program);

	// Then some entity organizer stuff

	// Then the debug sphere (that I'm not including)

	// Then setting up the Manager and loading models
	double modelLoadStartTime = glfwGetTime();

	ss.str("");
	//ss << SOLUTION_DIR << "common\\assets\\models\\";
	ss << SOLUTION_DIR << "common\\assets\\models\\";

	::g_pVAOManager->setFilePath(ss.str());

	// Load ALL the models
	//loadGraphicsMidtermModels();
	//loadGraphicsProject2Models();
	//loadAIModels();
	loadGameJamModels();

	//makeSpaceStation();

	// and instead of this loop, this is where we would do the threaded loading
	unsigned int totalVerticesLoaded = 0;
	unsigned int totalTrianglesLoaded = 0;
	for (std::vector<std::string>::iterator itModel = modelLocations.begin(); itModel != modelLocations.end(); itModel++)
	{
		sModelDrawInfo theModel;
		std::string modelName = *itModel;
		std::cout << "Loading " << modelName << "...";
		if (!::g_pVAOManager->LoadModelIntoVAO(modelName, theModel, program))
		{
			std::cout << "didn't work because: " << std::endl;
			std::cout << ::g_pVAOManager->getLastError(true) << std::endl;
		}
		else
		{
			std::cout << "OK." << std::endl;
			std::cout << "\t" << theModel.numberOfVertices << " vertices and " << theModel.numberOfTriangles << " triangles loaded." << std::endl;
			totalTrianglesLoaded += theModel.numberOfTriangles;
			totalVerticesLoaded += theModel.numberOfVertices;
		}
	} //end of for (std::vector<std::string>::iterator itModel

	double modelLoadEndTime = glfwGetTime();

	std::cout << "Done loading models. Time: " << modelLoadEndTime - modelLoadStartTime << std::endl;
	std::cout << "Total vertices loaded = " << totalVerticesLoaded << std::endl;
	std::cout << "Total triangles loaded = " << totalTrianglesLoaded << std::endl;

	//    __  __ ______  _____ _    _ ______  _____           __          ______  _____  _      _____  
	//   |  \/  |  ____|/ ____| |  | |  ____|/ ____|   ___    \ \        / / __ \|  __ \| |    |  __ \ 
	//   | \  / | |__  | (___ | |__| | |__  | (___    ( _ )    \ \  /\  / / |  | | |__) | |    | |  | |
	//   | |\/| |  __|  \___ \|  __  |  __|  \___ \   / _ \/\   \ \/  \/ /| |  | |  _  /| |    | |  | |
	//   | |  | | |____ ____) | |  | | |____ ____) | | (_>  <    \  /\  / | |__| | | \ \| |____| |__| |
	//   |_|  |_|______|_____/|_|  |_|______|_____/   \___/\/     \/  \/   \____/|_|  \_\______|_____/ 
	//                                                                                                 
	//        

	if (loadTSVGrid())
	{
		std::cout << "loadTSVGrid finished OK" << std::endl;
	}
	else
	{
		std::cout << "loadTSVGrid did not finish OK, aborting!" << std::endl;
		return -1;
	}

	////World file stuff here
	if (loadWorldFile())
	{
		std::cout << "loadWorldFile finished OK" << std::endl;
	}
	else
	{
		std::cout << "loadWorldFile did not finish OK, aborting" << std::endl;
		return -1;
	}

	// Normally this would be in a builder or something, but I'm still using my old WorldFile.txt
	// So I'm doing it a little HACKy way
	
	
	// also, I didn't realize the "screens" were already placed in the right spots (at least for the main console)
	// and I just figured out where to move them to in mesh lab and applied the transform here
	
	

	//    _______ ________   _________ _    _ _____  ______  _____ 
	//   |__   __|  ____\ \ / /__   __| |  | |  __ \|  ____|/ ____|
	//      | |  | |__   \ V /   | |  | |  | | |__) | |__  | (___  
	//      | |  |  __|   > <    | |  | |  | |  _  /|  __|  \___ \ 
	//      | |  | |____ / . \   | |  | |__| | | \ \| |____ ____) |
	//      |_|  |______/_/ \_\  |_|   \____/|_|  \_\______|_____/ 
	//                                                             
	//    

	// non-issue
	GLint glMaxCombinedTextureImageUnits = 0;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &glMaxCombinedTextureImageUnits);
	std::cout << "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS = " << glMaxCombinedTextureImageUnits << std::endl;

	GLint glMaxVertexTextureImageUnits = 0;
	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &glMaxVertexTextureImageUnits);
	std::cout << "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS = " << glMaxVertexTextureImageUnits << std::endl;

	GLint glMaxTextureSize = 0;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTextureSize);
	std::cout << "GL_MAX_TEXTURE_SIZE = " << glMaxTextureSize << std::endl;

	//Michael Feeney's BMP texture mapping
	//// Load the textures
	ss.str("");
	ss << SOLUTION_DIR << "common\\assets\\textures\\";
	::g_pTextureManager->SetBasePath(ss.str());


	if (::g_pTextureManager->Create2DTextureFromBMPFile("Scope.bmp", true))
	{
		std::cout << "Loaded Scope texture" << std::endl;
	}
	else
	{
		std::cout << "DIDN'T load Scope Texture" << std::endl;
	}

	if (::g_pTextureManager->Create2DTextureFromBMPFile("Window.bmp", true))
	{
		std::cout << "Loaded Window texture" << std::endl;
	}
	else
	{
		std::cout << "DIDN'T load Window texture" << std::endl;
	}

	
	// Default don't have a texture
	if (::g_pTextureManager->Create2DTextureFromBMPFile("BrightColouredUVMap.bmp", true))
		//if (::g_pTextureManager->Create2DTextureFromBMPFile("Pebbleswithquarzite.bmp", true))
	{
		std::cout << "Loaded Bright Coloured texture" << std::endl;
	}
	else
	{
		std::cout << "DIDN'T load Bright Coloured texture" << std::endl;
	}

	if (::g_pTextureManager->Create2DTextureFromBMPFile("Long_blue_Jet_Flame.bmp", true))
	{
		std::cout << "Loaded long blue jet flame texture" << std::endl;
	}
	else
	{
		std::cout << "DIDN'T load long blue jet flame texture" << std::endl;
	}

	ss << "DFK Textures\\";
	::g_pTextureManager->SetBasePath(ss.str());
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_bed_single_01_basecolor.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_bed_single_01_normal.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_bed_single_01_SmMetAO.bmp", true);

	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_ceiling_stone_basecolor.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_ceiling_stone_normal.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_ceiling_stone_SmMetAO.bmp", true);

	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_doors_01_basecolor.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_doors_01_normal.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_doors_01_SmMetAO.bmp", true);

	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_floor_01_basecolor.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_floor_01_normal.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_floor_01_SmMetAO.bmp", true);

	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_stairs_01_basecolor.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_stairs_01_normal.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_stairs_01_SmMetAO.bmp", true);

	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_stairs_wood_01_albedo.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_stairs_wood_01_normal.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_stairs_wood_01_metallic.bmp", true);

	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_wall_a_basecolor.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_wall_a_normal.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_wall_a_SmMetAO.bmp", true);

	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_doorframe_basecolor.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_doorframe_normal.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_doorframe_SmMetAO.bmp", true);

	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_gate_01_basecolor.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_gate_01_normal.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_gate_01_SmMetAO.bmp", true);

	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_candleholders_01_basecolor.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_candleholders_01_normal.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("dfk_candleholders_01_SmMetAO.bmp", true);

	
	// Add a skybox texture
	std::string errorTextString;
	ss.str("");
	ss << SOLUTION_DIR << "common\\assets\\textures\\cubemaps\\";
	::g_pTextureManager->SetBasePath(ss.str());		// update base path to cube texture location

	if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles("TropicalSunnyDay",
		"TropicalSunnyDayRight2048.bmp",	/* +X */	"TropicalSunnyDayLeft2048.bmp" /* -X */,
		"TropicalSunnyDayUp2048.bmp",		/* +Y */	"TropicalSunnyDayDown2048.bmp" /* -Y */,
		"TropicalSunnyDayFront2048.bmp",	/* +Z */	"TropicalSunnyDayBack2048.bmp" /* -Z */,
		true, errorTextString))
	{
		std::cout << "Didn't load because: " << errorTextString << std::endl;
	}


	if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles("Space01",
		"SpaceBox_right1_posX.bmp",		/* +X */	"SpaceBox_left2_negX.bmp" /* -X */,
		"SpaceBox_top3_posY.bmp",		/* +Y */	"SpaceBox_bottom4_negY.bmp" /* -Y */,
		"SpaceBox_front5_posZ.bmp",		/* +Z */	"SpaceBox_back6_negZ.bmp" /* -Z */,
		true, errorTextString))
	{
		std::cout << "Didn't load because: " << errorTextString << std::endl;
	}

	if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles("City01",
		"city_lf.bmp",		/* +X */	"city_rt.bmp" /* -X */,
		"city_dn.bmp",		/* +Y */	"city_up.bmp" /* -Y */,
		"city_ft.bmp",		/* +Z */	"city_bk.bmp" /* -Z */,
		true, errorTextString))
	{
		std::cout << "Didn't load because: " << errorTextString << std::endl;
	}

	//// Turns out to make a "cube" map, the images need to be squares
	if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles("Skyrim",
		"Skyrim_Right.bmp",		/* +X */	"Skyrim_Left.bmp" /* -X */,
		"Skyrim_Top.bmp",		/* +Y */	"Skyrim_Bottom.bmp" /* -Y */,
		"Skyrim_Forward.bmp",		/* +Z */	"Skyrim_Back.bmp" /* -Z */,
		true, errorTextString))
	{
		std::cout << "Didn't load because: " << errorTextString << std::endl;
	}

	// Make sphere for "skybox" before world file
	cMesh* sphereSky = new cMesh();
	//sphereSky->meshName = "Isosphere_Smooth_Normals.ply";
	// We are using a sphere with INWARD facing normals, so we see the "back" of the sphere
	sphereSky->meshName = "Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply";
	// 2 main ways we can do a skybox:
	//
	// - Make a sphere really big, so everything fits inside
	// (be careful of the far clipping plane)
	//
	// - Typical way is:
	//	- Turn off the depth test
	//	- Turn off the depth function (i.e. writing to the depth buffer)
	//	- Draw the skybox object (which can be really small, since it's not interacting with the depth buffer)
	//	- Once drawn:
	//		- Turn on the depth function
	//		- Turn on the depth test

	sphereSky->positionXYZ = ::cameraEye;	//glm::vec3(100.0f, -250.0f, 100.0f);
	//sphereSky->orientationXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	sphereSky->scale = glm::vec3(10000.0f);
	//sphereSky->bUseWholeObjectDiffuseColour = false;
	//sphereSky->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	sphereSky->bDontLight = true;
	sphereSky->clearTextureRatiosToZero();
	//sphereSky->textureNames[1] = "2k_jupiter.bmp";
	sphereSky->textureRatios[1] = 1.0f;
	//Draw it separately so don't add it to the vector
	//::g_vec_pMeshes.push_back(sphereSky);

	// My camera reset stuff	(might be broken, idk anymore)
	if (::cameraEye.x > 0 && ::cameraEye.z > 0)
	{
		::cameraYaw = 180.f + (atan(::cameraEye.z / ::cameraEye.x) * 180.f / glm::pi<float>());
	}
	else if (::cameraEye.x > 0 && ::cameraEye.z < 0)
	{
		::cameraYaw = 90.f - (atan(::cameraEye.z / ::cameraEye.x) * 180.f / glm::pi<float>());
	}
	else if (::cameraEye.x < 0 && ::cameraEye.z > 0)
	{
		::cameraYaw = (atan(::cameraEye.z / ::cameraEye.x) * 180.f / glm::pi<float>());
	}
	else if (::cameraEye.x < 0 && ::cameraEye.z < 0)
	{
		::cameraYaw = (atan(::cameraEye.z / ::cameraEye.x) * 180.f / glm::pi<float>());
	}
	else if (::cameraEye.x == 0.f)
	{
		if (::cameraEye.z >= 0.f)
		{
			::cameraYaw = 270.f;
		}
		else
		{
			::cameraYaw = 90.f;
		}
	}
	else if (::cameraEye.z == 0.f)
	{
		if (::cameraEye.x <= 0)
		{
			::cameraYaw = 0.f;
		}
		else
		{
			::cameraYaw = 180.f;
		}
	}
	//anyways, after figuring out the yaw, we set the target at the negative of the xz of the ::camera position and y=0 (this faces the ::camera towards the origin)
	::cameraTarget = glm::vec3(-1.f * ::cameraEye.x, 0, -1.f * ::cameraEye.z);
	glm::normalize(::cameraTarget);

	const double MAX_DELTA_TIME = 0.1;  // 100 ms
	double previousTime = glfwGetTime();


	//    ______ ____   ____  
	//   |  ____|  _ \ / __ \ 
	//   | |__  | |_) | |  | |
	//   |  __| |  _ <| |  | |
	//   | |    | |_) | |__| |
	//   |_|    |____/ \____/ 
	//                        
	//    

	// Create the FBO (Frame Buffer Object)
	// The texture we can render to
	::g_pFBO = new cFBO();
	// Set this off screen texture buffer to some random size
	std::string FBOerrorString;
	   // if (::g_pFBO->init(1024, 1024, FBOerrorString))
	    if (::g_pFBO->init(1200, 640, FBOerrorString))
	//    if (::g_pFBO->init( 8 * 1024, 8 * 1024, FBOerrorString))
	//if (::g_pFBO->init(128, 64, FBOerrorString))
	{
		std::cout << "FBO is all set!" << std::endl;
	}
	else
	{
		std::cout << "FBO Error: " << FBOerrorString << std::endl;
	}

	// Clear the OG back buffer once, BEFORE we render anything
	float ratio;
	int width, height;
	glfwGetFramebufferSize(pWindow, &width, &height);
	ratio = width / (float)height;
	glViewport(0, 0, width, height);
	glClearColor(0.0f, 164.0f / 255.0f, 239.0f / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	enum eRenderPasses
	{
		PASS_1_G_BUFFER_PASS = 1,	// Renders only geometry to G-Buffer
		PASS_2_LIGHT_PASS = 2,		// Apply lighting to G-Buffer
		PASS_3_2D_EFFECTS_PASS = 3		// Optional effects (blur, whatever...)
	};

	//const GLint RENDER_PASS_0_ENTIRE_SCENE = 0;
	//const GLint RENDER_PASS_1_QUAD_ONLY = 1;   
	//GLint renderPassNumber_LocID = glGetUniformLocation(program, "renderPassNumber");

	// Feeney put the dalek stuff here

	//    __  __          _____ _   _   _      ____   ____  _____  
	//   |  \/  |   /\   |_   _| \ | | | |    / __ \ / __ \|  __ \ 
	//   | \  / |  /  \    | | |  \| | | |   | |  | | |  | | |__) |
	//   | |\/| | / /\ \   | | | . ` | | |   | |  | | |  | |  ___/ 
	//   | |  | |/ ____ \ _| |_| |\  | | |___| |__| | |__| | |     
	//   |_|  |_/_/    \_\_____|_| \_| |______\____/ \____/|_|     
	//                                                             
	//   
	while (!glfwWindowShouldClose(pWindow))
	{
		//glUniform1ui(renderPassNumber_LocID, PASS_1_G_BUFFER_PASS);
		glUniform1ui(pShaderProc->mapUniformName_to_UniformLocation["renderPassNumber"], PASS_1_G_BUFFER_PASS);
		//float ratio;
		//int width, height;
		glm::mat4 matModel;             // used to be "m"; Sometimes it's called "world"
		glm::mat4 matProjection;        // used to be "p";
		glm::mat4 matView;              // used to be "v";

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - previousTime;
		deltaTime = (deltaTime > MAX_DELTA_TIME ? MAX_DELTA_TIME : deltaTime);
		previousTime = currentTime;


		//testEntity->Update(deltaTime);
		for (unsigned int index = 0; index != vec_pFSMEntities.size(); index++)
		{
			vec_pFSMEntities[index]->Update(deltaTime);
		}

		// this was where I made the other FBO objects, deleted for AI

		
		
		// Set pass to #0
		//glUniform1ui(renderPassNumber_LocID, PASS_1_G_BUFFER_PASS);
		glUniform1ui(pShaderProc->mapUniformName_to_UniformLocation["renderPassNumber"], PASS_1_G_BUFFER_PASS);

		// Set the output of the renderer to the screen (default FBO)
		GLuint FBO_ID = ::g_pFBO->ID;
		glBindFramebuffer(GL_FRAMEBUFFER, ::g_pFBO->ID);

		// Set the viewport to the size of my offscreen texture (FBO)
		glViewport(0, 0, ::g_pFBO->width, ::g_pFBO->height);
		ratio = ::g_pFBO->width / (float)::g_pFBO->height;

		// ^^^
		//glfwGetFramebufferSize(pWindow, &width, &height);
		//ratio = width / (float)height;

		// Turn on the depth buffer
		glEnable(GL_DEPTH);         // Turns on the depth buffer
		glEnable(GL_DEPTH_TEST);    // Check if the pixel is already closer

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glViewport(0, 0, width, height);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		::g_pFBO->clearBuffers(true, true);


		//        glfwGetFramebufferSize(pWindow, &width, &height);
		//        ratio = width / (float)height;
		//
		//        glViewport(0, 0, width, height);
		//
		//        // Set clear colour to black
		//        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f);
		//        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

				// *******************************************************
				// Screen is cleared and we are ready to draw the scene...
				// *******************************************************

				// Update the title text
		glfwSetWindowTitle(pWindow, ::g_TitleText.c_str());


		// Debug sphere here

		// Let's make the two torches flicker
		// Indexes 16 and 17
		//randomAddedAtten = (rand() % 100) / 100.0f + 1.0f;
		//::g_pTheLights->theLights[16].atten.y = baseAtten + randomAddedAtten * gGetRandBetween<float>(1.0f, 2.0f);
		//::g_pTheLights->theLights[17].atten.y = baseAtten + randomAddedAtten * gGetRandBetween<float>(1.0f, 2.0f);

		// So there's gunna be a LOT more lights then just the two.
		// and also, don't need to call sRand every frame
		for (unsigned int index = 0; index != ::vec_pTorches.size(); index++)
		{
			::vec_pTorches[index]->Update(deltaTime);
		}

		// Copy the light information into the shader to draw the scene
		::g_pTheLights->CopyLightInfoToShader();

		// I need to update this eventually to be variables we can change
		matProjection = glm::perspective(0.6f,	// FOV variable later
			ratio,
			0.01f,								// Near plane
			1'000'000.0f);						// Far plane

		// camera update
		// Feeney grabs the fly camera information for the matView, I have it stored as globals

		matView = glm::mat4(1.0f);

		if (::g_scene)
		{
			matView = glm::lookAt(glm::vec3(-2.5f, 4.5f, 12.5f),
				glm::vec3(-2.5f, 4.5f, 12.5f) + glm::vec3(0.01f, -0.3f, -0.95f),
				upVector);
		}
		else
		{
			matView = glm::lookAt(::cameraEye,
				::cameraEye + ::cameraTarget,
				upVector);
		}
		

		// this might be why we're getting flickering, pShaderProc is made twice
		//cShaderManager::cShaderProgram* pShaderProc = ::g_pShaderManager->pGetShaderProgramFromFriendlyName("Shader#1");

		glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
			1, GL_FALSE, glm::value_ptr(matView));


		//glUniformMatrix4fv(matProjection_Location, 1, GL_FALSE, glm::value_ptr(matProjection));
		glUniformMatrix4fv(pShaderProc->mapUniformName_to_UniformLocation["matProjection"], 1, GL_FALSE, glm::value_ptr(matProjection));

		// ***************************************
		// Geometry shader generate normals
		//  uniform bool bDrawNormals;
		//  uniform float debugNormalLength;

		//GLint bDrawDebugNormals_LocID = glGetUniformLocation(program, "bDrawDebugNormals");
		if (::g_bShowDebugObjects)
		{
			//glUniform1f(bDrawDebugNormals_LocID, (GLfloat)GL_TRUE);
			glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bDrawDebugNormals"], (GLfloat)GL_TRUE);

			//GLint debugNormalLength_LocID = glGetUniformLocation(program, "debugNormalLength");
			//glUniform1f(debugNormalLength_LocID, 10.0f);
			//glUniform1f(debugNormalLength_LocID, 0.1f);
			glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["debugNormalLength"], 0.1f);
		}
		else
		{
			//glUniform1f(bDrawDebugNormals_LocID, (GLfloat)GL_FALSE);
			glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bDrawDebugNormals"], (GLfloat)GL_FALSE);
		}
		// ***************************************


		// Animation control update, then ship update

		// *********************************************************************
		//    ____  _            ____             
		//   / ___|| | ___   _  | __ )  _____  __ 
		//   \___ \| |/ / | | | |  _ \ / _ \ \/ / 
		//    ___) |   <| |_| | | |_) | (_) >  <  
		//   |____/|_|\_\\__, | |____/ \___/_/\_\ 
		//               |___/                    
		//
		// Since this is a space game (most of the screen is filled with "sky"), 
		//  I'll draw the skybox first
		// 
		// This is updated to the "proper" way to do a skybox. 
		// Here's how:
		// 1. Turn off the depth function AND depth test
		// 2. Draw the skybox - it doesn't matter how big it is, since it's the 1st thing being drawn.
		//    Since I'm NOT writing to the depth buffer, it's written to the colour buffer 
		//    (i.e. we can see it), but anything drawn later doesn't "know" that's happened, 
		//    so all the other objects draw "on top" of it (on the colour buffer)
		// 
		//GLint bIsSkyBox_LocID = glGetUniformLocation(program, "bIsSkyBox");
		//glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_TRUE);
		glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bIsSkyBox"], (GLfloat)GL_TRUE);

		glDisable(GL_DEPTH_TEST);     // Disable depth test (always write to colour buffer)
		glDepthMask(GL_FALSE);          // Disable write to depth buffer
		// Move the "skybox object" with the camera
		sphereSky->positionXYZ = ::cameraEye;
		//DrawObject(sphereSky, glm::mat4(1.0f),
		//	matModel_Location, matModelInverseTranspose_Location,
		//	program, ::g_pVAOManager);

		DrawObject(sphereSky, glm::mat4(1.0f),
			pShaderProc->mapUniformName_to_UniformLocation["matModel"],
			pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"],
			program, 
			::g_pVAOManager);

		//glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_FALSE);
		glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bIsSkyBox"], (GLfloat)GL_FALSE);
		//
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		// *********************************************************************

		//             _      _____  _    _            _______ _____            _   _  _____ _____        _____  ______ _   _  _______     __
		//       /\   | |    |  __ \| |  | |   /\     |__   __|  __ \     /\   | \ | |/ ____|  __ \ /\   |  __ \|  ____| \ | |/ ____\ \   / /
		//      /  \  | |    | |__) | |__| |  /  \       | |  | |__) |   /  \  |  \| | (___ | |__) /  \  | |__) | |__  |  \| | |     \ \_/ / 
		//     / /\ \ | |    |  ___/|  __  | / /\ \      | |  |  _  /   / /\ \ | . ` |\___ \|  ___/ /\ \ |  _  /|  __| | . ` | |      \   /  
		//    / ____ \| |____| |    | |  | |/ ____ \     | |  | | \ \  / ____ \| |\  |____) | |  / ____ \| | \ \| |____| |\  | |____   | |   
		//   /_/    \_\______|_|    |_|  |_/_/    \_\    |_|  |_|  \_\/_/    \_\_| \_|_____/|_| /_/    \_\_|  \_\______|_| \_|\_____|  |_|   
		//                                                                                                                                   
		//   
		//For transparency for the beakers, they need to be drawn farthest from the camera first, so we need to sort the last 4 meshes
		// Alpha transparency sorting
		//for (unsigned int index = ::g_vec_pMeshes.size() - numberOfTransparentObjects; index != g_vec_pMeshes.size() - 1; index++)
		//{
		//	//Distance is the difference in length between camera eye and mesh position
		//	float dist1, dist2;
		//	dist1 = glm::abs(glm::length(cameraEye - ::g_vec_pMeshes[index]->positionXYZ));
		//	dist2 = glm::abs(glm::length(cameraEye - ::g_vec_pMeshes[index + 1]->positionXYZ));
		//	//if the second object is farther, move it up
		//	if (dist2 > dist1)
		//	{
		//		cMesh* temp = ::g_vec_pMeshes[index + 1];
		//		::g_vec_pMeshes[index + 1] = ::g_vec_pMeshes[index];
		//		::g_vec_pMeshes[index] = temp;
		//	}
		//}



		// **********************************************************************
		// Draw the "scene" of all objects.
		// i.e. go through the vector and draw each one...
		// **********************************************************************
		//    _____  _____       __          __   _____  _____ ______ _   _ ______     _  _  __ 
		//   |  __ \|  __ \     /\ \        / /  / ____|/ ____|  ____| \ | |  ____|  _| || |/_ |
		//   | |  | | |__) |   /  \ \  /\  / /  | (___ | |    | |__  |  \| | |__    |_  __  _| |
		//   | |  | |  _  /   / /\ \ \/  \/ /    \___ \| |    |  __| | . ` |  __|    _| || |_| |
		//   | |__| | | \ \  / ____ \  /\  /     ____) | |____| |____| |\  | |____  |_  __  _| |
		//   |_____/|_|  \_\/_/    \_\/  \/     |_____/ \_____|______|_| \_|______|   |_||_| |_|
		//                                                                                      
		//    

		for (unsigned int index = 0; index != vec_pFSMEntities.size(); index++)
		{
			matModel = glm::mat4(1.0f);
			//DrawObject(vec_pFSMEntities[index]->m_Mesh,
			//	matModel,
			//	matModel_Location,
			//	matModelInverseTranspose_Location,
			//	program,
			//	::g_pVAOManager);

			DrawObject(vec_pFSMEntities[index]->m_Mesh,
				matModel,
				pShaderProc->mapUniformName_to_UniformLocation["matModel"],
				pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"],
				program,
				::g_pVAOManager);
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

				DrawObject(pCurrentMesh,
					matModel,
					pShaderProc->mapUniformName_to_UniformLocation["matModel"],
					pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"],
					program,
					::g_pVAOManager);
			}


		}//for (unsigned int index


		


		// Draw Debug objects

		// Scene is drawn
		// **********************************************************************

		// At this point, the "scene" is done drawing.
		// i.e. we've drawn all the stuff we want to see.
		// 
		//    _____  _   _  ____            __   _       _                            
		//   | ____|| \ | ||  _ \    ___   / _| / | ___ | |_   _ __    __ _  ___  ___ 
		//   |  _|  |  \| || | | |  / _ \ | |_  | |/ __|| __| | '_ \  / _` |/ __|/ __|
		//   | |___ | |\  || |_| | | (_) ||  _| | |\__ \| |_  | |_) || (_| |\__ \\__ \
        //   |_____||_| \_||____/   \___/ |_|   |_||___/ \__| | .__/  \__,_||___/|___/
		//                                                    |_|                     
		// **********************************************************************


		if (::g_updateFBOResolution)
		{
			std::string errorString;
			glfwGetFramebufferSize(pWindow, &width, &height);
			if (!::g_pFBO->reset(width, height, errorString))
			{
				std::cout << errorString << std::endl;
			}
			::g_updateFBOResolution = false;
		}


		//    ____      _     _       _     _   _               ____               
		//   |___ \ _  | |   (_) __ _| |__ | |_(_)_ __   __ _  |  _ \ __ _ ___ ___ 
		//     __) (_) | |   | |/ _` | '_ \| __| | '_ \ / _` | | |_) / _` / __/ __|
		//    / __/ _  | |___| | (_| | | | | |_| | | | | (_| | |  __/ (_| \__ \__ \
        //   |_____(_) |_____|_|\__, |_| |_|\__|_|_| |_|\__, | |_|   \__,_|___/___/
		//                      |___/                   |___/                      

		{// STARTOF: 2nd (lighting pass)

			// 2nd pass of the render, where we do something bizzare
			if (::g_pFullScreenQuad == NULL)
			{
				::g_pFullScreenQuad = new cMesh;
				//            ::g_pFullScreenQuad->meshName = "Imposter_Shapes/Quad_2_sided_aligned_on_XY_plane.ply";
				//::g_pFullScreenQuad->meshName = "Imposter_Shapes/Quad_1_sided_aligned_on_XY_plane.ply";
				::g_pFullScreenQuad->meshName = "Quad_1_sided_aligned_on_XY_plane.ply";
				::g_pFullScreenQuad->friendlyName = "Full_Screen_Quad";

				::g_pFullScreenQuad->positionXYZ = glm::vec3(0.0f, 0.0f, 500.0f);
				//::g_pFullScreenQuad->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
				::g_pFullScreenQuad->setUniformScale(100.0f);
				//::g_pFullScreenQuad->setUniformScale(10.0f);
				::g_pFullScreenQuad->bIsWireframe = false;
				::g_pFullScreenQuad->bDontLight = true;
				::g_pFullScreenQuad->bUseObjectDebugColour = true;
				::g_pFullScreenQuad->objectDebugColourRGBA = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
			}//if (::g_pFullScreenQuad == NULL)

			// Set the viewport to the size of my offscreen texture (FBO)
			//
			// Note: We aren't rendering to the actual screen YET...
			// ...we are reading from the off-screen FBO 
			// ...AND writing to another FBO (in our case, they are the same 
			//  FBO, but to a different texture withing that FBO)
			// 
			glViewport(0, 0, ::g_pFBO->width, ::g_pFBO->height);
			ratio = ::g_pFBO->width / (float)::g_pFBO->height;
			//            glfwGetFramebufferSize(pWindow, &width, &height);
			//            ratio = width / (float)height;
			//            glViewport(0, 0, width, height);

			
			matView = glm::mat4(1.0f);


			glm::vec3 eyeForFullScreenQuad = glm::vec3(0.0f, 0.0f, 450.0f);   // "eye" is 100 units away from the quad
			glm::vec3 atForFullScreenQuad = glm::vec3(0.0f, 0.0f, 500.0f);    // "at" the quad
			glm::vec3 upForFullScreenQuad = glm::vec3(0.0f, 1.0f, 0.0f);      // "at" the quad
			matView = glm::lookAt(	eyeForFullScreenQuad,
									atForFullScreenQuad,
									upForFullScreenQuad);      // up in y direction

			//matView = glm::lookAt(::cameraEye,
			//	::cameraEye + ::cameraTarget,
			//	upVector);


			glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
				1, GL_FALSE, glm::value_ptr(matView));

			matProjection = glm::ortho(
				0.0f,   // Left
				1.0f / (float)width,  // Right
				0.0f,   // Top
				1.0f / (float)height, // Bottom
				0.01f,	//30.0f, // zNear  Eye is at 450, quad is at 500, so 50 units away
				1'000'000.0f); // zFar

			glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matProjection"),
				1, GL_FALSE, glm::value_ptr(matProjection));

			//GLint screenWidthHeight_locID = glGetUniformLocation(program, "screenWidthHeight");
			//glUniform2f(screenWidthHeight_locID, (float)width, (float)height);
			glUniform2f(pShaderProc->mapUniformName_to_UniformLocation["screenWidthHeight"], (float)width, (float)height);

			//glUniform1ui(renderPassNumber_LocID, PASS_2_LIGHT_PASS);
			glUniform1ui(pShaderProc->mapUniformName_to_UniformLocation["renderPassNumber"], PASS_2_LIGHT_PASS);

			// Connect the 4 FBO G-Buffer textures to the shader
			//uniform sampler2D texVertexMaterialColour;
			//uniform sampler2D texVertexNormal;
			//uniform sampler2D texVertexWorldPos;
			//uniform sampler2D texVertexSpecular;

			GLuint FSO_texVertexMaterialColour_TextureUnit = 5;	    // I picked 5 just because
			glActiveTexture(FSO_texVertexMaterialColour_TextureUnit + GL_TEXTURE0);
			GLuint texVertexMaterialColourTextureNumber = ::g_pFBO->vertexMaterialColour_1_ID;
			glBindTexture(GL_TEXTURE_2D, texVertexMaterialColourTextureNumber);
			// uniform sampler2D texVertexMaterialColour;
			//GLint FSQ_textureSamplerSamplerID = glGetUniformLocation(program, "texVertexMaterialColour");
			//glUniform1i(FSQ_textureSamplerSamplerID, FSO_texVertexMaterialColour_TextureUnit);
			glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["texVertexMaterialColour"], FSO_texVertexMaterialColour_TextureUnit);

			GLuint FSO_texVertexNormal_TextureUnit = 6;	    // I picked 6 just because
			glActiveTexture(FSO_texVertexNormal_TextureUnit + GL_TEXTURE0);
			GLuint texVertexNormalTextureNumber = ::g_pFBO->vertexNormal_2_ID;
			glBindTexture(GL_TEXTURE_2D, texVertexNormalTextureNumber);
			// uniform sampler2D texVertexNormal;
			//GLint FSQ_VertexNormalSamplerID = glGetUniformLocation(program, "texVertexNormal");
			//glUniform1i(FSQ_VertexNormalSamplerID, FSO_texVertexNormal_TextureUnit);
			glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["texVertexNormal"], FSO_texVertexNormal_TextureUnit);

			GLuint FSO_texVertexWorldPos_TextureUnit = 7;	    // I picked 7 just because
			glActiveTexture(FSO_texVertexWorldPos_TextureUnit + GL_TEXTURE0);
			GLuint texVertexWorldPosTextureNumber = ::g_pFBO->vertexWorldPos_3_ID;
			glBindTexture(GL_TEXTURE_2D, texVertexWorldPosTextureNumber);
			// uniform sampler2D texVertexWorldPos;
			//GLint FSQ_VertexWorldPosSamplerID = glGetUniformLocation(program, "texVertexWorldPos");
			//glUniform1i(FSQ_VertexWorldPosSamplerID, FSO_texVertexWorldPos_TextureUnit);
			glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["texVertexWorldPos"], FSO_texVertexWorldPos_TextureUnit);

			GLuint FSO_texVertexSpecular_TextureUnit = 8;	    // I picked 8 just because
			glActiveTexture(FSO_texVertexSpecular_TextureUnit + GL_TEXTURE0);
			GLuint texVertexSpecularTextureNumber = ::g_pFBO->vertexWorldPos_3_ID;
			glBindTexture(GL_TEXTURE_2D, texVertexSpecularTextureNumber);
			// uniform sampler2D texVertexSpecular;
			//GLint FSQ_VertexSpecularSamplerID = glGetUniformLocation(program, "texVertexSpecular");
			//glUniform1i(FSQ_VertexSpecularSamplerID, FSO_texVertexSpecular_TextureUnit);
			glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["texVertexSpecular"], FSO_texVertexSpecular_TextureUnit);


			glm::mat4x4 matModelFullScreenQuad = glm::mat4(1.0f);   // identity matrix

			glCullFace(GL_FRONT);

			//DrawObject(::g_pFullScreenQuad,
			//	matModelFullScreenQuad,
			//	matModel_Location,
			//	matModelInverseTranspose_Location,
			//	program,
			//	::g_pVAOManager);

			DrawObject(::g_pFullScreenQuad,
				matModelFullScreenQuad,
				pShaderProc->mapUniformName_to_UniformLocation["matModel"],
				pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"],
				program,
				::g_pVAOManager);

		}// ENDOF: 2nd (lighting pass)

		//    _____     ____   ____    _____   __   __              _          ____                  
		//   |___ / _  |___ \ |  _ \  | ____| / _| / _|  ___   ___ | |_  ___  |  _ \  __ _  ___  ___ 
		//     |_ \(_)   __) || | | | |  _|  | |_ | |_  / _ \ / __|| __|/ __| | |_) |/ _` |/ __|/ __|
		//    ___) |_   / __/ | |_| | | |___ |  _||  _||  __/| (__ | |_ \__ \ |  __/| (_| |\__ \\__ \
        //   |____/(_) |_____||____/  |_____||_|  |_|   \___| \___| \__||___/ |_|    \__,_||___/|___/
		//                                                                                           

		// 2nd pass of the render, where we do something bizzare
		if (::g_pFullScreenQuad == NULL)
		{
			::g_pFullScreenQuad = new cMesh;
			//            ::g_pFullScreenQuad->meshName = "Imposter_Shapes/Quad_2_sided_aligned_on_XY_plane.ply";
			//::g_pFullScreenQuad->meshName = "Imposter_Shapes/Quad_1_sided_aligned_on_XY_plane.ply";
			::g_pFullScreenQuad->meshName = "Quad_1_sided_aligned_on_XY_plane.ply";
			::g_pFullScreenQuad->friendlyName = "Full_Screen_Quad";

			::g_pFullScreenQuad->positionXYZ = glm::vec3(0.0f, 0.0f, 500.0f);
			//::g_pFullScreenQuad->positionXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
			::g_pFullScreenQuad->setUniformScale(100.0f);
			//::g_pFullScreenQuad->setUniformScale(10.0f);
			::g_pFullScreenQuad->bIsWireframe = false;
			::g_pFullScreenQuad->bDontLight = true;
			::g_pFullScreenQuad->bUseObjectDebugColour = true;
			::g_pFullScreenQuad->objectDebugColourRGBA = glm::vec4(0.7f, 0.7f, 0.7f, 1.0f);
		}//if (::g_pFullScreenQuad == NULL)


		// Point the output of the renderer to the real framebuffer

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//  Clear the frame buffer. 
		// NOTE: I'm clearing the color bit AND the depth buffer
		// I'm using the Microsoft DirectX light blue colour from here:
		// https://usbrandcolors.com/microsoft-colors/
		glClearColor(0.0f, 164.0f / 255.0f, 239.0f / 255.0f, 1.0f);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwGetFramebufferSize(pWindow, &width, &height);
		ratio = width / (float)height;

		//matProjection = glm::perspective<float>(
		//	::g_pFlyCamera->FOV,
		//	ratio,
		//	::g_pFlyCamera->nearPlane,      // Near plane (as large as possible)
		//	::g_pFlyCamera->farPlane);      // Far plane (as small as possible)

		matProjection = glm::perspective(0.6f,	// FOV variable later
			ratio,
			0.01f,								// Near plane
			1'000'000.0f);						// Far plane

		glViewport(0, 0, width, height);

		//GLint screenWidthHeight_locID = glGetUniformLocation(program, "screenWidthHeight");
		glUniform2f(pShaderProc->mapUniformName_to_UniformLocation["screenWidthHeight"], (float)width, (float)height);

		//glUniform1ui(renderPassNumber_LocID, PASS_3_2D_EFFECTS_PASS);
		glUniform1ui(pShaderProc->mapUniformName_to_UniformLocation["renderPassNumber"], PASS_3_2D_EFFECTS_PASS);

		// Set the FBO colour texture to be the texture source for this quad

		GLuint FSQ_textureUnit = 7;	    // We picked 7 just because yolo (i.e. it doesn't matter, we just had to pick one)
		glActiveTexture(FSQ_textureUnit + GL_TEXTURE0);
		GLuint TextureNumber = ::g_pFBO->colourTexture_0_ID;
		glBindTexture(GL_TEXTURE_2D, TextureNumber);

		// uniform sampler2D texLightPassColourBuffer;
		//GLint FSQ_textureSamplerID = glGetUniformLocation(program, "texLightPassColourBuffer");
		//glUniform1i(FSQ_textureSamplerID, FSQ_textureUnit);
		glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["texLightPassColourBuffer"], FSQ_textureUnit);

		if (::g_2DEffectOp == 8)
		{
			GLuint FSQ_textureUnit1 = 8;
			glActiveTexture(FSQ_textureUnit1 + GL_TEXTURE0);
			GLuint TextureNumber1 = ::g_pTextureManager->getTextureIDFromName("Scope.bmp");
			glBindTexture(GL_TEXTURE_2D, TextureNumber1);
			//GLint scopeTexture_Loc = glGetUniformLocation(program, "texScope");
			//glUniform1i(scopeTexture_Loc, FSQ_textureUnit1);
			glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["texScope"], FSQ_textureUnit1);

		}
		else if (::g_2DEffectOp == 9)
		{
			GLuint FSQ_textureUnit1 = 9;
			glActiveTexture(FSQ_textureUnit1 + GL_TEXTURE0);
			GLuint TextureNumber1 = ::g_pTextureManager->getTextureIDFromName("Window.bmp");
			glBindTexture(GL_TEXTURE_2D, TextureNumber1);
			//GLint scopeTexture_Loc = glGetUniformLocation(program, "texWindow");
			//glUniform1i(scopeTexture_Loc, FSQ_textureUnit1);
			glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["texWindow"], FSQ_textureUnit1);

		}


		matModel = glm::mat4(1.0f);  // "Identity" ("do nothing", like x1)
		//glm::mat4x4 matModelFullScreenQuad = glm::mat4(1.0f);   // identity matrix

		glCullFace(GL_FRONT);

		// Place the camera in front of the quad (the "full screen" quad)
		// Quad location is ::g_pFullScreenQuad->positionXYZ = glm::vec3( 0.0f, 0.0f, 500.0f);

		matView = glm::mat4(1.0f);

		glm::vec3 eyeForFullScreenQuad = glm::vec3(0.0f, 0.0f, 450.0f);   // "eye" is 100 units away from the quad
		glm::vec3 atForFullScreenQuad = glm::vec3(0.0f, 0.0f, 500.0f);    // "at" the quad
		glm::vec3 upForFullScreenQuad = glm::vec3(0.0f, 1.0f, 0.0f);      // "at" the quad
		//matView = glm::lookAt(::cameraEye,
		//			::cameraEye + ::cameraTarget,
		//			upVector);
		matView = glm::lookAt(eyeForFullScreenQuad,
			atForFullScreenQuad,
			upForFullScreenQuad);      // up in y direction

//detail::tmat4x4<T> glm::gtc::matrix_transform::ortho	(	T const & 	left,
//                                                         T const & 	right,
//                                                         T const & 	bottom,
//                                                         T const & 	top,
//                                                         T const & 	zNear,
//                                                         T const & 	zFar )		
		matProjection = glm::ortho(
			0.0f,   // Left
			1.0f / (float)width,  // Right
			0.0f,   // Top
			1.0f / (float)height, // Bottom
			0.1f,		//30.0f, // zNear  Eye is at 450, quad is at 500, so 50 units away
			70.0f); // zFar

		glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
			1, GL_FALSE, glm::value_ptr(matView));
		glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matProjection"),
			1, GL_FALSE, glm::value_ptr(matProjection));

		//GLuint twoDEffectOperator_Location = glGetUniformLocation(program, "twoDEffectOperator");
		//glUniform1i(twoDEffectOperator_Location, ::g_2DEffectOp);
		glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["twoDEffectOperator"], ::g_2DEffectOp);

		//DrawObject(::g_pFullScreenQuad,
		//	matModel,
		//	matModel_Location,
		//	matModelInverseTranspose_Location,
		//	program,
		//	::g_pVAOManager);

		DrawObject(::g_pFullScreenQuad,
			matModel,
			pShaderProc->mapUniformName_to_UniformLocation["matModel"],
			pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"],
			program,
			::g_pVAOManager);
		


		//    _____     ____   ____    _____   __   __              _          ____                  
		//   |___ / _  |___ \ |  _ \  | ____| / _| / _|  ___   ___ | |_  ___  |  _ \  __ _  ___  ___ 
		//     |_ \(_)   __) || | | | |  _|  | |_ | |_  / _ \ / __|| __|/ __| | |_) |/ _` |/ __|/ __|
		//    ___) |_   / __/ | |_| | | |___ |  _||  _||  __/| (__ | |_ \__ \ |  __/| (_| |\__ \\__ \
		//   |____/(_) |_____||____/  |_____||_|  |_|   \___| \___| \__||___/ |_|    \__,_||___/|___/
		//             

		// Update on the VAO manager to do any pending loads

		// "Present" what we've drawn.
		glfwSwapBuffers(pWindow);        // Show what we've drawn

		// Process any events that have happened
		glfwPollEvents();

		// Handle OUR keyboard, mouse stuff
		handleAsyncKeyboard(pWindow, deltaTime);
		handleAsyncMouse(pWindow, deltaTime);

	} //end of while (!glfwWindowShouldClose(window))

	// All done, so delete things...
	::g_ShutDown(pWindow);


	glfwDestroyWindow(pWindow);

	glfwTerminate();
	exit(EXIT_SUCCESS);

} //end of main



// Modified from previous versions to take texture information
bool loadWorldFile()
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
	std::stringstream ss;
	std::stringstream sFile;


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

	::g_pTheLights->TurnOffLight(0);

	unsigned int index = 1;	// can't start at 0 because for some reason the 0 light over writes all other lights

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


		// Load everything into the lights
		::g_pTheLights->theLights[index].position = glm::vec4(position, 1.0f);
		::g_pTheLights->theLights[index].diffuse = glm::vec4(diffuse, 1.0f);
		::g_pTheLights->theLights[index].specular = glm::vec4(specular, 1.0f);
		::g_pTheLights->theLights[index].atten = atten;
		::g_pTheLights->theLights[index].direction = glm::vec4(direction, 1.0f);
		::g_pTheLights->theLights[index].param1 = glm::vec4(param1, 1.0f);
		::g_pTheLights->theLights[index].param2 = glm::vec4(param2, 0.0f, 0.0f, 1.0f);

		index++;
	} //end of while

	mazeLightsStartIndex = index;
	::g_currentLightIndex = index;

	theFile.close();
	return true;

} //end of load lights

//Figured out the math for how to do this from https://learnopengl.com/Getting-started/Camera and http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/
//Using the mouse position we calculate the direction that the camera will be facing
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	//if it's the start of the program this smooths out a potentially glitchy jump
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	//find the offset of where the mouse positions have moved
	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	//multiply by sensitivity so that it's not potentially crazy fast
	float sensitivity = 0.1f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	::cameraYaw += xOffset;         // The yaw is the rotation around the ::camera's y-axis (so we want to add the xOffset to it)
	::cameraPitch += yOffset;       // The pitch is the rotation around the ::camera's x-axis (so we want to add the yOffset to it)
	//This limits the pitch so that we can't just spin the ::camera under/over itself
	if (::cameraPitch > 89.0f)
		::cameraPitch = 89.0f;
	if (::cameraPitch < -89.0f)
		::cameraPitch = -89.0f;
	//calculations for the new direction based on the mouse movements
	glm::vec3 direction;
	direction.x = cos(glm::radians(::cameraYaw)) * cos(glm::radians(::cameraPitch));
	direction.y = sin(glm::radians(::cameraPitch));
	direction.z = sin(glm::radians(::cameraYaw)) * cos(glm::radians(::cameraPitch));
	::cameraTarget = glm::normalize(direction);
} //fly ::camera

// Graphics 2 Midterm Models
void loadGraphicsMidtermModels()
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
void loadGraphicsProject2Models()
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
void loadAIModels()
{
	modelLocations.push_back("Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply");
	modelLocations.push_back("Isosphere_Smooth_Normals.ply");

	modelLocations.push_back("Quad_1_sided_aligned_on_XY_plane.ply");
	modelLocations.push_back("Quad_2_sided_aligned_on_XY_plane.ply");
}

// GameJam Models
void loadGameJamModels()
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
}

// GRAPHICS 2 Midterm
//void makeSpaceStation()
//{
//	cMesh* baseMesh = new cMesh();
//	baseMesh->meshName = "Arms Small_xyz_rgba_n_uv.ply";
//	baseMesh->positionXYZ = glm::vec3(0.0f, 0.0f, 500.0f);
//	baseMesh->orientationXYZ = glm::vec3(0.0f);
//	baseMesh->setUniformScale(0.01f);
//	baseMesh->bDontLight = true;
//	baseMesh->clearTextureRatiosToZero();
//	baseMesh->textureNames[1] = "HullPlates1.bmp";
//	baseMesh->textureNames[2] = "HullPlates2.bmp";
//	baseMesh->textureNames[3] = "HullPlates3.bmp";
//	baseMesh->textureOperator = 0;
//	baseMesh->textureRatios[1] = 0.4f;
//	baseMesh->textureRatios[2] = 0.3f;
//	baseMesh->textureRatios[3] = 0.3f;
//	//sigh
//	baseMesh->bUseWholeObjectDiffuseColour = true;
//	baseMesh->bUseObjectDebugColour = true;
//	baseMesh->wholeObjectDiffuseRGBA = glm::vec4(0.88f, 0.88f, 0.88f, 1.0f);
//	baseMesh->objectDebugColourRGBA = glm::vec4(0.88f, 0.88f, 0.88f, 1.0f);
//	
//	{	//2
//		cMesh* nextMesh = new cMesh();
//		nextMesh->meshName = "Arms_xyz_rgba_n_uv.ply";
//		nextMesh->positionXYZ = glm::vec3(0.0f);
//		nextMesh->orientationXYZ = glm::vec3(0.0f);
//		nextMesh->setUniformScale(1.0f);
//		nextMesh->bDontLight = true;
//		nextMesh->clearTextureRatiosToZero();
//		nextMesh->textureNames[1] = "HullPlates1.bmp";
//		nextMesh->textureNames[2] = "HullPlates2.bmp";
//		nextMesh->textureNames[3] = "HullPlates3.bmp";
//		nextMesh->textureOperator = 0;
//		nextMesh->textureRatios[1] = 0.4f;
//		nextMesh->textureRatios[2] = 0.3f;
//		nextMesh->textureRatios[3] = 0.3f;
//		nextMesh->bUseWholeObjectDiffuseColour = true;
//		nextMesh->bUseObjectDebugColour = true;
//		nextMesh->wholeObjectDiffuseRGBA = glm::vec4(0.0f, 0.0f, 0.6f, 1.0f);
//		nextMesh->objectDebugColourRGBA = glm::vec4(0.0f, 0.0f, 0.6f, 1.0f);
//
//		baseMesh->vec_pChildMeshes.push_back(nextMesh);
//	}
//
//	{	//3
//		cMesh* nextMesh = new cMesh();
//		nextMesh->meshName = "Cooling Arms_xyz_rgba_n_uv.ply";
//		nextMesh->positionXYZ = glm::vec3(0.0f);
//		nextMesh->orientationXYZ = glm::vec3(0.0f);
//		nextMesh->setUniformScale(1.0f);
//		nextMesh->bDontLight = true;
//		nextMesh->clearTextureRatiosToZero();
//		nextMesh->textureNames[1] = "HullPlates1.bmp";
//		nextMesh->textureNames[2] = "HullPlates2.bmp";
//		nextMesh->textureNames[3] = "HullPlates3.bmp";
//		nextMesh->textureOperator = 0;
//		nextMesh->textureRatios[1] = 0.4f;
//		nextMesh->textureRatios[2] = 0.3f;
//		nextMesh->textureRatios[3] = 0.3f;
//		nextMesh->bUseWholeObjectDiffuseColour = true;
//		nextMesh->bUseObjectDebugColour = true;
//		nextMesh->wholeObjectDiffuseRGBA = glm::vec4(0.8f, 0.0f, 0.0f, 1.0f);
//		nextMesh->objectDebugColourRGBA = glm::vec4(0.8f, 0.0f, 0.0f, 1.0f);
//
//		baseMesh->vec_pChildMeshes.push_back(nextMesh);
//	}
//
//	{	//4
//		cMesh* nextMesh = new cMesh();
//		nextMesh->meshName = "Hanger_xyz_rgba_n_uv.ply";
//		nextMesh->positionXYZ = glm::vec3(0.0f);
//		nextMesh->orientationXYZ = glm::vec3(0.0f);
//		nextMesh->setUniformScale(1.0f);
//		nextMesh->bDontLight = true;
//		nextMesh->clearTextureRatiosToZero();
//		nextMesh->textureNames[1] = "HullPlates1.bmp";
//		nextMesh->textureNames[2] = "HullPlates2.bmp";
//		nextMesh->textureNames[3] = "HullPlates3.bmp";
//		nextMesh->textureOperator = 0;
//		nextMesh->textureRatios[1] = 0.4f;
//		nextMesh->textureRatios[2] = 0.3f;
//		nextMesh->textureRatios[3] = 0.3f;
//		nextMesh->bUseWholeObjectDiffuseColour = true;
//		nextMesh->bUseObjectDebugColour = true;
//		nextMesh->wholeObjectDiffuseRGBA = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
//		nextMesh->objectDebugColourRGBA = glm::vec4(0.25f, 0.25f, 0.25f, 1.0f);
//
//		baseMesh->vec_pChildMeshes.push_back(nextMesh);
//	}
//
//	{	//5
//		cMesh* nextMesh = new cMesh();
//		nextMesh->meshName = "Parts_xyz_rgba_n_uv.ply";
//		nextMesh->positionXYZ = glm::vec3(0.0f);
//		nextMesh->orientationXYZ = glm::vec3(0.0f);
//		nextMesh->setUniformScale(1.0f);
//		nextMesh->bDontLight = true;
//		nextMesh->clearTextureRatiosToZero();
//		nextMesh->textureNames[1] = "HullPlates1.bmp";
//		nextMesh->textureNames[2] = "HullPlates2.bmp";
//		nextMesh->textureNames[3] = "HullPlates3.bmp";
//		nextMesh->textureOperator = 0;
//		nextMesh->textureRatios[1] = 0.4f;
//		nextMesh->textureRatios[2] = 0.3f;
//		nextMesh->textureRatios[3] = 0.3f;
//		nextMesh->bUseWholeObjectDiffuseColour = true;
//		nextMesh->bUseObjectDebugColour = true;
//		nextMesh->wholeObjectDiffuseRGBA = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
//		nextMesh->objectDebugColourRGBA = glm::vec4(0.1f, 0.1f, 0.1f, 1.0f);
//
//		baseMesh->vec_pChildMeshes.push_back(nextMesh);
//	}
//
//	{	//6
//		cMesh* nextMesh = new cMesh();
//		nextMesh->meshName = "Reactor_xyz_rgba_n_uv.ply";
//		nextMesh->positionXYZ = glm::vec3(0.0f);
//		nextMesh->orientationXYZ = glm::vec3(0.0f);
//		nextMesh->setUniformScale(1.0f);
//		nextMesh->bDontLight = true;
//		nextMesh->clearTextureRatiosToZero();
//		nextMesh->textureNames[1] = "HullPlates1.bmp";
//		nextMesh->textureNames[2] = "HullPlates2.bmp";
//		nextMesh->textureNames[3] = "HullPlates3.bmp";
//		nextMesh->textureOperator = 0;
//		nextMesh->textureRatios[1] = 0.4f;
//		nextMesh->textureRatios[2] = 0.3f;
//		nextMesh->textureRatios[3] = 0.3f;
//		nextMesh->bUseWholeObjectDiffuseColour = true;
//		nextMesh->bUseObjectDebugColour = true;
//		nextMesh->wholeObjectDiffuseRGBA = glm::vec4(0.88f, 0.88f, 0.88f, 1.0f);
//		nextMesh->objectDebugColourRGBA = glm::vec4(0.88f, 0.88f, 0.88f, 1.0f);
//
//		baseMesh->vec_pChildMeshes.push_back(nextMesh);
//	}
//
//	{	//7
//		cMesh* nextMesh = new cMesh();
//		nextMesh->meshName = "Ring Small1_xyz_rgba_n_uv.ply";
//		nextMesh->positionXYZ = glm::vec3(0.0f);
//		nextMesh->orientationXYZ = glm::vec3(0.0f);
//		nextMesh->setUniformScale(1.0f);
//		nextMesh->bDontLight = true;
//		nextMesh->clearTextureRatiosToZero();
//		nextMesh->textureNames[1] = "HullPlates1.bmp";
//		nextMesh->textureNames[2] = "HullPlates2.bmp";
//		nextMesh->textureNames[3] = "HullPlates3.bmp";
//		nextMesh->textureOperator = 0;
//		nextMesh->textureRatios[1] = 0.4f;
//		nextMesh->textureRatios[2] = 0.3f;
//		nextMesh->textureRatios[3] = 0.3f;
//		nextMesh->bUseWholeObjectDiffuseColour = true;
//		nextMesh->bUseObjectDebugColour = true;
//		nextMesh->wholeObjectDiffuseRGBA = glm::vec4(0.38f, 0.38f, 0.38f, 1.0f);
//		nextMesh->objectDebugColourRGBA = glm::vec4(0.38f, 0.38f, 0.38f, 1.0f);
//
//		baseMesh->vec_pChildMeshes.push_back(nextMesh);
//	}
//
//	{	//8
//		cMesh* nextMesh = new cMesh();
//		nextMesh->meshName = "Ring Small2_xyz_rgba_n_uv.ply";
//		nextMesh->positionXYZ = glm::vec3(0.0f);
//		nextMesh->orientationXYZ = glm::vec3(0.0f);
//		nextMesh->setUniformScale(1.0f);
//		nextMesh->bDontLight = true;
//		nextMesh->clearTextureRatiosToZero();
//		nextMesh->textureNames[1] = "HullPlates1.bmp";
//		nextMesh->textureNames[2] = "HullPlates2.bmp";
//		nextMesh->textureNames[3] = "HullPlates3.bmp";
//		nextMesh->textureOperator = 0;
//		nextMesh->textureRatios[1] = 0.4f;
//		nextMesh->textureRatios[2] = 0.3f;
//		nextMesh->textureRatios[3] = 0.3f;
//		nextMesh->bUseWholeObjectDiffuseColour = true;
//		nextMesh->bUseObjectDebugColour = true;
//		nextMesh->wholeObjectDiffuseRGBA = glm::vec4(0.38f, 0.38f, 0.38f, 1.0f);
//		nextMesh->objectDebugColourRGBA = glm::vec4(0.38f, 0.38f, 0.38f, 1.0f);
//
//		baseMesh->vec_pChildMeshes.push_back(nextMesh);
//	}
//
//	{	//9
//		cMesh* nextMesh = new cMesh();
//		nextMesh->meshName = "RingBig_xyz_rgba_n_uv.ply";
//		nextMesh->positionXYZ = glm::vec3(0.0f);
//		nextMesh->orientationXYZ = glm::vec3(0.0f);
//		nextMesh->setUniformScale(1.0f);
//		nextMesh->bDontLight = true;
//		nextMesh->clearTextureRatiosToZero();
//		nextMesh->textureNames[1] = "HullPlates1.bmp";
//		nextMesh->textureNames[2] = "HullPlates2.bmp";
//		nextMesh->textureNames[3] = "HullPlates3.bmp";
//		nextMesh->textureOperator = 0;
//		nextMesh->textureRatios[1] = 0.4f;
//		nextMesh->textureRatios[2] = 0.3f;
//		nextMesh->textureRatios[3] = 0.3f;
//		nextMesh->bUseWholeObjectDiffuseColour = true;
//		nextMesh->bUseObjectDebugColour = true;
//		nextMesh->wholeObjectDiffuseRGBA = glm::vec4(0.38f, 0.38f, 0.38f, 1.0f);
//		nextMesh->objectDebugColourRGBA = glm::vec4(0.38f, 0.38f, 0.38f, 1.0f);
//
//		baseMesh->vec_pChildMeshes.push_back(nextMesh);
//	}
//
//	{	//10
//		cMesh* nextMesh = new cMesh();
//		nextMesh->meshName = "Solar Panels_xyz_rgba_n_uv.ply";
//		nextMesh->positionXYZ = glm::vec3(0.0f);
//		nextMesh->orientationXYZ = glm::vec3(0.0f);
//		nextMesh->setUniformScale(1.0f);
//		nextMesh->bDontLight = true;
//		nextMesh->clearTextureRatiosToZero();
//		nextMesh->textureNames[1] = "SolarGrid.bmp";
//		nextMesh->textureNames[2] = "SolarGrid2.bmp";
//		nextMesh->textureOperator = 0;
//		nextMesh->textureRatios[1] = 0.6f;
//		nextMesh->textureRatios[2] = 0.4f;
//		nextMesh->bUseWholeObjectDiffuseColour = true;
//		nextMesh->bUseObjectDebugColour = true;
//		nextMesh->wholeObjectDiffuseRGBA = glm::vec4(0.8f, 0.0f, 0.0f, 1.0f);
//		nextMesh->objectDebugColourRGBA = glm::vec4(0.8f, 0.0f, 0.0f, 1.0f);
//
//		baseMesh->vec_pChildMeshes.push_back(nextMesh);
//	}
//
//	{	//11
//		cMesh* nextMesh = new cMesh();
//		nextMesh->meshName = "Triangle Parts_xyz_rgba_n_uv.ply";
//		nextMesh->positionXYZ = glm::vec3(0.0f);
//		nextMesh->orientationXYZ = glm::vec3(0.0f);
//		nextMesh->setUniformScale(1.0f);
//		nextMesh->bDontLight = true;
//		nextMesh->clearTextureRatiosToZero();
//		nextMesh->textureNames[1] = "SolarGrid.bmp";
//		nextMesh->textureNames[2] = "SolarGrid2.bmp";
//		nextMesh->textureOperator = 0;
//		nextMesh->textureRatios[1] = 0.6f;
//		nextMesh->textureRatios[2] = 0.4f;
//		nextMesh->bUseWholeObjectDiffuseColour = true;
//		nextMesh->bUseObjectDebugColour = true;
//		nextMesh->wholeObjectDiffuseRGBA = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
//		nextMesh->objectDebugColourRGBA = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
//
//		baseMesh->vec_pChildMeshes.push_back(nextMesh);
//	}
//
//
//	::g_vec_pMeshes.push_back(baseMesh);
//	localGStation = baseMesh;
//}


char GetColourCharacter(unsigned char r, unsigned char g, unsigned char b)
{
	if (r == 255 && g == 0 && b == 0)		return 'r';
	if (r == 0 && g == 255 && b == 0)		return 'g';
	if (r == 0 && g == 0 && b == 255)		return 'b';
	if (r == 255 && g == 255 && b == 0)		return 'y';
	if (r == 255 && g == 255 && b == 255)	return 'w';
	if (r == 0 && g == 0 && b == 0)			return '_';

	// Colours on Resource Map 3 are corrupted, some pixels are not exact colours
	//if (r >= 200 && g <= 50 && b <= 50)		return 'r';
	//if (r <= 50 && g >= 200 && b <= 50)		return 'g';
	//if (r <= 50 && g <= 50 && b >= 200)		return 'b';
	//if (r >= 200 && g >= 200 && b <= 50)	return 'y';
	//if (r >= 200 && g >= 200 && b >= 200)	return 'w';
	//if (r <= 50 && g <= 50 && b <= 50)		return '_';
	return 'x';
}

void MakeGraphAndMeshes()
{
	char* data = bmp->GetData();
	unsigned long imageWidth = bmp->GetImageWidth();
	unsigned long imageHeight = bmp->GetImageHeight();
	char name = 'a';

	int index = 0;
	for (unsigned long x = 0; x < imageWidth; x++) {
		for (unsigned long y = 0; y < imageHeight; y++) {
			char c = GetColourCharacter(data[index++], data[index++], data[index++]);
			printf("%c", c);

			cMesh* tempMesh = new cMesh();
			tempMesh->meshName = "Quad_1_sided_aligned_on_XY_plane.ply";
			tempMesh->positionXYZ = glm::vec3(y, x, 0.0f);
			tempMesh->orientationXYZ = glm::vec3(0.0f);
			tempMesh->setUniformScale(1.0f);
			tempMesh->bUseWholeObjectDiffuseColour = true;
			if (c == 'r')			// resource
			{
				tempMesh->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
				gNumResources++;
			}
			else if (c == 'g')		// spawn point
			{
				tempMesh->wholeObjectDiffuseRGBA = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
				spawnPoints.push_back(glm::vec3(y, x, 0.0f));
			}
			else if (c == 'b')		// home base
			{
				tempMesh->wholeObjectDiffuseRGBA = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			}
			else if (c == 'y')		// difficult terrain
			{
				tempMesh->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
			}
			else if (c == 'w')		// regular terrain
			{
				tempMesh->wholeObjectDiffuseRGBA = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
			}
			else if (c == '_')		// not traversable
			{
				tempMesh->wholeObjectDiffuseRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			}
			else if (c == 'x')		// reading error, colour in BMP is not one of our tile colours
			{
				//std::cout << "Uh oh, something wrong with the graph" << std::endl;
				//tempMesh->wholeObjectDiffuseRGBA = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
				tempMesh->wholeObjectDiffuseRGBA = glm::vec4(0.6f, 0.8f, 0.4f, 1.0f);
			}
			else
			{
				std::cout << "SOMETHING REALLY WRONG WITH THE GRAPH" << std::endl;
				tempMesh->wholeObjectDiffuseRGBA = glm::vec4(0.8f, 0.8f, 0.0f, 1.0f);
			}
			tempMesh->bDontLight = true;
			tempMesh->clearTextureRatiosToZero();
			::g_vec_pMeshes.push_back(tempMesh);

			// if the colour of the node is blue, it's the home base, if the colour of the node is red, it's a resource (or it has a goal)
			::g_Graph->CreateNode(name, Vertex(y, x, 0.0f), c, (c == 'b'), (c == 'r'));
			name++;
		}
		printf("\n");
	}
	//system("pause");

	// BMP has been read, maze/graph is generated, now we need to make edges for the graph...

	//for (unsigned int index = 0; index != ::g_Graph->nodes.size(); index++)
	//{
	//	std::cout << "Node: " << index << " is " << ::g_Graph->nodes[index]->id
	//		<< " at x: " << ::g_Graph->nodes[index]->position.x << " y: " << ::g_Graph->nodes[index]->position.y
	//		<< std::endl;
 //	}
	//system("pause");


	// other option, takes longer but makes more sense in my head
	// go through the nodes and find all the neighbhouring nodes
	for (unsigned int indexA = 0; indexA != ::g_Graph->nodes.size() - 1; indexA++)
	{
		float posAX = ::g_Graph->nodes[indexA]->position.x;
		float posAY = ::g_Graph->nodes[indexA]->position.y;

		// if the node at indexA is not traversable, then we don't want to make any edges out of it anyways 
		if (::g_Graph->nodes[indexA]->type != '_' && ::g_Graph->nodes[indexA]->type != 'x')
		{
			for (unsigned int indexB = indexA + 1; indexB != ::g_Graph->nodes.size(); indexB++)
			{
				// and then compare the position of each node at indexB to the position of the node at indexA
				float posBX = ::g_Graph->nodes[indexB]->position.x;
				float posBY = ::g_Graph->nodes[indexB]->position.y;

				// Left
				if (posBX == posAX - 1 && posBY == posAY)
				{
					// ok, so this node is to the left of our node
					// now we need to check what type it is, if it's black we don't make an Edge
					if (::g_Graph->nodes[indexB]->type != '_' && ::g_Graph->nodes[indexB]->type != 'x')
					{
						// node is white, red, green, blue or yellow, make an edge
						::g_Graph->AddEdge(::g_Graph->nodes[indexA],
							::g_Graph->nodes[indexB],
							10.0f,
							true);
					}
				}
				if (posBX == posAX + 1 && posBY == posAY)	// right
				{
					// now we need to check what type it is, if it's black we don't make an Edge
					if (::g_Graph->nodes[indexB]->type != '_' && ::g_Graph->nodes[indexB]->type != 'x')
					{
						// node is white, red, green, blue or yellow, make an edge
						::g_Graph->AddEdge(::g_Graph->nodes[indexA],
							::g_Graph->nodes[indexB],
							10.0f,
							true);
					}
				}

				if (posBY == posAY + 1 && posBX == posAX)	// up
				{
					// now we need to check what type it is, if it's black we don't make an Edge
					if (::g_Graph->nodes[indexB]->type != '_' && ::g_Graph->nodes[indexB]->type != 'x')
					{
						// node is white, red, green, blue or yellow, make an edge
						::g_Graph->AddEdge(::g_Graph->nodes[indexA],
							::g_Graph->nodes[indexB],
							10.0f,
							true);
					}
				}
				if (posBY == posAY - 1 && posBX == posAX)	// down
				{
					// now we need to check what type it is, if it's black we don't make an Edge
					if (::g_Graph->nodes[indexB]->type != '_' && ::g_Graph->nodes[indexB]->type != 'x')
					{
						// node is white, red, green, blue or yellow, make an edge
						::g_Graph->AddEdge(::g_Graph->nodes[indexA],
							::g_Graph->nodes[indexB],
							10.0f,
							true);
					}
				}

				// Now the fun part... Diagonals
				// which are dependent on the nodes left, right, above and below the current node... uggh

				// Left and Above
				if (posBX == posAX - 1
					&& posBY == posAY + 1)
				{
					// first find the nodes to the left and above the current node
					// holy cow this is inefficient
					Node* tempNodeA = nullptr;
					Node* tempNodeB = nullptr;
					for (unsigned int indexC = 0; indexC != ::g_Graph->nodes.size(); indexC++)
					{
						float posCX = ::g_Graph->nodes[indexC]->position.x;
						float posCY = ::g_Graph->nodes[indexC]->position.y;
						if (posCX == posAX - 1 && posCY == posAY)	// node to the left of current node
						{
							tempNodeA = ::g_Graph->nodes[indexC];
						}
						if (posCX == posAX && posCY == posAY + 1)	// node above the current node
						{
							tempNodeB = ::g_Graph->nodes[indexC];
						}
					}
					if (tempNodeA != nullptr && tempNodeB != nullptr)
					{
						if (tempNodeA->type != '_' && tempNodeA->type != 'x'
							&& tempNodeB->type != '_' && tempNodeB->type != 'x')
						{
							if (::g_Graph->nodes[indexB]->type != '_' && ::g_Graph->nodes[indexB]->type != 'x')
							{
								::g_Graph->AddEdge(::g_Graph->nodes[indexA],
									::g_Graph->nodes[indexB],
									14.0f,
									true);
							}
						}
					}
				}

				// Left and Below
				if (posBX == posAX - 1
					&& posBY == posAY - 1)
				{
					Node* tempNodeA = nullptr;
					Node* tempNodeB = nullptr;
					for (unsigned int indexC = 0; indexC != ::g_Graph->nodes.size(); indexC++)
					{
						float posCX = ::g_Graph->nodes[indexC]->position.x;
						float posCY = ::g_Graph->nodes[indexC]->position.y;
						if (posCX == posAX - 1 && posCY == posAY)	// node to the left of current node
						{
							tempNodeA = ::g_Graph->nodes[indexC];
						}
						if (posCX == posAX && posCY == posAY - 1)	// node below the current node
						{
							tempNodeB = ::g_Graph->nodes[indexC];
						}
					}
					if (tempNodeA != nullptr && tempNodeB != nullptr)
					{
						if (tempNodeA->type != '_' && tempNodeA->type != 'x'
							&& tempNodeB->type != '_' && tempNodeB->type != 'x')
						{
							if (::g_Graph->nodes[indexB]->type != '_' && ::g_Graph->nodes[indexB]->type != 'x')
							{
								::g_Graph->AddEdge(::g_Graph->nodes[indexA],
									::g_Graph->nodes[indexB],
									14.0f,
									true);
							}
						}
					}
				}

				// Right and Above
				if (posBX == posAX + 1
					&& posBY == posAY + 1)
				{
					// first find the nodes to the left and above the current node
					// holy cow this is inefficient
					Node* tempNodeA = nullptr;
					Node* tempNodeB = nullptr;
					for (unsigned int indexC = 0; indexC != ::g_Graph->nodes.size(); indexC++)
					{
						float posCX = ::g_Graph->nodes[indexC]->position.x;
						float posCY = ::g_Graph->nodes[indexC]->position.y;
						if (posCX == posAX + 1 && posCY == posAY)	// node to the right of current node
						{
							tempNodeA = ::g_Graph->nodes[indexC];
						}
						if (posCX == posAX && posCY == posAY + 1)	// node above the current node
						{
							tempNodeB = ::g_Graph->nodes[indexC];
						}
					}
					if (tempNodeA != nullptr && tempNodeB != nullptr)
					{
						if (tempNodeA->type != '_' && tempNodeA->type != 'x'
							&& tempNodeB->type != '_' && tempNodeB->type != 'x')
						{
							if (::g_Graph->nodes[indexB]->type != '_' && ::g_Graph->nodes[indexB]->type != 'x')
							{
								// node is  yellow, double edge weight
								::g_Graph->AddEdge(::g_Graph->nodes[indexA],
									::g_Graph->nodes[indexB],
									14.0f,
									true);
							}
						}
					}
				}

				// Right and Below
				if (posBX == posAX - 1
					&& posBY == posAY - 1)
				{
					Node* tempNodeA = nullptr;
					Node* tempNodeB = nullptr;
					for (unsigned int indexC = 0; indexC != ::g_Graph->nodes.size(); indexC++)
					{
						float posCX = ::g_Graph->nodes[indexC]->position.x;
						float posCY = ::g_Graph->nodes[indexC]->position.y;
						if (posCX == posAX + 1 && posCY == posAY)	// node to the right of current node
						{
							tempNodeA = ::g_Graph->nodes[indexC];
						}
						if (posCX == posAX && posCY == posAY - 1)	// node below the current node
						{
							tempNodeB = ::g_Graph->nodes[indexC];
						}
					}
					if (tempNodeA != nullptr && tempNodeB != nullptr)
					{
						if (tempNodeA->type != '_' && tempNodeA->type != 'x'
							&& tempNodeB->type != '_' && tempNodeB->type != 'x')
						{
							if (::g_Graph->nodes[indexB]->type != '_' && ::g_Graph->nodes[indexB]->type != 'x')
							{
								// node is  yellow, double edge weight
								::g_Graph->AddEdge(::g_Graph->nodes[indexA],
									::g_Graph->nodes[indexB],
									14.0f,
									true);
							}
						}
					}
				}

			}
		}
		
	}

	//::g_Graph->PrintGraph();

	//system("pause");

	MakeFSMEntities();

	return;
}

void MakeFSMEntities()
{
	for (unsigned int index = 0; index != spawnPoints.size(); index++)
	{
		cFSMEntity* tempEntity = new cFSMEntity(::g_Graph, spawnPoints[index]);
		vec_pFSMEntities.push_back(tempEntity);
	}
}

bool loadTSVGrid()
{
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
					std::cout << grid[x][y] << " ";
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
						std::cout << grid[x][y] << " ";
						x++;
						grid[x][y] = nextLetter;
					}
				}
				else
				{
					std::cout << grid[x][y] << " ";
					x++;
					grid[x][y] = nextLetter;
				}
			}
			std::cout << grid[x][y] << " ";
		}	//end of for x
		// Newline
		std::cout << std::endl;
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

	cDungeonMeshBuilder dungeonBuilder;

	// TODO: Either in these for loops or do it again, make the graph and nodes
	// 
	// Choose the mesh and location depending on the x/y values of the grid
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
			}
			else if (grid[x][y] == "D")
			{
				// We need to check what the orientation of the door is
				//newMesh = new cMesh();
				// Made a builder that does the nitty gritty for me, still have to set position and stuff of the floor itself afterwards
				
				newMesh = dungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::FLOOR, glm::vec3(1.0f));

				// Position
				newMesh->positionXYZ.x = 0.0f + (2.5f * x);
				newMesh->positionXYZ.y = 0.0f;
				newMesh->positionXYZ.z = 0.0f + (2.5f * y);


				cMesh* door = dungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::DOOR, glm::vec3(scale));
				newMesh->vec_pChildMeshes.push_back(door);

				// Check where the walls of the floor are
				if (grid[x - 1][y] == "-")	// East or right
				{
					cMesh* eastWall = dungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(scale));
					eastWall->positionXYZ = glm::vec3(-1.2f, 0.0f, 0.0f);
					eastWall->orientationXYZ = glm::vec3(0.0f, glm::radians(90.0f), 0.0f);
					newMesh->vec_pChildMeshes.push_back(eastWall);
				}
				if (grid[x + 1][y] == "-")	// West or left
				{
					cMesh* westWall = dungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(scale));
					westWall->positionXYZ = glm::vec3(1.2f, 0.0f, 0.0f);
					westWall->orientationXYZ = glm::vec3(0.0f, glm::radians(270.0f), 0.0f);
					newMesh->vec_pChildMeshes.push_back(westWall);
				}
				if (grid[x][y - 1] == "-")	// South or backwards
				{
					cMesh* southWall = dungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(scale));
					southWall->positionXYZ = glm::vec3(0.0f, 0.0f, -1.2f);
					southWall->orientationXYZ = glm::vec3(0.0f);				// I think these walls are double sided with normals so I shouldn't need to worry about what direction it's facing
					newMesh->vec_pChildMeshes.push_back(southWall);
				}
				if (grid[x][y + 1] == "-")	// North or forwards
				{
					cMesh* northWall = dungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(scale));
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
			}	// end of Door logic
			else if (grid[x][y] == "DS" || grid[x][y] == "DP")		// TODO: DP are portcullis, for now they'll be treated the same as the secret doors
			{
				// Secret doors are just going to use a different model for the door itself, otherwise I think it's going to be the same logic as the regular door
				newMesh = dungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::FLOOR, glm::vec3(scale));

				// Position
				newMesh->positionXYZ.x = 0.0f + (2.5f * x);
				newMesh->positionXYZ.y = 0.0f;
				newMesh->positionXYZ.z = 0.0f + (2.5f * y);

				cMesh* gateMesh;
				if (grid[x][y] == "DS")	// in case I ever actually do make them different
				{
					gateMesh = dungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::SECRETDOOR, glm::vec3(1.0f));
				}
				else
				{
					gateMesh = dungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::PORTCULLIS, glm::vec3(1.0f));
				}
				
				newMesh->vec_pChildMeshes.push_back(gateMesh);

				// Check where the walls of the floor are
				if (grid[x - 1][y] == "-")	// East or right
				{
					cMesh* eastWall = dungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(1.0f));
					eastWall->positionXYZ = glm::vec3(-1.2f, 0.0f, 0.0f);
					eastWall->orientationXYZ = glm::vec3(0.0f, glm::radians(90.0f), 0.0f);
					newMesh->vec_pChildMeshes.push_back(eastWall);
				}
				if (grid[x + 1][y] == "-")	// West or left
				{
					cMesh* westWall = dungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(1.0f));
					westWall->positionXYZ = glm::vec3(1.2f, 0.0f, 0.0f);
					westWall->orientationXYZ = glm::vec3(0.0f, glm::radians(270.0f), 0.0f);
					newMesh->vec_pChildMeshes.push_back(westWall);
				}
				if (grid[x][y - 1] == "-")	// South or backwards
				{
					cMesh* southWall = dungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(1.0f));
					southWall->positionXYZ = glm::vec3(0.0f, 0.0f, -1.2f);
					southWall->orientationXYZ = glm::vec3(0.0f);				// I think these walls are double sided with normals so I shouldn't need to worry about what direction it's facing
					newMesh->vec_pChildMeshes.push_back(southWall);
				}
				if (grid[x][y + 1] == "-")	// North or forwards
				{
					cMesh* northWall = dungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(1.0f));
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
			}	// end of DS || DP
			else if (grid[x][y] == "SD"
			|| grid[x][y] == "SDD"
			|| grid[x][y] == "SU"
			|| grid[x][y] == "SUU")	// Stairs, logic only needs to be separated for the y position, so let's just wrap the y value
			{
				
				newMesh = dungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::STAIRS, glm::vec3(scale));
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
				newMesh = dungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::FLOOR, glm::vec3(scale));
				
				if (grid[x - 1][y] == "-")	// East or right
				{
					cMesh* eastWall = dungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(1.0f));
					eastWall->positionXYZ = glm::vec3(-1.2f, 0.0f, 0.0f);
					eastWall->orientationXYZ = glm::vec3(0.0f, glm::radians(90.0f), 0.0f);
					newMesh->vec_pChildMeshes.push_back(eastWall);
				}
				if (grid[x + 1][y] == "-")	// West or left
				{
					cMesh* westWall = dungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(1.0f));
					westWall->positionXYZ = glm::vec3(1.2f, 0.0f, 0.0f);
					westWall->orientationXYZ = glm::vec3(0.0f, glm::radians(270.0f), 0.0f);
					newMesh->vec_pChildMeshes.push_back(westWall);
				}
				if (grid[x][y - 1] == "-")	// South or backwards
				{
					cMesh* southWall = dungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(1.0f));
					southWall->positionXYZ = glm::vec3(0.0f, 0.0f, -1.2f);
					southWall->orientationXYZ = glm::vec3(0.0f);				// I think these walls are double sided with normals so I shouldn't need to worry about what direction it's facing
					newMesh->vec_pChildMeshes.push_back(southWall);
				}
				if (grid[x][y + 1] == "-")	// North or forwards
				{
					cMesh* northWall = dungeonBuilder.MakeMesh(cDungeonMeshBuilder::TypeOfMesh::WALL, glm::vec3(1.0f));
					northWall->positionXYZ = glm::vec3(0.0f, 0.0f, 1.2f);
					northWall->orientationXYZ = glm::vec3(0.0f, glm::radians(180.0f), 0.0f);				// I think these walls are double sided with normals so I shouldn't need to worry about what direction it's facing
					newMesh->vec_pChildMeshes.push_back(northWall);
				}

				newMesh->positionXYZ.x = 0.0f + (2.5f * x);
				newMesh->positionXYZ.y = 0.0f;
				newMesh->positionXYZ.z = 0.0f + (2.5f * y);
				newMesh->orientationXYZ = glm::vec3(0.0f);

				::g_vec_pMeshes.push_back(newMesh);
			}	// end of floor
		}	// end of for x
	}	// end of for y

	return true;
}	//end of loadTSVGrid