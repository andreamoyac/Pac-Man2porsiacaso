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

	void addEnemy(const Point& pos, EnemyType type, const AABB& area, Look look);

	AABB GetEnemyHitBox(const Point& pos,EnemyType type) const;

	void Update(const AABB& player_hitbox, const AABB& weapon_hitbox, int& score);
	//Draw enemies
	void Draw() const;

	//Debug enemies' hitbox
	void DrawDebug() const;

	void Release();

	void handleGhostEaten(Enemy* ghost);
protected:
	TileMap* map;

private:
	std::vector<Enemy*> enemies;

	//Sound enemyHit;
};

