#include "Inky.h"
#include "Sprite.h"

Inky::Inky(const Point& p, int width, int height, int frame_size, Look look) :
	Enemy(p, width, height, frame_size)
{
	state = State::SCATTER;

	current_step = 0;
	current_frames = 0;
	type = EnemyType::INKY;
	this->look = look;
}
Inky::~Inky()
{
}
AppStatus Inky::Initialise(Look look, const AABB& area)
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
	sprite->SetNumberAnimations((int)InkyAnim::NUM_ANIMATIONS);



	sprite->SetAnimationDelay((int)InkyAnim::EYES_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)InkyAnim::EYES_LEFT, { (float)0 * n, 4 * h, -n, h });

	sprite->SetAnimationDelay((int)InkyAnim::EYES_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)InkyAnim::EYES_RIGHT, { (float)0 * n, 4 * h, n, h });

	sprite->SetAnimationDelay((int)InkyAnim::EYES_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)InkyAnim::EYES_UP, { (float)2 * n, 4 * h, -n, h });

	sprite->SetAnimationDelay((int)InkyAnim::EYES_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)InkyAnim::EYES_DOWN, { (float)2 * n, 4 * h, -n, h });



	sprite->SetAnimationDelay((int)InkyAnim::WALKING_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)InkyAnim::WALKING_LEFT, { (float)2 * n, 4 * h, n, h });

	sprite->SetAnimationDelay((int)InkyAnim::WALKING_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)InkyAnim::WALKING_RIGHT, { (float)2 * n, 4 * h, n, h });

	sprite->SetAnimationDelay((int)InkyAnim::WALKING_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)InkyAnim::WALKING_UP, { (float)2 * n, 4 * h, n, h });

	sprite->SetAnimationDelay((int)InkyAnim::WALKING_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)InkyAnim::WALKING_DOWN, { (float)2 * n, 4 * h, n, h });



	visibility_area = area;
	//InitPattern();

	return AppStatus::OK;
}

bool Inky::Update(const AABB& box)
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
			sprite->SetAnimation((int)InkyAnim::WALKING_LEFT);
		}
		else if (look == Look::RIGHT)
		{
			sprite->SetAnimation((int)InkyAnim::WALKING_RIGHT);
		}
		else if (look == Look::UP)
		{
			sprite->SetAnimation((int)InkyAnim::WALKING_UP);
		}
		else if (look == Look::DOWN)
		{
			sprite->SetAnimation((int)InkyAnim::WALKING_DOWN);
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
void Inky::MoveX()
{
	if (look == Look::LEFT) {
		pos.x -= ENEMY_SPEED;
	}
	else if (look == Look::RIGHT)
	{
		pos.x += ENEMY_SPEED;
	}

}
void Inky::MoveY()
{
	if (look == Look::DOWN) {
		pos.y -= ENEMY_SPEED;
	}
	else if (look == Look::UP)
	{
		pos.y += ENEMY_SPEED;
	}

}

void Inky::UpdateLook(int anim_id)
{
	InkyAnim anim = (InkyAnim)anim_id;
	look = anim == InkyAnim::WALKING_DOWN ? Look::LEFT : Look::RIGHT;//?
}
