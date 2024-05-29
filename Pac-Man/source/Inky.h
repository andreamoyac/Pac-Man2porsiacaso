#pragma once
#include "Enemy.h"
#include "TileMap.h"

#define CYAN_SPEED			2
#define CYAN_ANIM_DELAY	(2*ANIM_DELAY)





//Rendering states
enum class CyanAnim {
	EYES_LEFT, EYES_RIGHT, EYES_UP, EYES_DOWN,
	WALKING_LEFT, WALKING_RIGHT, WALKING_UP, WALKING_DOWN,
	BLUE_LEFT, BLUE_RIGHT, BLUE_UP, BLUE_DOWN,
	NUM_ANIMATIONS
};

class Cyan : public Enemy
{
public:
	Cyan(const Point& p, State s, Look view);
	~Cyan();

	AppStatus Initialise();
	//void SetTileMap(TileMap* tilemap);


	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

private:
	/*bool IsLookingRight() const;
	bool IsLookingLeft() const;
	bool IsLookingUp() const;
	bool IsLookingDown() const;*/


	//Animation management
	//void SetAnimation(int id);
	//EnemyAnim GetAnimation();
	//void Stop();
	//void StartWalkingLeft();
	//void StartWalkingRight();
	//void StartWalkingUp();
	//void StartWalkingDown();
	//void ChangeAnimRight();
	//void ChangeAnimLeft();
	//void ChangeAnimUp();
	//void ChangeAnimDown();
	//void Dead();


	State state;
	Look look;

	TileMap* map;

};

