#pragma once
#include "Enemy.h"
//#include "TileMap.h"


enum class BlinkyAnim {
	EYES_LEFT, EYES_RIGHT, EYES_UP, EYES_DOWN,
	WALKING_LEFT, WALKING_RIGHT, WALKING_UP, WALKING_DOWN,
	NUM_ANIMATIONS
};

struct BlinkyStep
{
	Point speed;	//direction
	int frames;		//duration in number of frames
	int anim;		//graphical representation
};

class Blinky : public Enemy
{
public:
	Blinky(const Point& p, int width, int height, int frame_size);
	~Blinky();

	//Initialize the enemy with the specified look and area
	AppStatus Initialise(Look look, const AABB& area) override;


	bool Update(const AABB& box) override;


private:
	//Create the pattern behaviour
	void InitPattern();

	//Update looking direction according to the current step of the pattern
	void UpdateLook(int anim_id);

	State state;

	int current_step;	//current step of the pattern
	int current_frames;	//number of frames in the current step
	std::vector<BlinkyStep> pattern;
};



