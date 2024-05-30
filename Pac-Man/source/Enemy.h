#pragma once
#include "Entity.h"
#include "TileMap.h"
#include "raylib.h"


#define ENEMY_FRAME_SIZE		16


#define ENEMY_PHYSICAL_WIDTH	28
#define ENEMY_PHYSICAL_HEIGHT	28

#define ENEMY_SPEED			2

enum class State { SCATTER, CHASE, FRIGHTENED, EATEN };
enum class EnemyType { BLINKY, PINKY, INKY, CLYDE };
//Rendering states
enum class EnemyAnim {
	EYES_LEFT, EYES_RIGHT, EYES_UP, EYES_DOWN,
	WALKING_LEFT, WALKING_RIGHT, WALKING_UP, WALKING_DOWN,
	NUM_ANIMATIONS
};

class Enemy : public Entity
{
public:
	Enemy(const Point& p, int width, int height, int frame_size);
	~Enemy();

	void DrawVisibilityArea(const Color& col) const;
	AppStatus Initialise();


	EnemyType type;
	TileMap* map;
	void Update();
protected:
	State currentState;
private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;
	bool IsLookingUp() const;
	bool IsLookingDown() const;

	void SetMode(State state) {
		currentState = state;
	}
	State GetMode() const { return currentState; }
	//Animation management
	void SetAnimation(int id);
	EnemyAnim GetAnimation();
	void Stop();
	void StartWalkingLeft();
	void StartWalkingRight();
	void StartWalkingUp();
	void StartWalkingDown();
	void ChangeAnimRight();
	void ChangeAnimLeft();
	void ChangeAnimUp();
	void ChangeAnimDown();
	
	void onEaten();


	State state;
	Look look;
	AABB visibility_area;

};

