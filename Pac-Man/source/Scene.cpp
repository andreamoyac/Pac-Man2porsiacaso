#include "Scene.h"
#include <stdio.h>
#include "Globals.h"

Scene::Scene()
{
	player = nullptr;
    level = nullptr;
	InteractableLevel = nullptr;
	
	enemies = nullptr;

	font = nullptr;
	currentLevel = 1;
	//timer? for like speed
	camera.target = { 0, 0 };				//Center of the screen
	camera.offset = { 0, MARGIN_GUI_Y };	//Offset from the target (center of the screen)
	camera.rotation = 0.0f;					
	camera.zoom = 1.0f;						

	debug = DebugMode::OFF;
}
Scene::~Scene()
{
	if (player != nullptr)
	{
		player->Release();
		delete player;
		player = nullptr;
	}
    if (level != nullptr)
    {
		level->Release();
        delete level;
        level = nullptr;
    }
	if (InteractableLevel != nullptr)
	{
		InteractableLevel->Release();
		delete InteractableLevel;
		InteractableLevel = nullptr;
	}
	for (Entity* obj : objects)
	{
		delete obj;
	}
	objects.clear();
	if (enemies != nullptr)
	{
		enemies->Release();
		delete enemies;
		enemies = nullptr;
	}
	if (font != nullptr)
	{
		delete font1;
		font = nullptr;
	}
}
AppStatus Scene::Init()
{
	//Create player
	player = new Player({ 0,0 }, State::IDLE, Look::UP);
	if (player == nullptr)
	{
		LOG("Failed to allocate memory for Player");
		return AppStatus::ERROR;
	}
	//Initialise player
	if (player->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Player");
		return AppStatus::ERROR;
	}
	//enemy manager to be easier to navigate
	enemies = new EnemyManager();
	if (enemies == nullptr)
	{
		LOG("Failed to allocate memory for Enemy Manager");
		return AppStatus::ERROR;
	}
	//Initialise Enemy manager
	if (enemies->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Enemy Manager");
		return AppStatus::ERROR;
	}

	//Create level 
    level = new TileMap();
    if (level == nullptr)
    {
        LOG("Failed to allocate memory for Level");
        return AppStatus::ERROR;
    }
	//Initialise level
	if (level->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	InteractableLevel = new TileMap();
	if (InteractableLevel == nullptr)
	{
		LOG("Failed to allocate memory for Level");
		return AppStatus::ERROR;
	}
	if (InteractableLevel->Initialise() != AppStatus::OK)
	{
		LOG("Failed to initialise Level");
		return AppStatus::ERROR;
	}
	//Load level
	if (LoadLevel(1) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::ERROR;
	}
	
	player->SetTileMap(level);
	enemies->SetTileMap(level);

	font = new Text();
	if (font == nullptr)
	{
		LOG("Failed to allocate memory for font");
		return AppStatus::ERROR;
	}
	//Initialise text font
	if (font->Initialise(Resource::IMG_FONT, "img/fontywh.png", '0', 8) != AppStatus::OK)
	{
		LOG("Failed to initialise font image");
		return AppStatus::ERROR;
	}

	transition.SetScene(1, 10, 10);

	return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	//enemies->totalEnemies = 0;
	//timer = 0;
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int *map = nullptr;
	int* InteractableMap = nullptr;
	Object *obj;
	AABB hitbox, area;
	currentLevel = stage;
	
	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if (stage == 1)
	{
		map = new int[size] {
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 5,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,  10,   9,   1,   1,   1,   1,   1,   1,	  1,   1,   1,   1,   1,   1,   6,
			 3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  23,  24,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   4,
			 3,   0,  25,  22,  22,  26,   0,  25,  22,  22,  22,  26,   0,  23,  24,   0,  25,  22,  22,  22,  26,	  0,  25,  22,  22,  26,   0,   4,
			 3,   0,  23,   0,   0,  24,   0,  23,   0,   0,   0,  24,   0,  23,  24,   0,  23,   0,   0,   0,  24,	  0,  23,   0,   0,  24,   0,   4,
			 3,   0,  27,  21,  21,  28,   0,  27,  21,  21,  21,  28,   0,  27,  28,   0,  27,  21,  21,  21,  28,	  0,  27,  21,  21,  28,   0,   4,
			 3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   4,
			 3,   0,  25,  22,  22,  26,   0,  25,  26,   0,  25,  22,  22,  22,  22,  22,  22,  26,   0,  25,  26,	  0,  25,  22,  22,  26,   0,   4,
			 3,   0,  27,  21,  21,  28,   0,  23,  24,   0,  27,  21,  21,  30,  29,  21,  21,  28,   0,  23,  24,	  0,  27,  21,  21,  28,   0,   4,
			 3,   0,   0,   0,   0,   0,   0,  23,  24,   0,   0,   0,   0,  23,  24,   0,   0,   0,   0,  23,  24,	  0,   0,   0,   0,   0,   0,   4,
			 7,   2,   2,   2,   2,  16,   0,  23,  31,  22,  22,  26,   0,  23,  24,   0,  25,  22,  22,  32,  24,	  0,  15,   2,   2,   2,   2,   8,
			 0,   0,   0,   0,   0,   3,   0,  23,  29,  21,  21,  28,   0,  27,  28,   0,  27,  21,  21,  30,  24,	  0,   4,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   3,   0,  23,  24,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  23,  24,	  0,   4,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   3,   0,  23,  24,   0,  33,   2,  20,  37,  37,  19,   2,  34,   0,  23,  24,	  0,   4,   0,   0,   0,   0,   0,
		     1,   1,   1,   1,   1,  18,   0,  27,  28,   0,   4,   0,   0,   0,   0,   0,   0,   3,   0,  27,  28,	  0,  17,   1,   1,   1,   1,   1,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   0,   0,   0,   0,   0,   0,   3,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 2,   2,   2,   2,   2,  16,   0,  25,  26,   0,   4,   0,   0,   0,   0,   0,   0,   3,   0,  25,  26,	  0,  15,   2,   2,   2,   2,   2,
			 0,   0,   0,   0,   0,   3,   0,  23,  24,   0,  35,   1,   1,   1,   1,   1,   1,  36,   0,  23,  24,	  0,   4,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   3,   0,  23,  24,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  23,  24,	  0,   4,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   3,   0,  23,  24,   0,  25,  22,  22,  22,  22,  22,  22,  26,   0,  23,  24,	  0,   4,   0,   0,   0,   0,   0,
		     5,   1,   1,   1,   1,  18,   0,  27,  28,   0,  27,  21,  21,  30,  29,  21,  21,  28,   0,  27,  28,   0,  17,   1,   1,   1,   1,   6,
			 3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  23,  24,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   4,
			 3,   0,  25,  22,  22,  26,   0,  25,  22,  22,  22,  26,   0,  23,  24,   0,  25,  22,  22,  22,  26,	  0,  25,  22,  22,  26,   0,   4,
			 3,   0,  27,  21,  30,  24,   0,  27,  21,  21,  21,  28,   0,  27,  28,   0,  27,  21,  21,  21,  28,	  0,  23,  29,  21,  28,   0,   4,
			 3,   0,   0,   0,  23,  24,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,  23,  24,   0,   0,   0,   4,
			13,  22,  26,   0,  23,  24,   0,  25,  26,   0,  25,  22,  22,  22,  22,  22,  22,  26,   0,  25,  26,	  0,  23,  24,   0,  25,  22,  11,
			14,  21,  28,   0,  27,  28,   0,  23,  24,   0,  27,  21,  21,  30,  29,  21,  21,  28,   0,  23,  24,	  0,  27,  28,   0,  27,  21,  12,
			 3,   0,   0,   0,   0,   0,   0,  23,  24,   0,   0,   0,   0,  23,  24,   0,   0,   0,   0,  23,  24,	  0,   0,   0,   0,   0,   0,   4,
			 3,   0,  25,  22,  22,  22,  22,  32,  31,  22,  22,  26,   0,  23,  24,   0,  25,  22,  22,  32,  31,	 22,  22,  22,  22,  26,   0,   4,
			 3,   0,  27,  21,  21,  21,  21,  21,  21,  21,  21,  28,   0,  27,  28,   0,  27,  21,  21,  21,  21,	 21,  21,  21,  21,  28,   0,   4,
			 3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   4,
		     7,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,	  2,   2,   2,   2,   2,   2,   8,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
		};
		InteractableMap=new int[size] {
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,   0,   0,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,   0,
			 0,  50,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,  50,   0,
			 0,  60,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,  60,   0,
			 0,  50,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,  50,   0,
			 0,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,   0,
			 0,  50,   0,   0,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,  50,   0,   0,	 50,   0,   0,   0,   0,  50,   0,
			 0,  50,   0,   0,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,  50,   0,   0,	 50,   0,   0,   0,   0,  50,   0,
			 0,  50,  50,  50,  50,  50,  50,   0,   0,  50,  50,  50,  50,   0,   0,  50,  50,  50,  50,   0,   0,  50,  50,  50,  50,  50,  50,   0,
		     0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,  -1, 101,   0,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,  -1,   0,  -1,   0,  -1,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0, 103,   0, 102,   0, 104,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,   0,   0,  50,  50,  50,  50,  50,  50,	 50,  50,  50,  50,  50,  50,   0,
			 0,  50,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,  50,   0,
			 0,  50,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,  50,   0,
			 0,  60,  50,  50,   0,   0,  50,  50,  50,  50,  50,  50,  50,  -1, 100,  50,  50,  50,  50,  50,  50,	 50,   0,   0,  50,  50,  60,   0,
			 0,   0,   0,  50,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,  50,   0,   0,	 50,   0,   0,  50,   0,   0,   0,
			 0,   0,   0,  50,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,  50,   0,   0,	 50,   0,   0,  50,   0,   0,   0,
			 0,  50,  50,  50,  50,  50,  50,   0,   0,  50,  50,  50,  50,   0,   0,  50,  50,  50,  50,   0,   0,	 50,  50,  50,  50,  50,  50,   0,
			 0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,  50,   0,
			 0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,  50,   0,
			 0,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,	 50,  50,  50,  50,  50,  50,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  110,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,   0,   0,
		};
		player->InitScore();
	}
	else if (stage == 2)
	{
		map = new int[size] {
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 5,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,  10,   9,   1,   1,   1,   1,   1,   1,	  1,   1,   1,   1,   1,   1,   6,
			 3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  23,  24,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   4,
			 3,   0,  25,  22,  22,  26,   0,  25,  22,  22,  22,  26,   0,  23,  24,   0,  25,  22,  22,  22,  26,	  0,  25,  22,  22,  26,   0,   4,
			 3,   0,  23,   0,   0,  24,   0,  23,   0,   0,   0,  24,   0,  23,  24,   0,  23,   0,   0,   0,  24,	  0,  23,   0,   0,  24,   0,   4,
			 3,   0,  27,  21,  21,  28,   0,  27,  21,  21,  21,  28,   0,  27,  28,   0,  27,  21,  21,  21,  28,	  0,  27,  21,  21,  28,   0,   4,
			 3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   4,
			 3,   0,  25,  22,  22,  26,   0,  25,  26,   0,  25,  22,  22,  22,  22,  22,  22,  26,   0,  25,  26,	  0,  25,  22,  22,  26,   0,   4,
			 3,   0,  27,  21,  21,  28,   0,  23,  24,   0,  27,  21,  21,  30,  29,  21,  21,  28,   0,  23,  24,	  0,  27,  21,  21,  28,   0,   4,
			 3,   0,   0,   0,   0,   0,   0,  23,  24,   0,   0,   0,   0,  23,  24,   0,   0,   0,   0,  23,  24,	  0,   0,   0,   0,   0,   0,   4,
			 7,   2,   2,   2,   2,  16,   0,  23,  31,  22,  22,  26,   0,  23,  24,   0,  25,  22,  22,  32,  24,	  0,  15,   2,   2,   2,   2,   8,
			 0,   0,   0,   0,   0,   3,   0,  23,  29,  21,  21,  28,   0,  27,  28,   0,  27,  21,  21,  30,  24,	  0,   4,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   3,   0,  23,  24,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  23,  24,	  0,   4,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   3,   0,  23,  24,   0,  33,   2,  20,  37,  37,  19,   2,  34,   0,  23,  24,	  0,   4,   0,   0,   0,   0,   0,
		     1,   1,   1,   1,   1,  18,   0,  27,  28,   0,   4,   0,   0,   0,   0,   0,   0,   3,   0,  27,  28,	  0,  17,   1,   1,   1,   1,   1,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   4,   0,   0,   0,   0,   0,   0,   3,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 2,   2,   2,   2,   2,  16,   0,  25,  26,   0,   4,   0,   0,   0,   0,   0,   0,   3,   0,  25,  26,	  0,  15,   2,   2,   2,   2,   2,
			 0,   0,   0,   0,   0,   3,   0,  23,  24,   0,  35,   1,   1,   1,   1,   1,   1,  36,   0,  23,  24,	  0,   4,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   3,   0,  23,  24,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  23,  24,	  0,   4,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   3,   0,  23,  24,   0,  25,  22,  22,  22,  22,  22,  22,  26,   0,  23,  24,	  0,   4,   0,   0,   0,   0,   0,
		     5,   1,   1,   1,   1,  18,   0,  27,  28,   0,  27,  21,  21,  30,  29,  21,  21,  28,   0,  27,  28,   0,  17,   1,   1,   1,   1,   6,
			 3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  23,  24,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   4,
			 3,   0,  25,  22,  22,  26,   0,  25,  22,  22,  22,  26,   0,  23,  24,   0,  25,  22,  22,  22,  26,	  0,  25,  22,  22,  26,   0,   4,
			 3,   0,  27,  21,  30,  24,   0,  27,  21,  21,  21,  28,   0,  27,  28,   0,  27,  21,  21,  21,  28,	  0,  23,  29,  21,  28,   0,   4,
			 3,   0,   0,   0,  23,  24,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,  23,  24,   0,   0,   0,   4,
			13,  22,  26,   0,  23,  24,   0,  25,  26,   0,  25,  22,  22,  22,  22,  22,  22,  26,   0,  25,  26,	  0,  23,  24,   0,  25,  22,  11,
			14,  21,  28,   0,  27,  28,   0,  23,  24,   0,  27,  21,  21,  30,  29,  21,  21,  28,   0,  23,  24,	  0,  27,  28,   0,  27,  21,  12,
			 3,   0,   0,   0,   0,   0,   0,  23,  24,   0,   0,   0,   0,  23,  24,   0,   0,   0,   0,  23,  24,	  0,   0,   0,   0,   0,   0,   4,
			 3,   0,  25,  22,  22,  22,  22,  32,  31,  22,  22,  26,   0,  23,  24,   0,  25,  22,  22,  32,  31,	 22,  22,  22,  22,  26,   0,   4,
			 3,   0,  27,  21,  21,  21,  21,  21,  21,  21,  21,  28,   0,  27,  28,   0,  27,  21,  21,  21,  21,	 21,  21,  21,  21,  28,   0,   4,
			 3,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   4,
		     7,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,	  2,   2,   2,   2,   2,   2,   8,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
		};
		InteractableMap=new int[size] {
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,   0,   0,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,   0,
			 0,  50,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,  50,   0,
			 0,  60,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,  60,   0,
			 0,  50,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,  50,   0,
			 0,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,   0,
			 0,  50,   0,   0,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,  50,   0,   0,	 50,   0,   0,   0,   0,  50,   0,
			 0,  50,   0,   0,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,  50,   0,   0,	 50,   0,   0,   0,   0,  50,   0,
			 0,  50,  50,  50,  50,  50,  50,   0,   0,  50,  50,  50,  50,   0,   0,  50,  50,  50,  50,   0,   0,  50,  50,  50,  50,  50,  50,   0,
		     0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,  -1, 101,   0,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,  -1,   0,  -1,   0,  -1,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0, 103,   0, 102,   0, 104,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,   0,   0,
			 0,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,   0,   0,  50,  50,  50,  50,  50,  50,	 50,  50,  50,  50,  50,  50,   0,
			 0,  50,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,  50,   0,
			 0,  50,   0,   0,   0,   0,  50,   0,   0,   0,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,	 50,   0,   0,   0,   0,  50,   0,
			 0,  60,  50,  50,   0,   0,  50,  50,  50,  50,  50,  50,  50,  -1, 100,  50,  50,  50,  50,  50,  50,	 50,   0,   0,  50,  50,  60,   0,
			 0,   0,   0,  50,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,  50,   0,   0,	 50,   0,   0,  50,   0,   0,   0,
			 0,   0,   0,  50,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,   0,   0,   0,  50,   0,   0,	 50,   0,   0,  50,   0,   0,   0,
			 0,  50,  50,  50,  50,  50,  50,   0,   0,  50,  50,  50,  50,   0,   0,  50,  50,  50,  50,   0,   0,	 50,  50,  50,  50,  50,  50,   0,
			 0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,  50,   0,
			 0,  50,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  50,   0,   0,  50,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,  50,   0,
			 0,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50,	 50,  50,  50,  50,  50,  50,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  110,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,   0,   0,
		};
	}
	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;	
	}

	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT); 
	InteractableLevel->Load(InteractableMap, LEVEL_WIDTH, LEVEL_HEIGHT);

	//Entities and objects
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)InteractableMap[i];
			pos.x = x * TILE_SIZE;  //otra parte en GetRenderingPosition() en entity.cpp
			//pos.y = y * TILE_SIZE + TILE_SIZE;
			pos.y = y * TILE_SIZE + TILE_SIZE - 1;
			//tile = (Tile)map[i];

			if (tile == Tile::PACMAN)
			{
				player->SetPos(pos);
			}
			else if (tile == Tile::PELLET)
			{
				obj = new Object(pos, ObjectType::PELLET);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::BLINKY_RED)
			{
				pos.x += (GHOST_FRAME_SIZE_WIDTH - GHOST_PHYSICAL_WIDTH) / 2;
				hitbox = enemies->GetEnemyHitBox(pos, EnemyType::GHOST);
				area = level->GetSweptAreaX(hitbox); //?
				enemies->Add(pos, EnemyType::GHOST, area);
				enemies->totalEnemies++;
			}
			else if (tile == Tile::ENERGIZER)
			{
				obj = new Object(pos, ObjectType::ENERGIZER);
				objects.push_back(obj);
				map[i] = 0;
			}
			else
			{
				LOG("Internal error loading scene: invalid entity or object tile id")
			}
			++i;
		}
	}
	
	level->ClearObjEntPos();
	InteractableLevel->ClearObjEntPos();


	//level->Load(InteractableMap, LEVEL_WIDTH, LEVEL_HEIGHT); ??
	delete[] InteractableMap;
	delete[] map;

	return AppStatus::OK;
}
void Scene::Update()
{
	timer++;
	if (timer == 4294967290)
	{
		timer = 0;
	}
	if (player->GetHasDied() == true)
	{
		ResetScreenTimer();

		if (timerComparision + 180 == timer)
			ResetScreen();
	} //no comprendo

	Point p1, p2;
	AABB box;

	if (transition.IsActive())
	{
		transition.Update();
	}
	else {
		if (player->GetPosX() == 0 && currentLevel == 4)
		{
			transition.SetScene(currentLevel - 1, currentLevel, 10, 10);
			int PosYtmp = player->GetPosY() - 16;
			LoadLevel(currentLevel - 1);
			player->SetPos(Point(WINDOW_WIDTH - (PLAYER_PHYSICAL_WIDTH + 10), PosYtmp));
			currentLevel--;
		}
		else if (player->GetPosX() == WINDOW_WIDTH - PLAYER_PHYSICAL_WIDTH && currentLevel == 3)
		{
			transition.SetScene(currentLevel + 1, currentLevel, 10, 10);
			int PosYtmp = player->GetPosY() + 16;
			LoadLevel(currentLevel + 1);
			player->SetPos(Point(10, PosYtmp));
			currentLevel++;
		}
		else if (player->GetPosX() == 0 && currentLevel > 1)
		{
			transition.SetScene(currentLevel - 1, currentLevel, 10, 10);
			int PosYtmp = player->GetPosY();
			LoadLevel(currentLevel - 1);
			player->SetPos(Point(WINDOW_WIDTH - (PLAYER_PHYSICAL_WIDTH + 10), PosYtmp));
			currentLevel--;
		}
		else if (player->GetPosX() == WINDOW_WIDTH - PLAYER_PHYSICAL_WIDTH && currentLevel < 5)
		{
			transition.SetScene(currentLevel + 1, currentLevel, 10, 10);
			int PosYtmp = player->GetPosY();
			LoadLevel(currentLevel + 1);
			player->SetPos(Point(10, PosYtmp));
			currentLevel++;
		}
		else if (player->GetPosX() <= 0 && currentLevel == 1)
		{
			// TODO: Change this in the player.cpp --> Stop animations if a wall is hit
			int PosYtmp = player->GetPosY();
			player->SetPos(Point(0, PosYtmp));
		}
		else if (player->GetPosX() >= WINDOW_WIDTH - PLAYER_PHYSICAL_WIDTH && currentLevel == 4)
		{
			int PosYtmp = player->GetPosY();
			player->SetPos(Point(WINDOW_WIDTH - PLAYER_PHYSICAL_WIDTH, PosYtmp));
		}
		// TODO: Add it for level 4

		//ResetScreen(); // REVIEW: this wasn't commented pre-prototype

		level->Update();
		InteractableLevel->Update();
		player->Update();
		CheckCollisions();

		hitbox = player->GetHitbox();
		enemies->Update(hitbox, weaponHitbox, player->score);
		//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 

		if (IsKeyPressed(KEY_G))
		{
			debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);

		}
		//if (IsKeyPressed(KEY_ONE))		LoadLevel(1);
		//else if (IsKeyPressed(KEY_TWO))	LoadLevel(2);
		else if (IsKeyPressed(KEY_F2))
		{
			player->GodMode();
		}
		else if (IsKeyPressed(KEY_KP_1) || IsKeyPressed(KEY_ONE))
		{
			transition.SetScene(1, currentLevel, 10, 10);
			LoadLevel(1);
			player->SetPos(Point(20, 150));
			currentLevel = 1;
		}
		else if (IsKeyPressed(KEY_KP_2) || IsKeyPressed(KEY_TWO))
		{
			//transition.SetScene(1);
			transition.SetScene(currentLevel, 2, 10, 10);
			currentLevel = 2;
			LoadLevel(2);
			player->SetPos(Point(20, 150));
		}
	}

	if ((currentLevel == 1 /*|| currentLevel == 3*/) && ((timer % 150) == 0))
	{
		AABB player_hitbox;

		player_hitbox = player->GetHitbox();
		//enemy_box = enemies->GetEnemyHitBox(ZOMBIE);
	//	enemy_box = enemies->GetEnemyHitBox(pos, EnemyType::ZOMBIE);


		Point pos;
		AABB hitbox, area;

		if (player->GetPlayerLooksRight() == true)
		{
			pos.x = WINDOW_WIDTH - FISHMAN_FRAME_SIZE_WIDTH - 1;
			pos.y = 100;
		}
		else if (player->GetPlayerLooksgLeft() == true)
		{
			pos.x = 1;
			pos.y = 148;
		}
		if (player->GetPlayerLooksUp() == true)
		{
			pos.x = WINDOW_WIDTH - FISHMAN_FRAME_SIZE_WIDTH - 1;
			pos.y = 100;
		}
		else if (player->GetPlayerLooksDown() == true)
		{
			pos.x = 1;
			pos.y = 148;
		}


		hitbox = enemies->GetEnemyHitBox(pos, EnemyType::GHOST);
		area = level->GetSweptAreaX(hitbox);//?

	}
}

void Scene::Render()
{
	BeginMode2D(camera);

    level->Render();
	InteractableLevel->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects(); 
		enemies->Draw();
		player->Draw();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
		enemies->DrawDebug();
		level->RenderGrid();
	}

	EndMode2D();
	if (transition.IsActive()) transition.Render();
	RenderGUI();
}
void Scene::Release()
{
    level->Release();
	InteractableLevel->Release();
	player->Release();
	ClearLevel();
}
void Scene::ScreenReset()
{
	//StopMusicStream();
	LoadLevel(1);
	player->SetDead(false);
	dyingTimer = false;
}
void Scene::TimerReset()
{
	if (dyingTimer == false)
	{
		dyingTimer = true;
		timerComparision = timer;
	}
}
bool Scene::GameOver()
{
	if (player->GetGameOver() == true)
	{
		player->SetGameOver(false);
		return true;
	}
}
bool Scene::End()
{
	if (player->GetEndGame() == true)
	{
		player->SetEndGame(false);
		return true;
	}
}
void Scene::CheckCollisions()
{
	AABB player_box, obj_box;
	
	player_box = player->GetHitbox();
	auto it = objects.begin();
	while (it != objects.end())
	{
		obj_box = (*it)->GetHitbox();
		if(player_box.TestAABB(obj_box))
		{
			player->IncrScore((*it)->Points());
			
			//Delete the object
			delete* it; 
			//Erase the object from the vector and get the iterator to the next valid element
			it = objects.erase(it); 
		}

		else
		{
			//Move to the next object
			++it; 
		}
	}
}
void Scene::ClearLevel()
{
	for (Object* obj : objects)
	{
		delete obj;
	}
	objects.clear();
	enemies->Release();
}
void Scene::RenderObjects() const
{
	for (Object* obj : objects)
	{
		obj->Draw();
	}
}
void Scene::RenderObjectsDebug(const Color& col) const
{
	for (Object* obj : objects)
	{
		obj->DrawDebug(col);
	}
}
void Scene::RenderGUI() const
{
	//Temporal approach
	DrawText(TextFormat("SCORE : %d", player->GetScore()), 5, 5, 8, LIGHTGRAY);
	font->Draw(157, 9, TextFormat("%02d", currentLevel));
}
