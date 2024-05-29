#pragma once
#include "Enemy.h"
#include "TileMap.h"


enum class OrangeAnim {
	EYES_LEFT, EYES_RIGHT, EYES_UP, EYES_DOWN,
	ORANGE_LEFT, ORANGE_RIGHT, ORANGE_UP, ORANGE_DOWN,
	NUM_ANIMATIONS
};

class Enemy : public Entity
{
public:
	Enemy(const Point& p, State s, Look view);
	~Enemy();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);


	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;
	bool IsLookingUp() const;
	bool IsLookingDown() const;


	State state;
	Look look;

	TileMap* map;

};

