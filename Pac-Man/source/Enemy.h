#pragma once
#include "Entity.h"


#define ENEMY_FRAME_SIZE		16


#define ENEMY_PHYSICAL_WIDTH	28
#define ENEMY_PHYSICAL_HEIGHT	28

#define ENEMY_SPEED			2

enum class EnemyColor {CYAN, RED, PINK, ORANGE };

enum class State { SCATTER, CHASE, FRIGHTENED };
enum class Look { RIGHT, LEFT, UP, DOWN };

//Rendering states
enum class EnemyAnim {
	EYES_LEFT, EYES_RIGHT, EYES_UP, EYES_DOWN,
	WALKING_LEFT, WALKING_RIGHT, WALKING_UP, WALKING_DOWN,
	BLUE_LEFT, BLUE_RIGHT, BLUE_UP, BLUE_DOWN,
	NUM_ANIMATIONS
};

class Enemy : public Entity
{
public:
	Enemy(const Point& p, State s, Look view);
	~Enemy();

	void DrawVisibilityArea(const Color& col) const;
	AppStatus Initialise();


	void Update();

private:
	bool IsLookingRight() const;
	bool IsLookingLeft() const;
	bool IsLookingUp() const;
	bool IsLookingDown() const;


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
	void Dead();


	State state;
	Look look;
	AABB visibility_area;

};

