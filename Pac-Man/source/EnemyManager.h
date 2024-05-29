#pragma once
#include "Enemy.h"
#include "Entity.h"

class EnemyManager
{
public:
	EnemyManager();
	~EnemyManager();

	AppStatus Initialise();

	void SetTileMap(TileMap* tilemap);

	void Add(const Point& pos, EnemyColor color, const AABB& area, Look look);

	AABB GetEnemyHitBox(const Point& pos) const;

	void Update(const AABB& player_hitbox, const AABB& enemy_hitbox, int& score);
	//Draw enemies
	void Draw() const;

	//Debug enemies' hitbox
	void DrawDebug() const;

	void Release();

	int totalEnemies = 4; //?

protected:
	TileMap* map;

private:
	std::vector<Enemy*> enemies;

	//Sound enemyHit;
};

