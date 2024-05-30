#include "Blinky.h"
#include "Sprite.h"

Blinky::Blinky(const Point& p, int width, int height, int frame_size, Look look) :
	Enemy(p, width, height, frame_size)
{
	state = State::SCATTER;

	current_step = 0;
	current_frames = 0;
	type = EnemyType::BLINKY;
	this->look = look;
}
Blinky::~Blinky()
{
}
AppStatus Blinky::Initialise(Look look, const AABB& area)
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
	sprite->SetNumberAnimations((int)BlinkyAnim::NUM_ANIMATIONS);



	sprite->SetAnimationDelay((int)BlinkyAnim::EYES_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)BlinkyAnim::EYES_LEFT, { (float)0 * n, 4 * h, -n, h });

	sprite->SetAnimationDelay((int)BlinkyAnim::EYES_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)BlinkyAnim::EYES_RIGHT, { (float)0 * n, 4 * h, n, h });

	sprite->SetAnimationDelay((int)BlinkyAnim::EYES_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)BlinkyAnim::EYES_UP, { (float)2 * n, 4 * h, -n, h });

	sprite->SetAnimationDelay((int)BlinkyAnim::EYES_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)BlinkyAnim::EYES_DOWN, { (float)2 * n, 4 * h, -n, h });



	sprite->SetAnimationDelay((int)BlinkyAnim::WALKING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)BlinkyAnim::WALKING_LEFT, { (float)2 * n, 4 * h, n, h });
	
	sprite->SetAnimationDelay((int)BlinkyAnim::WALKING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)BlinkyAnim::WALKING_RIGHT, { (float)2 * n, 4 * h, n, h });

	sprite->SetAnimationDelay((int)BlinkyAnim::WALKING_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)BlinkyAnim::WALKING_UP, { (float)2 * n, 4 * h, n, h });

	sprite->SetAnimationDelay((int)BlinkyAnim::WALKING_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)BlinkyAnim::WALKING_DOWN, { (float)2 * n, 4 * h, n, h });



	visibility_area = area;
	//InitPattern();

	return AppStatus::OK;
}

bool Blinky::Update(const AABB& box)
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
			sprite->SetAnimation((int)BlinkyAnim::WALKING_LEFT);
		}
		else if(look==Look::RIGHT)
		{
			sprite->SetAnimation((int)BlinkyAnim::WALKING_RIGHT);
		}
		else if (look == Look::UP)
		{
			sprite->SetAnimation((int)BlinkyAnim::WALKING_UP);
		}
		else if (look == Look::DOWN)
		{
			sprite->SetAnimation((int)BlinkyAnim::WALKING_DOWN);
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
void Blinky::MoveX()
{
	if (look == Look::LEFT) {
		pos.x -= ENEMY_SPEED;
	}
	else if (look==Look::RIGHT)
	{
		pos.x += ENEMY_SPEED;
	}

}
void Blinky::MoveY()
{
	if (look == Look::DOWN) {
		pos.y -= ENEMY_SPEED;
	}
	else if (look == Look::UP)
	{
		pos.y += ENEMY_SPEED;
	}

}

void Blinky::UpdateLook(int anim_id)
{
	BlinkyAnim anim = (BlinkyAnim)anim_id;
	look = anim == BlinkyAnim::WALKING_DOWN ? Look::LEFT : Look::RIGHT;//?
}
