# Fanshawe GDP Game Jam
# Evan Sinasac - 1081418
# This installer script will create an installer "DungeonAdventure_Installer.exe"
# This installer will create a "Dungeon Adventure" folder on the desktop
# that will contain the distribution package and an uninstaller "DungeonAdventure_Uninstaller.exe"
# that will clean everything up, including itself

!include "LogicLib.nsh"

# define the name of our installer
OutFile "DungeonAdventure_Installer.exe"

Function InstallationCheck
    ${If} ${FileExists} "$DESKTOP\Dungeon Adventure"
        MessageBox MB_YESNO|MB_ICONSTOP "There is already a hello application installed!  \
        Do you want to overwrite it?" IDYES yes IDNO cancel
        yes: 
            DetailPrint "Installing over the existing program"
            Goto next
        cancel:
            DetailPrint "Cancelling the installation"
            Quit
        next:
    ${EndIf}
FunctionEnd

# define the directory to install to.
InstallDir $DESKTOP

# start our default section
Section

Call InstallationCheck

CreateDirectory "$DESKTOP\Dungeon Adventure"
CreateDirectory "$DESKTOP\Dungeon Adventure\DungeonAdventure_Distribution"
CreateDirectory "$DESKTOP\Dungeon Adventure\DungeonAdventure_Distribution\Bin"
CreateDirectory "$DESKTOP\Dungeon Adventure\DungeonAdventure_Distribution\Assets"
CreateDirectory "$DESKTOP\Dungeon Adventure\DungeonAdventure_Distribution\Assets\audio"
CreateDirectory "$DESKTOP\Dungeon Adventure\DungeonAdventure_Distribution\Assets\models"
CreateDirectory "$DESKTOP\Dungeon Adventure\DungeonAdventure_Distribution\Assets\shaders"
CreateDirectory "$DESKTOP\Dungeon Adventure\DungeonAdventure_Distribution\Assets\textures"
CreateDirectory "$DESKTOP\Dungeon Adventure\DungeonAdventure_Distribution\Assets\textures\cubemaps"
CreateDirectory "$DESKTOP\Dungeon Adventure\DungeonAdventure_Distribution\Assets\textures\DFK Textures"

SetOutPath "$INSTDIR\Dungeon Adventure"
File DungeonAdventure_Distribution.zip

# define the output path for this file
SetOutPath "$INSTDIR\Dungeon Adventure\DungeonAdventure_Distribution"

# define what to install and put in the output path
SetOutPath "$INSTDIR\Dungeon Adventure\DungeonAdventure_Distribution\Bin"
File DungeonAdventure_Distribution\Bin\assimp-vc142-mt.dll
File DungeonAdventure_Distribution\Bin\fmod.dll
File DungeonAdventure_Distribution\Bin\FreeImage.dll
File DungeonAdventure_Distribution\Bin\glew32.dll
File DungeonAdventure_Distribution\Bin\glfw3.dll
File DungeonAdventure_Distribution\Bin\freetype.dll
File DungeonAdventure_Distribution\Bin\ReadMe.txt
File DungeonAdventure_Distribution\Bin\DungeonAdventure.exe

SetOutPath "$INSTDIR\Dungeon Adventure\DungeonAdventure_Distribution\Assets"
File DungeonAdventure_Distribution\Assets\lights.txt
File DungeonAdventure_Distribution\Assets\worldFile.txt
File "DungeonAdventure_Distribution\Assets\The Catacombs of Horrendous Devastation 01 - Converted.tsv"

SetOutPath "$INSTDIR\Dungeon Adventure\DungeonAdventure_Distribution\Assets\audio"
File DungeonAdventure_Distribution\Assets\audio\audioList.txt
File DungeonAdventure_Distribution\Assets\audio\MM_Steps_Snow1.wav
File DungeonAdventure_Distribution\Assets\audio\MM_Steps_Snow2.wav
File DungeonAdventure_Distribution\Assets\audio\MM_Steps_Snow3.wav

SetOutPath "$INSTDIR\Dungeon Adventure\DungeonAdventure_Distribution\Assets\models"
File DungeonAdventure_Distribution\Assets\models\dalek2005_XYZ_N_RGBA_UV_hi_res.ply
File DungeonAdventure_Distribution\Assets\models\dalek2005_XYZ_N_RGBA_UV_low_res.ply
File DungeonAdventure_Distribution\Assets\models\dfk_bed_single_01_XYZ_N_RGBA_UV_transformed.ply
File DungeonAdventure_Distribution\Assets\models\dfk_bookshelf_XYZ_N_RGBA_UV.ply
File DungeonAdventure_Distribution\Assets\models\dfk_ceiling_XYZ_N_RGBA_UV_transformed.ply
File DungeonAdventure_Distribution\Assets\models\dfk_chairs_XYZ_N_RGBA_UV.ply
File DungeonAdventure_Distribution\Assets\models\dfk_chest_02_closed_XYZ_N_RGBA_UV_transformed.ply
File DungeonAdventure_Distribution\Assets\models\dfk_crystal_01_XYZ_N_RGBA_UV_transformed.ply
File DungeonAdventure_Distribution\Assets\models\dfk_crystal_02_XYZ_N_RGBA_UV_transformed.ply
File DungeonAdventure_Distribution\Assets\models\dfk_door_01_XYZ_N_RGBA_UV_transformed.ply
File DungeonAdventure_Distribution\Assets\models\dfk_doorframe_XYZ_N_RGBA_UV_transformed.ply
File DungeonAdventure_Distribution\Assets\models\dfk_floor_XYZ_N_RGBA_UV_transformed.ply
File DungeonAdventure_Distribution\Assets\models\dfk_gate_small_XYZ_N_RGBA_UV_transformed.ply
File DungeonAdventure_Distribution\Assets\models\dfk_stairs_wood_XYZ_N_RGBA_UV_transformed.ply
File DungeonAdventure_Distribution\Assets\models\dfk_table_large_XYZ_N_RGBA_UV_transformed.ply
File DungeonAdventure_Distribution\Assets\models\dfk_torch_holder_XYZ_N_RGBA_UV_transformed.ply
File DungeonAdventure_Distribution\Assets\models\dfk_torch_XYZ_N_RGBA_UV_transformed.ply
File DungeonAdventure_Distribution\Assets\models\dfk_wall_1_bevel_door_XYZ_N_RGBA_UV_transformed.ply
File DungeonAdventure_Distribution\Assets\models\dfk_wall_1_XYZ_N_RGBA_N_transformed.ply
File DungeonAdventure_Distribution\Assets\models\dfk_wardrobe_01_XYZ_N_RGBA_UV.ply
File DungeonAdventure_Distribution\Assets\models\Engine_Exhaust_Imposter.ply
File DungeonAdventure_Distribution\Assets\models\hollowknight_XYZ_N_RGBA_UV.ply
File DungeonAdventure_Distribution\Assets\models\Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply
File DungeonAdventure_Distribution\Assets\models\Isosphere_Smooth_Normals.ply
File DungeonAdventure_Distribution\Assets\models\Pokemon.ply
File DungeonAdventure_Distribution\Assets\models\Quad_1_sided_aligned_on_XY_plane.ply
File DungeonAdventure_Distribution\Assets\models\Quad_2_sided_aligned_on_XY_plane.ply
File DungeonAdventure_Distribution\Assets\models\Quad_x3_2_sided_axial_imposter_base_on_XY_axis.ply
File DungeonAdventure_Distribution\Assets\models\SM_Env_Consoles_01_xyz_n_rgba_uv.ply
File DungeonAdventure_Distribution\Assets\models\SM_Env_Consoles_Corner_01_xyz_n_rgba_uv.ply
File DungeonAdventure_Distribution\Assets\models\SM_Env_Construction_Wall_01_xyz_n_rgba_uv.ply
File DungeonAdventure_Distribution\Assets\models\SM_Env_Floor_01_xyz_n_rgba_uv.ply

SetOutPath "$INSTDIR\Dungeon Adventure\DungeonAdventure_Distribution\Assets\shaders"
File DungeonAdventure_Distribution\Assets\shaders\fragShader_01.glsl
File DungeonAdventure_Distribution\Assets\shaders\vertShader_01.glsl
File DungeonAdventure_Distribution\Assets\shaders\geomShader.glsl

SetOutPath "$INSTDIR\Dungeon Adventure\DungeonAdventure_Distribution\Assets\textures"
File DungeonAdventure_Distribution\Assets\textures\BrightColouredUVMap.bmp
File DungeonAdventure_Distribution\Assets\textures\Dinosaur-Footprints-clipart-2.bmp
File DungeonAdventure_Distribution\Assets\textures\Final_Pokemon_Ambient_Occlusion.bmp
File DungeonAdventure_Distribution\Assets\textures\Final_Pokemon_Diffuse.bmp
File DungeonAdventure_Distribution\Assets\textures\Final_Pokemon_Normal.bmp
File DungeonAdventure_Distribution\Assets\textures\Long_blue_Jet_Flame.bmp
File DungeonAdventure_Distribution\Assets\textures\metroid.bmp
File DungeonAdventure_Distribution\Assets\textures\metroidDreadSpider.bmp
File DungeonAdventure_Distribution\Assets\textures\metroidVisor.bmp
File DungeonAdventure_Distribution\Assets\textures\Scope.bmp
File DungeonAdventure_Distribution\Assets\textures\SpaceInteriors_Emmision.bmp
File DungeonAdventure_Distribution\Assets\textures\SpaceInteriors_Texture.bmp
File DungeonAdventure_Distribution\Assets\textures\static.bmp
File DungeonAdventure_Distribution\Assets\textures\Treads.bmp
File DungeonAdventure_Distribution\Assets\textures\uv_hollow.bmp
File DungeonAdventure_Distribution\Assets\textures\Window.bmp
File DungeonAdventure_Distribution\Assets\textures\WorldMap.bmp

SetOutPath "$INSTDIR\Dungeon Adventure\DungeonAdventure_Distribution\Assets\textures\cubemaps"
File DungeonAdventure_Distribution\Assets\textures\cubemaps\city_bk.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\city_dn.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\city_ft.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\city_lf.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\city_rt.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\city_up.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\Skyrim_Back.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\Skyrim_Bottom.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\Skyrim_Forward.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\Skyrim_Left.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\Skyrim_Right.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\Skyrim_Top.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\SpaceBox_back6_negZ.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\SpaceBox_bottom4_negY.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\SpaceBox_front5_posZ.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\SpaceBox_left2_negX.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\SpaceBox_right1_posX.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\SpaceBox_top3_posY.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\TropicalSunnyDayBack2048.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\TropicalSunnyDayDown2048.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\TropicalSunnyDayFront2048.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\TropicalSunnyDayLeft2048.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\TropicalSunnyDayRight2048.bmp
File DungeonAdventure_Distribution\Assets\textures\cubemaps\TropicalSunnyDayUp2048.bmp

SetOutPath "$INSTDIR\Dungeon Adventure\DungeonAdventure_Distribution\Assets\textures\DFK Textures"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_bed_single_01_basecolor.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_bed_single_01_normal.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_bed_single_01_SmMetAO.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_bookshelf_01_basecolor.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_bookshelf_01_normal.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_bookshelf_01_SmMetAO.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_braziers_01_basecolor.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_braziers_01_normal.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_braziers_01_SmMetAO.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_candleholders_01_basecolor.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_candleholders_01_normal.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_candleholders_01_SmMetAO.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_ceiling_stone_basecolor.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_ceiling_stone_normal.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_ceiling_stone_SmMetAo.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_chair_01_basecolor.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_chair_01_normal.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_chair_01_SmMetAO.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_crystal_blue_albedo.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_crystal_green_albedo.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_crystal_red_albedo.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_crystal_white_albedo.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_crystal_yellow_albedo.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_crystal_normal.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_crystal_SmMetAO.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_doorframe_basecolor.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_doorframe_normal.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_doorframe_SmMetAO.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_doors_01_basecolor.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_doors_01_normal.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_doors_01_SmMetAO.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_floor_01_basecolor.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_floor_01_normal.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_floor_01_smmetao.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_gate_01_basecolor.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_gate_01_normal.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_gate_01_smmetao.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_small_containers_blue_basecolor.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_small_containers_normal.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_small_containers_SmMetAO.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_stairs_01_basecolor.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_stairs_01_normal.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_stairs_01_SmMetAO.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_stairs_wood_01_albedo.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_stairs_wood_01_metallic.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_stairs_wood_01_normal.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_table_large_01_basecolor.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_table_large_01_normal.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_table_large_01_SmMetAO.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_wall_a_basecolor.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_wall_a_normal.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_wall_a_SmMetAO.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_wardrobe_01_basecolor.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_wardrobe_01_normal.bmp"
File "DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_wardrobe_01_SmMetAO.bmp"

# define uninstaller
WriteUninstaller "$INSTDIR\Dungeon Adventure\DungeonAdventure_Uninstaller.exe"

# end our default section
SectionEnd

# now start our uninstaller section
# the section will always be "Uninstall"
Section "Uninstall"
# TODO: Delete everything we made
Delete $INSTDIR\DungeonAdventure_Uninstaller.exe
Delete $INSTDIR\DungeonAdventure_Distribution.zip

Delete $INSTDIR\DungeonAdventure_Distribution\Bin\assimp-vc142-mt.dll
Delete $INSTDIR\DungeonAdventure_Distribution\Bin\fmod.dll
Delete $INSTDIR\DungeonAdventure_Distribution\Bin\FreeImage.dll
Delete $INSTDIR\DungeonAdventure_Distribution\Bin\glew32.dll
Delete $INSTDIR\DungeonAdventure_Distribution\Bin\glfw3.dll
Delete $INSTDIR\DungeonAdventure_Distribution\Bin\freetype.dll
Delete $INSTDIR\DungeonAdventure_Distribution\Bin\ReadMe.txt
Delete $INSTDIR\DungeonAdventure_Distribution\Bin\DungeonAdventure.exe

Delete $INSTDIR\DungeonAdventure_Distribution\Assets\lights.txt
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\worldFile.txt
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\The Catacombs of Horrendous Devastation 01 - Converted.tsv"

Delete $INSTDIR\DungeonAdventure_Distribution\Assets\audio\audioList.txt
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\audio\MM_Steps_Snow1.wav
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\audio\MM_Steps_Snow2.wav
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\audio\MM_Steps_Snow3.wav

Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\dalek2005_XYZ_N_RGBA_UV_hi_res.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\dalek2005_XYZ_N_RGBA_UV_low_res.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\dfk_bed_single_01_XYZ_N_RGBA_UV_transformed.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\dfk_bookshelf_XYZ_N_RGBA_UV.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\dfk_ceiling_XYZ_N_RGBA_UV_transformed.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\dfk_chairs_XYZ_N_RGBA_UV.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\dfk_chest_02_closed_XYZ_N_RGBA_UV_transformed.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\dfk_crystal_01_XYZ_N_RGBA_UV_transformed.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\dfk_crystal_02_XYZ_N_RGBA_UV_transformed.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\dfk_door_01_XYZ_N_RGBA_UV_transformed.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\dfk_doorframe_XYZ_N_RGBA_UV_transformed.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\dfk_floor_XYZ_N_RGBA_UV_transformed.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\dfk_gate_small_XYZ_N_RGBA_UV_transformed.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\dfk_stairs_wood_XYZ_N_RGBA_UV_transformed.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\dfk_table_large_XYZ_N_RGBA_UV_transformed.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\dfk_torch_holder_XYZ_N_RGBA_UV_transformed.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\dfk_torch_XYZ_N_RGBA_UV_transformed.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\dfk_wall_1_bevel_door_XYZ_N_RGBA_UV_transformed.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\dfk_wall_1_XYZ_N_RGBA_N_transformed.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\dfk_wardrobe_01_XYZ_N_RGBA_UV.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\Engine_Exhaust_Imposter.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\hollowknight_XYZ_N_RGBA_UV.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\Isosphere_Smooth_Inverted_Normals_for_SkyBox.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\Isosphere_Smooth_Normals.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\Pokemon.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\Quad_1_sided_aligned_on_XY_plane.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\Quad_2_sided_aligned_on_XY_plane.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\Quad_x3_2_sided_axial_imposter_base_on_XY_axis.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\SM_Env_Consoles_01_xyz_n_rgba_uv.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\SM_Env_Consoles_Corner_01_xyz_n_rgba_uv.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\SM_Env_Construction_Wall_01_xyz_n_rgba_uv.ply
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\models\SM_Env_Floor_01_xyz_n_rgba_uv.ply

Delete $INSTDIR\DungeonAdventure_Distribution\Assets\shaders\fragShader_01.glsl
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\shaders\vertShader_01.glsl
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\shaders\geomShader.glsl

Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\BrightColouredUVMap.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\Dinosaur-Footprints-clipart-2.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\Final_Pokemon_Ambient_Occlusion.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\Final_Pokemon_Diffuse.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\Final_Pokemon_Normal.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\Long_blue_Jet_Flame.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\metroid.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\metroidDreadSpider.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\metroidVisor.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\Scope.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\SpaceInteriors_Emmision.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\SpaceInteriors_Texture.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\static.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\Treads.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\uv_hollow.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\Window.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\WorldMap.bmp

Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\city_bk.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\city_dn.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\city_ft.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\city_lf.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\city_rt.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\city_up.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\Skyrim_Back.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\Skyrim_Bottom.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\Skyrim_Forward.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\Skyrim_Left.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\Skyrim_Right.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\Skyrim_Top.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\SpaceBox_back6_negZ.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\SpaceBox_bottom4_negY.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\SpaceBox_front5_posZ.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\SpaceBox_left2_negX.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\SpaceBox_right1_posX.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\SpaceBox_top3_posY.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\TropicalSunnyDayBack2048.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\TropicalSunnyDayDown2048.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\TropicalSunnyDayFront2048.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\TropicalSunnyDayLeft2048.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\TropicalSunnyDayRight2048.bmp
Delete $INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps\TropicalSunnyDayUp2048.bmp

Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_bed_single_01_basecolor.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_bed_single_01_normal.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_bed_single_01_SmMetAO.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_bookshelf_01_basecolor.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_bookshelf_01_normal.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_bookshelf_01_SmMetAO.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_braziers_01_basecolor.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_braziers_01_normal.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_braziers_01_SmMetAO.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_candleholders_01_basecolor.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_candleholders_01_normal.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_candleholders_01_SmMetAO.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_ceiling_stone_basecolor.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_ceiling_stone_normal.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_ceiling_stone_SmMetAo.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_chair_01_basecolor.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_chair_01_normal.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_chair_01_SmMetAO.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_crystal_blue_albedo.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_crystal_green_albedo.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_crystal_red_albedo.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_crystal_white_albedo.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_crystal_yellow_albedo.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_crystal_normal.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_crystal_SmMetAO.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_doorframe_basecolor.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_doorframe_normal.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_doorframe_SmMetAO.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_doors_01_basecolor.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_doors_01_normal.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_doors_01_SmMetAO.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_floor_01_basecolor.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_floor_01_normal.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_floor_01_smmetao.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_gate_01_basecolor.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_gate_01_normal.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_gate_01_smmetao.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_small_containers_blue_basecolor.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_small_containers_normal.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_small_containers_SmMetAO.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_stairs_01_basecolor.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_stairs_01_normal.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_stairs_01_SmMetAO.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_stairs_wood_01_albedo.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_stairs_wood_01_metallic.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_stairs_wood_01_normal.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_table_large_01_basecolor.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_table_large_01_normal.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_table_large_01_SmMetAO.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_wall_a_basecolor.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_wall_a_normal.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_wall_a_SmMetAO.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_wardrobe_01_basecolor.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_wardrobe_01_normal.bmp"
Delete "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures\dfk_wardrobe_01_SmMetAO.bmp"


# delete the directory
RMDir "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\DFK Textures"
RMDir "$INSTDIR\DungeonAdventure_Distribution\Assets\textures\cubemaps"
RMDir "$INSTDIR\DungeonAdventure_Distribution\Assets\textures"
RMDir "$INSTDIR\DungeonAdventure_Distribution\Assets\shaders"
RMDir "$INSTDIR\DungeonAdventure_Distribution\Assets\models"
RMDir "$INSTDIR\DungeonAdventure_Distribution\Assets\audio"
RMDir "$INSTDIR\DungeonAdventure_Distribution\Assets"
RMDir "$INSTDIR\DungeonAdventure_Distribution\Bin"
RMDir "$INSTDIR\DungeonAdventure_Distribution"
RMDir "$INSTDIR\..\Dungeon Adventure"

# end the uninstaller section
SectionEnd