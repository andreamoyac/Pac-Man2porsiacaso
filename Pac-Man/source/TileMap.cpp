#include "TileMap.h"
#include "Globals.h"
#include "ResourceManager.h"
#include <cstring>

TileMap::TileMap()
{
	map = nullptr;
	width =0;
	height =0;
	//lifes = nullptr; diria q iria en scene
	//fruits = nullptr;
	img_tiles = nullptr;

	InitTileDictionary();
}
TileMap::~TileMap()
{
	if (map != nullptr)
	{
		delete[] map;
		map = nullptr;
	}
	/*if (fruits != nullptr)
	{
		fruits->Release();
		delete fruits;
		fruits = nullptr;
	}*/
	/*if (lifes != nullptr)
	{
		lifes->Release();
		delete lifes;
		lifes = nullptr;
	}*/
}
void TileMap::InitTileDictionary()
{
	const int n = TILE_SIZE;

	dict_rect[(int)Tile::DOUBLE_STRAIGHT_LINE_HORIZONTAL_UP] = { 10*n,  24*n, n, n };
	dict_rect[(int)Tile::DOUBLE_STRAIGHT_LINE_HORIZONTAL_DOWN] = { 13*n,  24*n, n, n };
	dict_rect[(int)Tile::DOUBLE_STRAIGHT_LINE_VERTICAL_LEFT] = { 3 * n, 24*n, n, n };
	dict_rect[(int)Tile::DOUBLE_STRAIGHT_LINE_VERTICAL_RIGHT] = { 2 * n, 24*n, n, n };
	dict_rect[(int)Tile::DOUBLE_ROUND_TILE_TL] = {  n,  24*n, n, n };
	dict_rect[(int)Tile::DOUBLE_ROUND_TILE_TR] = { 0,  24*n, n, n };
	dict_rect[(int)Tile::DOUBLE_ROUND_TILE_BL] = { 5*n, 24*n, n, n };
	dict_rect[(int)Tile::DOUBLE_ROUND_TILE_BR] = { 4* n, 24*n, n, n };
	dict_rect[(int)Tile::DOUBLE_LINE_UP_ROUND_TILE_TL] = { 6 * n,  26*n, n, n };
	dict_rect[(int)Tile::DOUBLE_LINE_UP_ROUND_TILE_TR] = { 7 * n, 26*n, n, n };
	dict_rect[(int)Tile::DOUBLE_LINE_RIGHT_ROUND_TILE_BR] = { 6 * n, 24*n, n, n };
	dict_rect[(int)Tile::DOUBLE_LINE_RIGHT_ROUND_TILE_TR] = { 8 * n, 24*n, n, n };
	dict_rect[(int)Tile::DOUBLE_LINE_LEFT_ROUND_TILE_BL] = { 7 * n, 24*n, n, n };
	dict_rect[(int)Tile::DOUBLE_LINE_LEFT_ROUND_TILE_TL] = { 9 * n, 24*n, n, n };
	dict_rect[(int)Tile::DOUBLE_ROUND_TILE_MINI_TL] = { 4 * n, 25 * n, n, n };
	dict_rect[(int)Tile::DOUBLE_ROUND_TILE_MINI_TR] = { 5 * n, 25 * n, n, n };
	dict_rect[(int)Tile::DOUBLE_ROUND_TILE_MINI_BL] = { 6 * n, 25 * n, n, n };
	dict_rect[(int)Tile::DOUBLE_ROUND_TILE_MINI_BR] = { 7 * n, 25 * n, n, n };
	dict_rect[(int)Tile::DOUBLE_END_LEFT] = { 0, 26 * n, n, n };
	dict_rect[(int)Tile::DOUBLE_END_RIGHT] = { n, 26 * n, n, n };

	dict_rect[(int)Tile::STRAIGHT_LINE_HORIZONTAL_UP] = { 12 * n, 24 * n, n, n };
	dict_rect[(int)Tile::STRAIGHT_LINE_HORIZONTAL_DOWN] = { 11 * n, 24 * n, n, n };
	dict_rect[(int)Tile::STRAIGHT_LINE_VERTICAL_RIGHT] = { 9 * n, 25 * n, n, n };
	dict_rect[(int)Tile::STRAIGHT_LINE_VERTICAL_LEFT] = { 8 * n, 25 * n, n, n };
	dict_rect[(int)Tile::SINGLE_ROUND_TILE_TL] = { 4 * n, 25 * n, n, n };
	dict_rect[(int)Tile::SINGLE_ROUND_TILE_TR] = { 5 * n, 25 * n, n, n };
	dict_rect[(int)Tile::SINGLE_ROUND_TILE_BL] = { 6 * n, 25 * n, n, n };
	dict_rect[(int)Tile::SINGLE_ROUND_TILE_BR] = { 7 * n, 25 * n, n, n };
	dict_rect[(int)Tile::BIG_ROUND_TILE_TL] = { 2 * n, 26 * n, n, n };
	dict_rect[(int)Tile::BIG_ROUND_TILE_TR] = { 3 * n, 26 * n, n, n };
	dict_rect[(int)Tile::BIG_ROUND_TILE_BL] = { 4 * n, 26 * n, n, n };
	dict_rect[(int)Tile::BIG_ROUND_TILE_BR] = { 5 * n, 26 * n, n, n };

	dict_rect[(int)Tile::SQUARE_TILE_TL] = { 10*n, 25 * n, n, n };
	dict_rect[(int)Tile::SQUARE_TILE_TR] = { 11 * n, 25 * n, n, n };
	dict_rect[(int)Tile::SQUARE_TILE_BL] = { 10 * n, 26 * n, n, n };
	dict_rect[(int)Tile::SQUARE_TILE_BR] = { 11 * n, 26 * n, n, n };
	
	dict_rect[(int)Tile::PINK_RECTANGLE_DOOR] = { 13 * n, 26 * n, n, n };

	dict_rect[(int)Tile::PELLET] = { 8 * n, 26 * n, n, n };
	dict_rect[(int)Tile::ENERGIZER] = { 12 * n, 26 * n, n, n };
	dict_rect[(int)Tile::CHERRY] = { 23 * n, 24 * n, n, n }; //DIRIA

	//dict_rect[(int)Tile::CURRENT_FRUITS] = { 12 * n, 26 * n, n, n };
	//dict_rect[(int)Tile::CURRENT_LIFES] = { 12 * n, 26 * n, n, n };

}
AppStatus TileMap::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();

	if (data.LoadTexture(Resource::IMG_TILES, "images/tilemap.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_TILES);


	/*if (data.LoadTexture(Resource::IMG_TILES, "images/tilemap.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}
	img_tiles = data.GetTexture(Resource::IMG_TILES);*/
	/*energizer = new Sprite(img_tiles);
	if (energizer == nullptr)
	{
		LOG("Failed to allocate memory for laser sprite");
		return AppStatus::ERROR;
	}
	laser->SetNumberAnimations(1);
	laser->SetAnimationDelay(0, ANIM_DELAY_FIRE);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME0]);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME1]);
	laser->AddKeyFrame(0, dict_rect[(int)Tile::LASER_FRAME2]);
	laser->SetAnimation(0);

	return AppStatus::OK;*/ //energizer and 1up
	return AppStatus::OK;
}

AppStatus TileMap::Load(int data[], int w, int h)
{
	size = w*h;
	width = w;
	height = h;

	if (map != nullptr)	delete[] map;

	map = new Tile[size];
	if (map == nullptr)	
	{
		LOG("Failed to allocate memory for tile map");
		return AppStatus::ERROR;
	}
	memcpy(map, data, size * sizeof(int));

	return AppStatus::OK;
}

void TileMap::ClearObjEntPos()
{
	int i;
	Tile tile;
	for (i = 0; i < size; ++i)
	{
		tile = map[i];
		if (IsTileEntity(tile) || IsTileObject(tile) || tile == Tile::EMPTY)
			map[i] = Tile::AIR;
	}
}
void TileMap::Update()
{
	/*if(DIE)
	lifes->Update();
	if(fruit)
	fruits->Update();*/
}
Tile TileMap::GetTileIndex(int x, int y) const
{
	int idx = x + y*width;
	if(idx < 0 || idx >= size)
	{
		LOG("Error: Index out of bounds. Tile map dimensions: %dx%d. Given index: (%d, %d)", width, height, x, y)
		return Tile::AIR;
	}
	return map[x + y * width];
}
bool TileMap::IsTileObject(Tile tile) const
{
	return Tile::SPECIAL_FIRST <= tile && tile <= Tile::SPECIAL_LAST;
}
bool TileMap::IsTileEntity(Tile tile) const
{
	return Tile::ENTITY_FIRST <= tile && tile <= Tile::ENTITY_LAST;
}
//bool TileMap::IsTileSolid(Tile tile) const
//{
//	//return (Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID_LAST);
//}
bool TileMap::IsTileSolid(Tile tile) const
{
	return (Tile::SOLID_FIRST <= tile && tile <= Tile::SOLID_LAST);
}
bool TileMap::TestCollisionWallLeft(const AABB& box) const
{
	return CollisionX(box.pos, box.height);
}
bool TileMap::TestCollisionWallRight(const AABB& box) const
{
	return CollisionX(box.pos + Point(box.width - 1, 0), box.height);
}
bool TileMap::TestCollisionWallUp(const AABB& box) const
{
	return CollisionY(box.pos, box.width); //creo q hay q cambiar esto
}
bool TileMap::TestCollisionWallDown(const AABB& box) const
{
	return CollisionY(box.pos + Point(box.height - 1, 0), box.width); //creo q hay q cambiar esto
}

bool TileMap::CollisionX(const Point& p, int distance) const
{
	int x, y, y0, y1;

	//Calculate the tile coordinates and the range of tiles to check for collision
	x = p.x / TILE_SIZE;
	y0 = p.y / TILE_SIZE;
	y1 = (p.y + distance - 1) / TILE_SIZE;
	
	//Iterate over the tiles within the vertical range
	for (y = y0; y <= y1; ++y)
	{
		//One solid tile is sufficient
		if (IsTileSolid(GetTileIndex(x, y)))
			return true;
	}
	return false;
}
bool TileMap::CollisionY(const Point& p, int distance) const
{
	int x, y, x0, x1;
	Tile tile;

	//Calculate the tile coordinates and the range of tiles to check for collision
	y = p.y / TILE_SIZE;
	x0 = p.x / TILE_SIZE;
	x1 = (p.x + distance - 1) / TILE_SIZE;

	//Iterate over the tiles within the horizontal range
	for (x = x0; x <= x1; ++x)
	{
		tile = GetTileIndex(x, y);

		//One solid or laddertop tile is sufficient
		if (IsTileSolid(tile))
			return true;
	}
	return false;
}
//AABB TileMap::GetSweptAreaX(const AABB& hitbox) const //no undertiendo
//{
//	AABB box;
//	int column, x, y, y0, y1;
//	bool collision;
//
//	box.pos.y = hitbox.pos.y;
//	box.height = hitbox.height;
//
//	column = hitbox.pos.x / TILE_SIZE;
//	y0 = hitbox.pos.y / TILE_SIZE;
//	y1 = (hitbox.pos.y + hitbox.height - 1) / TILE_SIZE;
//
//	//Compute left tile index
//	collision = false;
//	x = column - 1;
//	while (!collision && x >= 0)
//	{
//		//Iterate over the tiles within the vertical range
//		for (y = y0; y <= y1; ++y)
//		{
//			//One solid tile is sufficient
//			if (IsTileSolid(GetTileIndex(x, y)))
//			{
//				collision = true;
//				break;
//			}
//		}
//		if (!collision) x--;
//	}
//	box.pos.x = (x + 1) * TILE_SIZE;
//
//	//Compute right tile index
//	collision = false;
//	x = column + 1;
//	while (!collision && x < LEVEL_WIDTH)
//	{
//		//Iterate over the tiles within the vertical range
//		for (y = y0; y <= y1; ++y)
//		{
//			//One solid tile is sufficient
//			if (IsTileSolid(GetTileIndex(x, y)))
//			{
//				collision = true;
//				break;
//			}
//		}
//		if (!collision) x++;
//	}
//	box.width = x * TILE_SIZE - box.pos.x;
//
//	return box;
//}
void TileMap::Render()
{
	Tile tile;
	Rectangle rc;
	Vector2 pos;

	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			tile = map[i * width + j];
			if (tile != Tile::AIR)
			{
				pos.x = (float)j * TILE_SIZE;
				pos.y = (float)i * TILE_SIZE;

					rc = dict_rect[(int)tile];
					DrawTextureRec(*img_tiles, rc, pos, WHITE);
				/*else
				{
					laser->Draw((int)pos.x, (int)pos.y);
				}*/
			}
		}
	}
}
void TileMap::RenderGrid() const
{
	for (int i = 0; i < height; i++)
	{
		DrawLine(0,i*TILE_SIZE, WINDOW_WIDTH,i*TILE_SIZE,YELLOW);
	}
	for (int j = 0; j < width; j++)
	{
		DrawLine(j*TILE_SIZE, 0, j*TILE_SIZE, WINDOW_HEIGHT, YELLOW);
	}
}
void TileMap::Release()
{
	ResourceManager& data = ResourceManager::Instance(); 
	data.ReleaseTexture(Resource::IMG_TILES);

	//laser->Release();

	dict_rect.clear();
}