#include "Pinky.h"
#include "Sprite.h"

Pinky::Pinky(const Point& p, int width, int height, int frame_size, Look look) :
	Enemy(p, width, height, frame_size)
{
	state = State::SCATTER;

	current_step = 0;
	current_frames = 0;
	type = EnemyType::PINKY;
	this->look = look;
}
Pinky::~Pinky()
{
}
AppStatus Pinky::Initialise(Look look, const AABB& area)
{
	int i;
	const int n = ENEMY_FRAME_SIZE;
	const int h = ENEMY_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	render = new Sprite(data.GetTexture(Resource::IMG_ENEMIES));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for leopard sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)PinkyAnim::NUM_ANIMATIONS);



	sprite->SetAnimationDelay((int)PinkyAnim::EYES_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PinkyAnim::EYES_LEFT, { (float)0 * n, 4 * h, -n, h });

	sprite->SetAnimationDelay((int)PinkyAnim::EYES_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PinkyAnim::EYES_RIGHT, { (float)0 * n, 4 * h, n, h });

	sprite->SetAnimationDelay((int)PinkyAnim::EYES_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PinkyAnim::EYES_UP, { (float)2 * n, 4 * h, -n, h });

	sprite->SetAnimationDelay((int)PinkyAnim::EYES_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)PinkyAnim::EYES_DOWN, { (float)2 * n, 4 * h, -n, h });



	sprite->SetAnimationDelay((int)PinkyAnim::WALKING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PinkyAnim::WALKING_LEFT, { (float)2 * n, 4 * h, n, h });

	sprite->SetAnimationDelay((int)PinkyAnim::WALKING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PinkyAnim::WALKING_RIGHT, { (float)2 * n, 4 * h, n, h });

	sprite->SetAnimationDelay((int)PinkyAnim::WALKING_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PinkyAnim::WALKING_UP, { (float)2 * n, 4 * h, n, h });

	sprite->SetAnimationDelay((int)PinkyAnim::WALKING_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)PinkyAnim::WALKING_DOWN, { (float)2 * n, 4 * h, n, h });



	visibility_area = area;
	//InitPattern();

	return AppStatus::OK;
}

bool Pinky::Update(const AABB& box)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	bool shoot = false;
	int anim_id;

	if (state == State::FRIGHTENED)
	{
		//state = BlackLeopardState::RUNNING;

	}
	else if (state == State::CHASE)
	{
		MoveX();
		if (look == Look::LEFT)
		{
			sprite->SetAnimation((int)PinkyAnim::WALKING_LEFT);
		}
		else if (look == Look::RIGHT)
		{
			sprite->SetAnimation((int)PinkyAnim::WALKING_RIGHT);
		}
		else if (look == Look::UP)
		{
			sprite->SetAnimation((int)PinkyAnim::WALKING_UP);
		}
		else if (look == Look::DOWN)
		{
			sprite->SetAnimation((int)PinkyAnim::WALKING_DOWN);
		}
	}
	else if (state == State::SCATTER) {

	}
	else if (state == State::EATEN) {

	}

	sprite->Update();
	MoveY();




	return shoot;
}
void Pinky::MoveX()
{
	if (look == Look::LEFT) {
		pos.x -= ENEMY_SPEED;
	}
	else if (look == Look::RIGHT)
	{
		pos.x += ENEMY_SPEED;
	}

}
void Pinky::MoveY()
{
	if (look == Look::DOWN) {
		pos.y -= ENEMY_SPEED;
	}
	else if (look == Look::UP)
	{
		pos.y += ENEMY_SPEED;
	}

}

void Pinky::UpdateLook(int anim_id)
{
	PinkyAnim anim = (PinkyAnim)anim_id;
	look = anim == PinkyAnim::WALKING_DOWN ? Look::LEFT : Look::RIGHT;//?
}