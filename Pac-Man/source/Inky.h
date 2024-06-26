#pragma once
#include "Enemy.h"
//#include "TileMap.h"


enum class InkyAnim {
	EYES_LEFT, EYES_RIGHT, EYES_UP, EYES_DOWN,
	WALKING_LEFT, WALKING_RIGHT, WALKING_UP, WALKING_DOWN,
	NUM_ANIMATIONS
};

struct InkyStep
{
	Point speed;	//direction
	int frames;		//duration in number of frames
	int anim;		//graphical representation
};

class Inky : public Enemy
{
public:
	Inky(const Point& p, int width, int height, int frame_size, Look look);
	~Inky();

	//Initialize the enemy with the specified look and area
	AppStatus Initialise(Look look, const AABB& area)override;


	bool Update(const AABB& box)override;


private:
	//Create the pattern behaviour
	void InitPattern();
	void MoveX();
	void MoveY();
	//Update looking direction according to the current step of the pattern
	void UpdateLook(int anim_id);

	State state;

	int current_step;	//current step of the pattern
	int current_frames;	//number of frames in the current step
	std::vector<InkyStep> pattern;
};
