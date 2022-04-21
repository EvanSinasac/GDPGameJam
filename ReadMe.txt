Fanshawe GDP Game Jam
Dungeon Adventure
Evan Sinasac - 1081418

Dungeon Adventure is a dungeon crawling adventure stylized after the old Advanced Dungeons and Dragons video games (Eye of the Beholder, Neverwinter Nights, Secret of the Silver Blades, etc.).  Where the player can navigate a maze and attempt to escape.

Built and compiled in Visual Studios 2019 Community in Debug and Release, x64. 

CONTROLS
WASD			- Move Camera (Observation Mode only)
QE			- Raise/Lower Camera (Observation Mode Only)
Space			- Switch Mode (Observation->FirstPerson->Overhead->Observation)
1-0			- Various 2D Effects (leftovers from previous projects)
Arrow Keys		- Move/Rotate Player Character
Z			- Draw Debug Spheres on Point Light Locations
X			- Turn On/Off All Lights (based on distance from camera)
C			- Turn On/Off Debug Graph Meshes
V			- Turn On/Off Stencil Buffer
B			- Turn On/Off Debug Normals
N			- Turn On/Off Bloom Effect
M			- Switch Meshes between Wireframe and Solid Fill
L			- Turn On/Off LOD Logic
K			- Start Player Cheating
P			- Reset FBO Size to Window Size
Keypad 1-4		- Entity Speed Modifier


GRAPHICS (New Effects)
	Bloom:
		Bloom effect implemented by using an offscreen FBO to determine the "bright" areas of the scene, then blur those areas, and then additive blend the blurred texture with the scene texture.
	LOD:
		Various attempts at LOD depending on the current mode and various flags.  Use low detailed meshes for entities that are greater than a certain distance from the camera eye, enable only the nearest lights to the camera eye, or just not drawing meshes/entities that are beyond a range.  Additional explanations and details in long video.
	Stencil:
		Created a stencil effect that highlights the various entities in different colours depending on their type by using the OpenGL Stencil (glStencilFunc and glStencilMask) to draw a slightly larger mesh at the same position as the entity but only where the original mesh is not.
	Decal:
		I've changed the implementation of this from my original proposal because the way that I implemented the graph and dungeon models made it difficult to have the new texture loaded with the floor model.  Instead, there are invisible Quads located just above each floor mesh, and when an enemy entity moves from one Node to the next, it turns on the Footprint Quad on the previous node and sets the texture depending on what type of enemy it is.  These Quads then lower their Alpha Transparency each frame until reaching 0 before turning invisible again (if the quad is not visible then we don't draw it at all).  These quads also use discard transparency to eliminate the pixels of the texture that are too bright (since the footprints are grey/black).  

ARTIFICIAL INTELLIGENCE
	Really simple A.I., using Finite State Machines to determine behaviour.  I used the Graph and Node code and logic to determine what areas of the dungeon are traversable and for path finding.  The player can activate a "cheat" mode which disables their control, and uses AStar to find the best path from their current location to the dungeon exit.  The Listening Enemy type uses Breadth First Search algorithm to search all nodes up to door nodes to find if the player has entered their area, and if they have to then go up to the player and start "attacking" them.

PHYSICS & CONFIGURATION
	Implemented the basic particle physics (from Physics 1) to add a nice "fire" effect to the torches in the world.  We've had flickering lights for a while (adjusting the attenuation every frame or small amount, 0.5 seconds in this case), so now the torches also throw a particle every time it changes the attentuation mostly up from the base of the flame texture.  These particles have a mesh attached, and changes the colour of the mesh based on its time alive.  Turning from red to yellow before dying and being removed from the world.
	Basic configuration that creates and zips a distribution package when the game is run in Release x64.  I also made an installer script, and the installer takes the distribution package and installs it on the desktop.

MEDIA FUNDAMENTALS
	I implemented FMOD to have some basic sound effects in the game.  Mainly, footsteps for whenever the player moves.  Ok, only for when the player moves.  (But it's there and can be expanded upon later).

GAME ENGINE FRAMEWORKS
	I ended up making a few builders to make things a little easier for myself.  One is for building the different meshes of the dungeon for when loading the dungeon file rather than doing all the mesh work in the same function.  The other is an entity builder, again to make loading the scene a little easier for myself.  When the dungeon file is loaded, certain nodes in the graph are marked as spawnpoints, and then I run through the list of spawnpoints and spawn different types of entities; enemies, objects or treasure.

MODELS
	All models used are from various sources.  Any model not found within one of the following sources was given to us at some point by one of our professors for or during class, projects and exams.
All models with DFK or dfk in their names are from Runemark Studio's Dark Fantasy Kit
https://assetstore.unity.com/packages/3d/environments/fantasy/dark-fantasy-kit-123894
The Ivysaur model (Pokemon.ply) was made by Simon Telezhkin and uploaded to TurboSquid
https://www.turbosquid.com/FullPreview/1136333
The Hollowknight model was made by Andrea Castillo García and uploaded to Sketchfab
https://sketchfab.com/3d-models/hollow-knight-2582571713404e1e8224c72b0527e373

TEXTURES
	All textures are from various sources.  Any texture not found within one of the following sources was given to us at some point by one of our professors for or during class, projects and exams.
All textures with DFK or dfk in their names are form Runemark Studio's Dark Fantasy Kit
https://assetstore.unity.com/packages/3d/environments/fantasy/dark-fantasy-kit-123894
The Final_Pokemon textures are from Simon Telezhkin
https://www.turbosquid.com/FullPreview/1136333
The uv_hollow.bmp was made by Andrea Castillo García
https://sketchfab.com/3d-models/hollow-knight-2582571713404e1e8224c72b0527e373
Dinosaur-Footprints-clipart-2.bmp was found on clipart world
https://clipart.world/footprint-clipart/dinosaur-footprints-clipart-2/
The treads.bmp was found on dreamstime and edited by me
https://www.dreamstime.com/car-motorcycle-tire-track-vector-set-mark-truck-tread-road-concept-different-type-moto-wheel-image181979628
The metroid textures were found after a quick Google search for the Graphics 2 Midterm, and at the time I did not get sources due to the time crunch.  However, I think I've re-found the images.
metroid.bmp is from a resetera thread:
https://www.resetera.com/threads/status-of-metroid-prime-trilogy-on-dolphin.204438/page-2
metroidDreadSpider.bmp is from Game UI Database:
https://www.gameuidatabase.com/gameData.php?id=1191
metroidVisor.bmp is from ZDoom forum
https://forum.zdoom.org/viewtopic.php?f=60&t=61190

AUDIO
	The three sound effects I use for the player's footsteps are from Zelda: Majora's Mask, found at:
https://noproblo.dayjo.org/ZeldaSounds/MM/index.html

VIDEOS
Short: https://youtu.be/qt6hlfwNV0c
Long: https://youtu.be/7l1aKd3BA8A

GitHub
https://github.com/EvanSinasac/GDPGameJam.git