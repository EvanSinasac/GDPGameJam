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

#include "cPlayerEntity.h"


float lastX = 600.0f;
float lastY = 320.0f;
bool firstMouse = true;
float cameraYaw = 0.0f;
float cameraPitch = 0.0f;
// am I even using these anymore?
float doorLightDirCurX = 1.0f;
float doorLightDirCurZ = 0.0f;
float doorLightRotateX = 1.0f;
float doorLightRotateZ = 1.0f;

double deltaTime = 0.0;

unsigned int numberOfTransparentObjects = 0;

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

void DrawScene1(GLuint program);

bool loadWorldFile(unsigned int& numberOfTransparentObjects);
bool loadLightsFile();

void loadGraphicsMidtermModels(std::vector<std::string>& modelLocations);
void loadGraphicsProject2Models(std::vector<std::string>& modelLocations);
void loadAIModels(std::vector<std::string>& modelLocations);
void loadGameJamModels(std::vector<std::string>& modelLocations);
bool loadTextures();
bool loadDefaultSkyBox();

//void MakeGraphAndMeshes();
void MakeFSMEntities(std::vector<glm::vec3> spawnPoints);

bool loadTSVGrid();			// modified version from Graphics 1 Final

// AI Project 3
//char GetColourCharacter(unsigned char r, unsigned char g, unsigned char b);

int gNumResources;
//BMPImage* bmp;

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
	
	pShaderProc->mapUniformName_to_UniformLocation["tex2VertexMaterialColour"] = glGetUniformLocation(program, "tex2VertexMaterialColour");
	pShaderProc->mapUniformName_to_UniformLocation["tex2VertexNormal"] = glGetUniformLocation(program, "tex2VertexNormal");
	pShaderProc->mapUniformName_to_UniformLocation["tex2VertexWorldPos"] = glGetUniformLocation(program, "tex2VertexWorldPos");
	pShaderProc->mapUniformName_to_UniformLocation["tex2VertexSpecular"] = glGetUniformLocation(program, "tex2VertexSpecular");
	pShaderProc->mapUniformName_to_UniformLocation["tex2LightPassColourBuffer"] = glGetUniformLocation(program, "tex2LightPassColourBuffer");
	pShaderProc->mapUniformName_to_UniformLocation["horizontal"] = glGetUniformLocation(program, "horizontal");
	pShaderProc->mapUniformName_to_UniformLocation["bloom"] = glGetUniformLocation(program, "bloom");
	pShaderProc->mapUniformName_to_UniformLocation["useBloom"] = glGetUniformLocation(program, "useBloom");
	pShaderProc->mapUniformName_to_UniformLocation["exposure"] = glGetUniformLocation(program, "exposure");

	pShaderProc->mapUniformName_to_UniformLocation["calcBloomBrightness"] = glGetUniformLocation(program, "calcBloomBrightness");
	pShaderProc->mapUniformName_to_UniformLocation["texBloomBrightness"] = glGetUniformLocation(program, "texBloomBrightness");

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
	
	// ...
	pShaderProc->mapUniformName_to_UniformLocation["eyeLocation"] = glGetUniformLocation(program, "eyeLocation");
	pShaderProc->mapUniformName_to_UniformLocation["bUseAllLights"] = glGetUniformLocation(program, "bUseAllLights");

	pShaderProc->mapUniformName_to_UniformLocation["stencilColour"] = glGetUniformLocation(program, "stencilColour");

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
	//loadGraphicsMidtermModels(modelLocations);
	//loadGraphicsProject2Models(modelLocations);
	//loadAIModels(modelLocations);
	loadGameJamModels(modelLocations);

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
	if (loadWorldFile(numberOfTransparentObjects))
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


	if (loadTextures())
	{
		std::cout << "loadTextures finished OK!" << std::endl;
	}
	else
	{
		std::cout << "One or more textures did not load, please investigate further..." << std::endl;
	}

	if (loadDefaultSkyBox())
	{
		std::cout << "loadDefaultSkyBox finished OK!" << std::endl;
	}
	else
	{
		std::cout << "One or more cube map textures did not load, please investigate further..." << std::endl;
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
	//if (::cameraEye.x > 0 && ::cameraEye.z > 0)
	//{
	//	::cameraYaw = 180.f + (atan(::cameraEye.z / ::cameraEye.x) * 180.f / glm::pi<float>());
	//}
	//else if (::cameraEye.x > 0 && ::cameraEye.z < 0)
	//{
	//	::cameraYaw = 90.f - (atan(::cameraEye.z / ::cameraEye.x) * 180.f / glm::pi<float>());
	//}
	//else if (::cameraEye.x < 0 && ::cameraEye.z > 0)
	//{
	//	::cameraYaw = (atan(::cameraEye.z / ::cameraEye.x) * 180.f / glm::pi<float>());
	//}
	//else if (::cameraEye.x < 0 && ::cameraEye.z < 0)
	//{
	//	::cameraYaw = (atan(::cameraEye.z / ::cameraEye.x) * 180.f / glm::pi<float>());
	//}
	//else if (::cameraEye.x == 0.f)
	//{
	//	if (::cameraEye.z >= 0.f)
	//	{
	//		::cameraYaw = 270.f;
	//	}
	//	else
	//	{
	//		::cameraYaw = 90.f;
	//	}
	//}
	//else if (::cameraEye.z == 0.f)
	//{
	//	if (::cameraEye.x <= 0)
	//	{
	//		::cameraYaw = 0.f;
	//	}
	//	else
	//	{
	//		::cameraYaw = 180.f;
	//	}
	//}
	// 
	// Why was I looking at cameraEye?  Cause I was focused on the camera looking at the origin
	if (::cameraTarget == glm::vec3(0.0f))
	{
		// whoops, safety
		::cameraTarget = glm::vec3(1.0f, 0.0f, 0.0f);
	}
	::cameraTarget = glm::normalize(::cameraTarget);
	if (::cameraTarget.x == 1.0f)
	{
		::cameraYaw = 0.0f;
	}
	else if (::cameraTarget.x == -1.0f)
	{
		::cameraYaw = 180.0f;
	}
	else if (::cameraTarget.z == 1.0f)
	{
		::cameraYaw = 90.0f;
	}
	else if (::cameraTarget.z == -1.0f)
	{
		::cameraYaw = 270.0f;
	}
	else
	{
		::cameraYaw = 0.0f;
	}
	//anyways, after figuring out the yaw, we set the target at the negative of the xz of the ::camera position and y=0 (this faces the ::camera towards the origin)
	//::cameraTarget = glm::vec3(-1.f * ::cameraEye.x, 0, -1.f * ::cameraEye.z);
	//glm::normalize(::cameraTarget);

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

	cFBO* bloomFBO = new cFBO();
	FBOerrorString = "";
	if (bloomFBO->init(1200, 640, FBOerrorString))
	{
		std::cout << "Bloom FBO good" << std::endl;
	}
	else
	{
		std::cout << "Bloom FBO error: " << FBOerrorString << std::endl;
	}


	cFBO* pingFBO = new cFBO();
	cFBO* pongFBO = new cFBO();

	FBOerrorString = "";
	if (pingFBO->init(1200, 640, FBOerrorString))
	{
		std::cout << "pingFBO good" << std::endl;
	}
	else
	{
		std::cout << "pingFBO error: " << FBOerrorString << std::endl;
	}
	FBOerrorString = "";
	if (pongFBO->init(1200, 640, FBOerrorString))
	{
		std::cout << "pongFBO good" << std::endl;
	}
	else
	{
		std::cout << "pongFBO error: " << FBOerrorString << std::endl;
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

		::g_pPlayer->Update(deltaTime);

		//testEntity->Update(deltaTime);
		for (unsigned int index = 0; index != ::vec_pFSMEntities.size(); index++)
		{
			::vec_pFSMEntities[index]->Update(deltaTime);
		}

		if (::g_UseBloom)
		{
			glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["useBloom"], (float)GL_TRUE);
		}
		else
		{
			glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["useBloom"], (float)GL_FALSE);
		}
		// this was where I made the other FBO objects, deleted for AI
		// alright, time to do this the way I know how
		{
			// bright bloom colour FBO
			glUniform1ui(pShaderProc->mapUniformName_to_UniformLocation["renderPassNumber"], PASS_1_G_BUFFER_PASS);
			glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO->ID);

			glViewport(0, 0, bloomFBO->width, bloomFBO->height);
			ratio = bloomFBO->width / (float)bloomFBO->height;

			glEnable(GL_DEPTH);         // Turns on the depth buffer
			glEnable(GL_DEPTH_TEST);    // Check if the pixel is already closer

			bloomFBO->clearBuffers(true, true);

			matProjection = glm::perspective(0.6f,	// FOV variable later
				ratio,
				0.1f,								// Near plane
				1'000'000.0f);						// Far plane

			matView = glm::mat4(1.0f);

			if (!::g_ObservationMode)
			{
				glm::vec3 normLookAt = glm::normalize(((cPlayerEntity*)::g_pPlayer)->lookAt);
				::cameraEye = glm::vec3(((cPlayerEntity*)::g_pPlayer)->position.x + normLookAt.x,
					((cPlayerEntity*)::g_pPlayer)->position.y + 1.5f,
					((cPlayerEntity*)::g_pPlayer)->position.z + normLookAt.z);
				::cameraTarget = ((cPlayerEntity*)::g_pPlayer)->lookAt;
			}
			//else
			//{
			matView = glm::lookAt(::cameraEye,
				::cameraEye + ::cameraTarget,
				upVector);

			glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
				1, GL_FALSE, glm::value_ptr(matView));

			glUniformMatrix4fv(pShaderProc->mapUniformName_to_UniformLocation["matProjection"], 1, GL_FALSE, glm::value_ptr(matProjection));
		
			// don't really want to blur the skybox so let's just not draw it for the FBO
			//glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bIsSkyBox"], (GLfloat)GL_TRUE);

			//glDisable(GL_DEPTH_TEST);     // Disable depth test (always write to colour buffer)
			//glDepthMask(GL_FALSE);          // Disable write to depth buffer
			//// Move the "skybox object" with the camera
			//sphereSky->positionXYZ = ::cameraEye;
			////DrawObject(sphereSky, glm::mat4(1.0f),
			////	matModel_Location, matModelInverseTranspose_Location,
			////	program, ::g_pVAOManager);

			//DrawObject(sphereSky, glm::mat4(1.0f),
			//	pShaderProc->mapUniformName_to_UniformLocation["matModel"],
			//	pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"],
			//	program,
			//	::g_pVAOManager);

			////glUniform1f(bIsSkyBox_LocID, (GLfloat)GL_FALSE);
			//glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bIsSkyBox"], (GLfloat)GL_FALSE);
			////
			//glEnable(GL_DEPTH_TEST);
			//glDepthMask(GL_TRUE);
			//// *********************************************************************
			
			// Draw the scene using the calcBloomBrightness to create a texture that highlights the brightest parts of the scene
			glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["calcBloomBrightness"], (float)GL_TRUE);
			DrawScene1(program);
			glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["calcBloomBrightness"], (float)GL_FALSE);
		
			matView = glm::mat4(1.0f);

			if (!::g_ObservationMode)
			{
				glm::vec3 normLookAt = glm::normalize(((cPlayerEntity*)::g_pPlayer)->lookAt);
				::cameraEye = glm::vec3(((cPlayerEntity*)::g_pPlayer)->position.x + normLookAt.x,
					((cPlayerEntity*)::g_pPlayer)->position.y + 1.5f,
					((cPlayerEntity*)::g_pPlayer)->position.z + normLookAt.z);
				::cameraTarget = ((cPlayerEntity*)::g_pPlayer)->lookAt;
			}
			//else
			//{
			matView = glm::lookAt(::cameraEye,
				::cameraEye + ::cameraTarget,
				upVector);

			matProjection = glm::ortho(
				0.0f,   // Left
				1.0f / (float)width,  // Right
				0.0f,   // Top
				1.0f / (float)height, // Bottom
				0.1f, // zNear  Eye is at 450, quad is at 500, so 50 units away
				70.0f); // zFar

			glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matProjection"),
				1, GL_FALSE, glm::value_ptr(matProjection));

			GLint screenWidthHeight_locID = glGetUniformLocation(program, "screenWidthHeight");
			glUniform2f(screenWidthHeight_locID, (float)bloomFBO->width, (float)bloomFBO->height);

			glUniform1ui(pShaderProc->mapUniformName_to_UniformLocation["renderPassNumber"], PASS_2_LIGHT_PASS);

			GLuint FSO_bloomVertexMaterialColour_TextureUnit = 17;
			glActiveTexture(FSO_bloomVertexMaterialColour_TextureUnit + GL_TEXTURE0);
			GLuint bloomVertexMaterialTextureNumber = bloomFBO->vertexMaterialColour_1_ID;
			glBindTexture(GL_TEXTURE_2D, bloomVertexMaterialTextureNumber);
			GLint FSQ_bloomVertexMaterialColour2SamplerID = glGetUniformLocation(program, "texBloomBrightness");
			glUniform1i(FSQ_bloomVertexMaterialColour2SamplerID, FSO_bloomVertexMaterialColour_TextureUnit);

			glCullFace(GL_FRONT);

			// texture should now be bound, if there's any colours
			// cewl it works, now I need to do the blur effect
		}

		{
		// use the ping pong buffers to perform a Gaussian blur
		// except that the way I'm doing it I don't really need to use both, so I'mma try to get away with just using one
			glUniform1ui(pShaderProc->mapUniformName_to_UniformLocation["renderPassNumber"], PASS_1_G_BUFFER_PASS);
			glBindFramebuffer(GL_FRAMEBUFFER, pingFBO->ID);

			glViewport(0, 0, pingFBO->width, pingFBO->height);
			ratio = pingFBO->width / (float)pingFBO->height;

			glEnable(GL_DEPTH);         // Turns on the depth buffer
			glEnable(GL_DEPTH_TEST);    // Check if the pixel is already closer
			
			pingFBO->clearBuffers(true, true);

			matProjection = glm::perspective(0.6f,	// FOV variable later
				ratio,
				0.1f,								// Near plane
				1'000'000.0f);						// Far plane

			matView = glm::mat4(1.0f);
			if (!::g_ObservationMode)
			{
				glm::vec3 normLookAt = glm::normalize(((cPlayerEntity*)::g_pPlayer)->lookAt);
				::cameraEye = glm::vec3(((cPlayerEntity*)::g_pPlayer)->position.x + normLookAt.x,
					((cPlayerEntity*)::g_pPlayer)->position.y + 1.5f,
					((cPlayerEntity*)::g_pPlayer)->position.z + normLookAt.z);
				::cameraTarget = ((cPlayerEntity*)::g_pPlayer)->lookAt;
			}
			//else
			//{
			matView = glm::lookAt(::cameraEye,
				::cameraEye + ::cameraTarget,
				upVector);

			glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matView"),
				1, GL_FALSE, glm::value_ptr(matView));

			glUniformMatrix4fv(pShaderProc->mapUniformName_to_UniformLocation["matProjection"], 1, GL_FALSE, glm::value_ptr(matProjection));

			// Draw the scene 10 times using bloom and store the resulting texture in the fbo
			glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bloom"], (float)GL_TRUE);
			//bool horizontal = true;
			//unsigned int amount = 10;
			//for (unsigned int index = 0; index < amount; index++)
			//{
				//glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["horizontal"], horizontal ? (float)GL_TRUE : (float)GL_FALSE);
			glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["exposure"], 1.0f);
				DrawScene1(program);

				glUniform1ui(pShaderProc->mapUniformName_to_UniformLocation["renderPassNumber"], PASS_2_LIGHT_PASS);
				GLuint FSO_pingVertexMaterialColour_TextureUnit = 18;
				glActiveTexture(FSO_pingVertexMaterialColour_TextureUnit + GL_TEXTURE0);
				GLuint pingVertexMaterialTextureNumber = pingFBO->vertexMaterialColour_1_ID;
				glBindTexture(GL_TEXTURE_2D, pingVertexMaterialTextureNumber);
				GLint FSQ_pingVertexMaterialColour2SamplerID = glGetUniformLocation(program, "tex2LightPassColourBuffer");
				glUniform1i(FSQ_pingVertexMaterialColour2SamplerID, FSO_pingVertexMaterialColour_TextureUnit);

				//horizontal = !horizontal;
			//}
			
			glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bloom"], (float)GL_FALSE);

			matView = glm::mat4(1.0f);

			if (!::g_ObservationMode)
			{
				glm::vec3 normLookAt = glm::normalize(((cPlayerEntity*)::g_pPlayer)->lookAt);
				::cameraEye = glm::vec3(((cPlayerEntity*)::g_pPlayer)->position.x + normLookAt.x,
					((cPlayerEntity*)::g_pPlayer)->position.y + 1.5f,
					((cPlayerEntity*)::g_pPlayer)->position.z + normLookAt.z);
				::cameraTarget = ((cPlayerEntity*)::g_pPlayer)->lookAt;
			}
			//else
			//{
			matView = glm::lookAt(::cameraEye,
				::cameraEye + ::cameraTarget,
				upVector);

			matProjection = glm::ortho(
				0.0f,   // Left
				1.0f / (float)width,  // Right
				0.0f,   // Top
				1.0f / (float)height, // Bottom
				0.1f, // zNear  Eye is at 450, quad is at 500, so 50 units away
				70.0f); // zFar

			glUniformMatrix4fv(pShaderProc->getUniformID_From_Name("matProjection"),
				1, GL_FALSE, glm::value_ptr(matProjection));

			GLint screenWidthHeight_locID = glGetUniformLocation(program, "screenWidthHeight");
			glUniform2f(screenWidthHeight_locID, (float)bloomFBO->width, (float)bloomFBO->height);

			glUniform1ui(pShaderProc->mapUniformName_to_UniformLocation["renderPassNumber"], PASS_2_LIGHT_PASS);

			GLuint FSO_bloomVertexMaterialColour_TextureUnit = 17;
			glActiveTexture(FSO_bloomVertexMaterialColour_TextureUnit + GL_TEXTURE0);
			GLuint bloomVertexMaterialTextureNumber = bloomFBO->vertexMaterialColour_1_ID;
			glBindTexture(GL_TEXTURE_2D, bloomVertexMaterialTextureNumber);
			GLint FSQ_bloomVertexMaterialColour2SamplerID = glGetUniformLocation(program, "texBloomBrightness");
			glUniform1i(FSQ_bloomVertexMaterialColour2SamplerID, FSO_bloomVertexMaterialColour_TextureUnit);

			glCullFace(GL_FRONT);
			
		}
		
		
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
		glEnable(GL_STENCIL_TEST);

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		//glViewport(0, 0, width, height);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

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

		// *********************************************************
		glStencilMask(0x00);	// DON'T Update the Stencil Mask while drawing floor and objects

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

		if (!::g_ObservationMode)
		{
			glm::vec3 normLookAt = glm::normalize(((cPlayerEntity*)::g_pPlayer)->lookAt);
			::cameraEye = glm::vec3(((cPlayerEntity*)::g_pPlayer)->position.x + normLookAt.x,
				((cPlayerEntity*)::g_pPlayer)->position.y + 1.5f,
				((cPlayerEntity*)::g_pPlayer)->position.z + normLookAt.z);
			::cameraTarget = ((cPlayerEntity*)::g_pPlayer)->lookAt;
		}
		//else
		//{
			matView = glm::lookAt(::cameraEye,
				::cameraEye + ::cameraTarget,
				upVector);
		//}

		// ...
		glUniform4f(pShaderProc->getUniformID_From_Name("eyeLocation"),
			::cameraEye.x, ::cameraEye.y, ::cameraEye.z, 1.0f);
		

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
			//glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["debugNormalLength"], 0.1f);
			glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["debugNormalLength"], 0.05f);
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


		// I really hope drawing the player and objects I want to use the stencil buffer on last doesn't make them all flickery
		if (::g_bStencilsOn)
		{
			glStencilFunc(GL_ALWAYS, 1, 0xFF);	// enable the stencil buffer and update it for the player and non-torch entities
			glStencilMask(0xFF);
		}
		// Draw the player and non-torch entities with stencil on if it's on

		matModel = glm::mat4(1.0f);
		DrawObject(((cPlayerEntity*)::g_pPlayer)->m_Mesh,
			matModel,
			pShaderProc->mapUniformName_to_UniformLocation["matModel"],
			pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"],
			program,
			::g_pVAOManager);



		// Before moving on to the second pass, is the stencil on?
		if (::g_bStencilsOn)
		{
			glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
			glStencilMask(0x00);
			glDisable(GL_DEPTH_TEST);
			// I want to draw the stencils, go through the entities and any of type player, object or enemy need to be drawn with the stencil buffer
			// by which I mean instead of making a new shader, I just use a texture operator and pass in the colour
			for (unsigned int index = 0; index != ::vec_pAllEntities.size(); index++)
			{
				//if (::vec_pAllEntities[index]->type != iEntity::ENTITY_TYPE::TORCH)	// draw a stencil around anything that isn't a torch entity
				if (::vec_pAllEntities[index]->m_Mesh->bUseStencil)						// draw a stencil around anything that isn't a torch entity
				{
					glm::vec3 scale = ::vec_pAllEntities[index]->m_Mesh->scale;				// store the scale to reset after
					int normalTexOp = ::vec_pAllEntities[index]->m_Mesh->textureOperator;	// store texture operator to reset after
					::vec_pAllEntities[index]->m_Mesh->textureOperator = 20;				// set texture operator to 20 for the stencil

					glm::vec4 stencilColour;												// decide on colour of stencil depending on type of entity
					switch (::vec_pAllEntities[index]->type)
					{
					case iEntity::ENTITY_TYPE::PLAYER:
						stencilColour = glm::vec4(0.0f, 0.3f, 0.7f, 1.0f);
						::vec_pAllEntities[index]->m_Mesh->scale = glm::vec3(scale.x * 1.1f, scale.y * 1.05f, scale.z * 1.1f);
						break;
					case iEntity::ENTITY_TYPE::ENEMY:
						stencilColour = glm::vec4(0.8f, 0.2f, 0.0f, 1.0f);
						//::vec_pAllEntities[index]->m_Mesh->scale = glm::vec3(scale.x * 1.01f, scale.y * 1.01f, scale.z * 1.01f);
						::vec_pAllEntities[index]->m_Mesh->setUniformScale(1.1f);
						break;
					case iEntity::ENTITY_TYPE::OBJECT:
						stencilColour = glm::vec4(0.2f, 0.8f, 0.0f, 1.0f);
						//::vec_pAllEntities[index]->m_Mesh->scale = glm::vec3(scale.x * 1.01f, scale.y * 1.01f, scale.z * 1.01f);
						::vec_pAllEntities[index]->m_Mesh->setUniformScale(1.1f);
						break;
					case iEntity::ENTITY_TYPE::TREASURE:
						stencilColour = glm::vec4(1.0f, 1.0f, 0.6f, 1.0f);
						//::vec_pAllEntities[index]->m_Mesh->scale = glm::vec3(scale.x * 1.01f, scale.y * 1.01f, scale.z * 1.01f);
						::vec_pAllEntities[index]->m_Mesh->setUniformScale(1.1f);
						break;
					default:
						stencilColour = glm::vec4(0.9f, 0.9f, 0.9f, 1.0f);
						::vec_pAllEntities[index]->m_Mesh->setUniformScale(1.1f);
					}

					// pass the colour to the shader
					glUniform4f(pShaderProc->mapUniformName_to_UniformLocation["stencilColour"],
						stencilColour.x, stencilColour.y, stencilColour.z, stencilColour.a);

					matModel = glm::mat4(1.0f);										// draw the stencil around the entity
					DrawObject(::vec_pAllEntities[index]->m_Mesh,
						matModel,
						pShaderProc->mapUniformName_to_UniformLocation["matModel"],
						pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"],
						program,
						::g_pVAOManager);

					// reset the texture operator and scale
					::vec_pAllEntities[index]->m_Mesh->textureOperator = normalTexOp;
					::vec_pAllEntities[index]->m_Mesh->scale = scale;
				}
			}
			// once all stencil objects are drawn, we can reset the stencil stuff
			glStencilMask(0xFF);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glEnable(GL_DEPTH_TEST);
		}	// now if we drawing the stencil outline there should be an outline around all my entities (minus the torches)


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
			errorString = "";
			if (!bloomFBO->reset(width, height, errorString))
			{
				std::cout << errorString << std::endl;
			}
			errorString = "";
			if (!pingFBO->reset(width, height, errorString))
			{
				std::cout << errorString << std::endl;
			}
			errorString = "";
			if (!pongFBO->reset(width, height, errorString))
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
			GLuint texVertexSpecularTextureNumber = ::g_pFBO->vertexSpecular_4_ID;
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

		// ok, the entire scene is drawn, now I want to create a brightness texture
		//glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["calcBloomBrightness"], (float)GL_TRUE);

		//glBindFramebuffer(GL_FRAMEBUFFER, bloomFBO->ID);
		//bloomFBO->clearBuffers(true, true);
		////glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		//glCullFace(GL_FRONT);
		//matModel = glm::mat4(1.0f);
		//DrawObject(::g_pFullScreenQuad,
		//	matModel,
		//	pShaderProc->mapUniformName_to_UniformLocation["matModel"],
		//	pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"],
		//	program,
		//	::g_pVAOManager);

		//GLuint FSO_bloomVertexMaterial_TextureUnit = 22;	    // We picked 7 just because yolo (i.e. it doesn't matter, we just had to pick one)
		//glActiveTexture(FSO_bloomVertexMaterial_TextureUnit + GL_TEXTURE0);
		//GLuint bloomVertexMaterial_TextureNumber = bloomFBO->vertexMaterialColour_1_ID;
		//glBindTexture(GL_TEXTURE_2D, bloomVertexMaterial_TextureNumber);
		//glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["texBloomBrightness"], FSO_bloomVertexMaterial_TextureUnit);

		//
		//glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["calcBloomBrightness"], (float)GL_FALSE);

		//glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bloom"], (float)GL_TRUE);
		//if (::g_UseBloom)
		//{	// world has been drawn and lighting has been calculated.  Now I need to calculate bloom

		//	bool horizontal = true, first_iteration = true;
		//	unsigned int amount = 10;
		//	glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["exposure"], 1.0f);
		//	for (unsigned int index = 0; index < amount; index++)
		//	{
		//		glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["horizontal"], horizontal ? (float)GL_TRUE : (float)GL_FALSE);
		//		if (index % 2 == 0)
		//		{
		//			glBindFramebuffer(GL_FRAMEBUFFER, pingFBO->ID);

		//			pingFBO->clearBuffers(true, true);

		//			GLuint FSO_pingVertexMaterial_TextureUnit = 14;	    // We picked 7 just because yolo (i.e. it doesn't matter, we just had to pick one)
		//			glActiveTexture(FSO_pingVertexMaterial_TextureUnit + GL_TEXTURE0);
		//			GLuint pingVertexMaterial_TextureNumber = pingFBO->vertexMaterialColour_1_ID;
		//			glBindTexture(GL_TEXTURE_2D, pingVertexMaterial_TextureNumber);
		//			glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["tex2VertexMaterialColour"], FSO_pingVertexMaterial_TextureUnit);

		//			GLuint FSO_pingNormalMaterial_TextureUnit = 15;	    // We picked 7 just because yolo (i.e. it doesn't matter, we just had to pick one)
		//			glActiveTexture(FSO_pingNormalMaterial_TextureUnit + GL_TEXTURE0);
		//			GLuint pingNormalMaterial_TextureNumber = pingFBO->vertexNormal_2_ID;
		//			glBindTexture(GL_TEXTURE_2D, pingNormalMaterial_TextureNumber);
		//			glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["tex2VertexNormal"], FSO_pingNormalMaterial_TextureUnit);

		//			GLuint FSO_pingWorldPosMaterial_TextureUnit = 16;	    // We picked 7 just because yolo (i.e. it doesn't matter, we just had to pick one)
		//			glActiveTexture(FSO_pingWorldPosMaterial_TextureUnit + GL_TEXTURE0);
		//			GLuint pingWorldPosMaterial_TextureNumber = pingFBO->vertexWorldPos_3_ID;
		//			glBindTexture(GL_TEXTURE_2D, pingWorldPosMaterial_TextureNumber);
		//			glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["tex2VertexWorldPos"], FSO_pingWorldPosMaterial_TextureUnit);

		//			GLuint FSO_pingSpecularMaterial_TextureUnit = 17;	    // We picked 7 just because yolo (i.e. it doesn't matter, we just had to pick one)
		//			glActiveTexture(FSO_pingSpecularMaterial_TextureUnit + GL_TEXTURE0);
		//			GLuint pingSpecularMaterial_TextureNumber = pingFBO->vertexSpecular_4_ID;
		//			glBindTexture(GL_TEXTURE_2D, pingSpecularMaterial_TextureNumber);
		//			glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["tex2VertexSpecular"], FSO_pingSpecularMaterial_TextureUnit);

		//			glCullFace(GL_FRONT);
		//			matModel = glm::mat4(1.0f);
		//			DrawObject(::g_pFullScreenQuad,
		//				matModel,
		//				pShaderProc->mapUniformName_to_UniformLocation["matModel"],
		//				pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"],
		//				program,
		//				::g_pVAOManager);

		//			// uniform sampler2D texLightPassColourBuffer;
		//			//GLint FSQ_textureSamplerID = glGetUniformLocation(program, "tex2LightPassColourBuffer");
		//			//glUniform1i(FSQ_textureSamplerID, FSQ_textureUnit);
		//			glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["tex2LightPassColourBuffer"], FSO_pingVertexMaterial_TextureUnit);


		//			//glBindTexture(GL_TEXTURE_2D, pingFBO->colourTexture_0_ID);
		//		}
		//		else
		//		{
		//			glBindFramebuffer(GL_FRAMEBUFFER, pongFBO->ID);

		//			pongFBO->clearBuffers(true, true);

		//			GLuint FSO_pongVertexMaterial_TextureUnit = 18;	    // We picked 7 just because yolo (i.e. it doesn't matter, we just had to pick one)
		//			glActiveTexture(FSO_pongVertexMaterial_TextureUnit + GL_TEXTURE0);
		//			GLuint pongVertexMaterial_TextureNumber = pongFBO->vertexMaterialColour_1_ID;
		//			glBindTexture(GL_TEXTURE_2D, pongVertexMaterial_TextureNumber);
		//			glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["tex2VertexMaterialColour"], FSO_pongVertexMaterial_TextureUnit);

		//			GLuint FSO_pongNormalMaterial_TextureUnit = 19;	    // We picked 7 just because yolo (i.e. it doesn't matter, we just had to pick one)
		//			glActiveTexture(FSO_pongNormalMaterial_TextureUnit + GL_TEXTURE0);
		//			GLuint pongNormalMaterial_TextureNumber = pongFBO->vertexNormal_2_ID;
		//			glBindTexture(GL_TEXTURE_2D, pongNormalMaterial_TextureNumber);
		//			glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["tex2VertexNormal"], FSO_pongNormalMaterial_TextureUnit);

		//			GLuint FSO_pongWorldPosMaterial_TextureUnit = 20;	    // We picked 7 just because yolo (i.e. it doesn't matter, we just had to pick one)
		//			glActiveTexture(FSO_pongWorldPosMaterial_TextureUnit + GL_TEXTURE0);
		//			GLuint pongWorldPosMaterial_TextureNumber = pongFBO->vertexWorldPos_3_ID;
		//			glBindTexture(GL_TEXTURE_2D, pongWorldPosMaterial_TextureNumber);
		//			glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["tex2VertexWorldPos"], FSO_pongWorldPosMaterial_TextureUnit);

		//			GLuint FSO_pongSpecularMaterial_TextureUnit = 21;	    // We picked 7 just because yolo (i.e. it doesn't matter, we just had to pick one)
		//			glActiveTexture(FSO_pongSpecularMaterial_TextureUnit + GL_TEXTURE0);
		//			GLuint pongSpecularMaterial_TextureNumber = pongFBO->vertexSpecular_4_ID;
		//			glBindTexture(GL_TEXTURE_2D, pongSpecularMaterial_TextureNumber);
		//			glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["tex2VertexSpecular"], FSO_pongSpecularMaterial_TextureUnit);

		//			glCullFace(GL_FRONT);
		//			matModel = glm::mat4(1.0f);
		//			DrawObject(::g_pFullScreenQuad,
		//				matModel,
		//				pShaderProc->mapUniformName_to_UniformLocation["matModel"],
		//				pShaderProc->mapUniformName_to_UniformLocation["matModelInverseTranspose"],
		//				program,
		//				::g_pVAOManager);

		//			 //uniform sampler2D texLightPassColourBuffer;
		//			//GLint FSQ_textureSamplerID = glGetUniformLocation(program, "tex2LightPassColourBuffer");
		//			//glUniform1i(FSQ_textureSamplerID, FSQ_textureUnit);
		//			glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["tex2LightPassColourBuffer"], FSO_pongVertexMaterial_TextureUnit);

		//			glBindTexture(GL_TEXTURE_2D, pongFBO->colourTexture_0_ID);
		//		}

		//		
		//		horizontal = !horizontal;
		//	}

		//}
		//glUniform1f(pShaderProc->mapUniformName_to_UniformLocation["bloom"], (float)GL_FALSE);

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

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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

		GLuint FSQ_textureUnit = 9;	    // We picked 7 just because yolo (i.e. it doesn't matter, we just had to pick one)
		glActiveTexture(FSQ_textureUnit + GL_TEXTURE0);
		GLuint TextureNumber = ::g_pFBO->colourTexture_0_ID;
		glBindTexture(GL_TEXTURE_2D, TextureNumber);

		// uniform sampler2D texLightPassColourBuffer;
		//GLint FSQ_textureSamplerID = glGetUniformLocation(program, "texLightPassColourBuffer");
		//glUniform1i(FSQ_textureSamplerID, FSQ_textureUnit);
		glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["texLightPassColourBuffer"], FSQ_textureUnit);

		if (::g_2DEffectOp == 8)
		{
			GLuint FSQ_textureUnit1 = 10;
			glActiveTexture(FSQ_textureUnit1 + GL_TEXTURE0);
			GLuint TextureNumber1 = ::g_pTextureManager->getTextureIDFromName("Scope.bmp");
			glBindTexture(GL_TEXTURE_2D, TextureNumber1);
			//GLint scopeTexture_Loc = glGetUniformLocation(program, "texScope");
			//glUniform1i(scopeTexture_Loc, FSQ_textureUnit1);
			glUniform1i(pShaderProc->mapUniformName_to_UniformLocation["texScope"], FSQ_textureUnit1);

		}
		else if (::g_2DEffectOp == 9)
		{
			GLuint FSQ_textureUnit1 = 11;
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
	//if (::g_ObservationMode)
	//{
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
	//}
} //fly ::camera

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


// BMP colour loader from the A.I. project, not being used currently

//char GetColourCharacter(unsigned char r, unsigned char g, unsigned char b)
//{
//	if (r == 255 && g == 0 && b == 0)		return 'r';
//	if (r == 0 && g == 255 && b == 0)		return 'g';
//	if (r == 0 && g == 0 && b == 255)		return 'b';
//	if (r == 255 && g == 255 && b == 0)		return 'y';
//	if (r == 255 && g == 255 && b == 255)	return 'w';
//	if (r == 0 && g == 0 && b == 0)			return '_';
//
//	// Colours on Resource Map 3 are corrupted, some pixels are not exact colours
//	//if (r >= 200 && g <= 50 && b <= 50)		return 'r';
//	//if (r <= 50 && g >= 200 && b <= 50)		return 'g';
//	//if (r <= 50 && g <= 50 && b >= 200)		return 'b';
//	//if (r >= 200 && g >= 200 && b <= 50)	return 'y';
//	//if (r >= 200 && g >= 200 && b >= 200)	return 'w';
//	//if (r <= 50 && g <= 50 && b <= 50)		return '_';
//	return 'x';
//}


