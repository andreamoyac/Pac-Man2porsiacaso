#include "Scene.h"
#include <stdio.h>
#include "Globals.h"

Scene::Scene()
{
	player = nullptr;
    level = nullptr;
	
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
	for (Entity* obj : objects)
	{
		delete obj;
	}
	objects.clear();
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
	//pink = new Entity({ 0,0 }, State::IDLE, Look::UP);
	//if (player == nullptr)
	//{
	//	LOG("Failed to allocate memory for Player");
	//	return AppStatus::ERROR;
	//}
	////Initialise player
	//if (player->Initialise() != AppStatus::OK)
	//{
	//	LOG("Failed to initialise Player");
	//	return AppStatus::ERROR;
	//}
	//player = new Player({ 0,0 }, State::IDLE, Look::UP);
	//if (player == nullptr)
	//{
	//	LOG("Failed to allocate memory for Player");
	//	return AppStatus::ERROR;
	//}
	////Initialise player
	//if (player->Initialise() != AppStatus::OK)
	//{
	//	LOG("Failed to initialise Player");
	//	return AppStatus::ERROR;
	//}
	//player = new Player({ 0,0 }, State::IDLE, Look::UP);
	//if (player == nullptr)
	//{
	//	LOG("Failed to allocate memory for Player");
	//	return AppStatus::ERROR;
	//}
	////Initialise player
	//if (player->Initialise() != AppStatus::OK)
	//{
	//	LOG("Failed to initialise Player");
	//	return AppStatus::ERROR;
	//}
	//player = new Player({ 0,0 }, State::IDLE, Look::UP);
	//if (player == nullptr)
	//{
	//	LOG("Failed to allocate memory for Player");
	//	return AppStatus::ERROR;
	//}
	////Initialise player
	//if (player->Initialise() != AppStatus::OK)
	//{
	//	LOG("Failed to initialise Player");
	//	return AppStatus::ERROR;
	//}

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
	//Load level
	if (LoadLevel(1) != AppStatus::OK)
	{
		LOG("Failed to load Level 1");
		return AppStatus::ERROR;
	}
	
	player->SetTileMap(level);

    return AppStatus::OK;
}
AppStatus Scene::LoadLevel(int stage)
{
	int size;
	int x, y, i;
	Tile tile;
	Point pos;
	int *map = nullptr;
	int* map2 = nullptr;
	Object *obj;
	
	ClearLevel();

	size = LEVEL_WIDTH * LEVEL_HEIGHT;
	if (stage == 1)
	{
		map = new int[size] {
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  -1, 100,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,   0,   0,
			};	 
				 player->InitScore();
				 /*map = new int[size] {
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
		player->InitScore();
		map2 = new int[size] {
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
		};*/
	}
	else
	{
		//Error level doesn't exist or incorrect level number
		LOG("Failed to load level, stage %d doesn't exist", stage);
		return AppStatus::ERROR;	
	}

	//Entities and objects
	i = 0;
	for (y = 0; y < LEVEL_HEIGHT; ++y)
	{
		for (x = 0; x < LEVEL_WIDTH; ++x)
		{
			tile = (Tile)map[i];
			if (tile == Tile::EMPTY)
			{
				map[i] = 0;
			}
			else if (tile == Tile::PACMAN)
			{
				pos.x = x * TILE_SIZE ;  //otra parte en GetRenderingPosition() en entity.cpp
				pos.y = y * TILE_SIZE +TILE_SIZE;
				player->SetPos(pos);
				map[i] = 0;
			}
			/*else if (tile == Tile::ITEM_APPLE)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::APPLE);
				objects.push_back(obj);
				map[i] = 0;
			}
			else if (tile == Tile::ITEM_CHILI)
			{
				pos.x = x * TILE_SIZE;
				pos.y = y * TILE_SIZE + TILE_SIZE - 1;
				obj = new Object(pos, ObjectType::CHILI);
				objects.push_back(obj);
				map[i] = 0;
			}*/
			++i;
		}
	}
	//Tile map
	level->Load(map, LEVEL_WIDTH, LEVEL_HEIGHT);

	return AppStatus::OK;
}
void Scene::Update()
{
	Point p1, p2;
	AABB box;

	//Switch between the different debug modes: off, on (sprites & hitboxes), on (hitboxes) 
	if (IsKeyPressed(KEY_F1))
	{
		debug = (DebugMode)(((int)debug + 1) % (int)DebugMode::SIZE);

	}
	if (IsKeyPressed(KEY_ONE))		LoadLevel(1);
	else if (IsKeyPressed(KEY_TWO))	LoadLevel(2);

	level->Update();
	player->Update();
	CheckCollisions();
}
void Scene::Render()
{
	BeginMode2D(camera);

    level->Render();
	if (debug == DebugMode::OFF || debug == DebugMode::SPRITES_AND_HITBOXES)
	{
		RenderObjects(); 
		player->Draw();
	}
	if (debug == DebugMode::SPRITES_AND_HITBOXES || debug == DebugMode::ONLY_HITBOXES)
	{
		RenderObjectsDebug(YELLOW);
		player->DrawDebug(GREEN);
		level->RenderGrid();
	}

	EndMode2D();

	RenderGUI();
}
void Scene::Release()
{
    level->Release();
	player->Release();
	ClearLevel();
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
}
