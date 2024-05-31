
#include "Player.h"
#include "Sprite.h"
#include "Globals.h"
#include <raymath.h>
#include "raylib.h" 

Player::Player(const Point& p, StateP s, Look view) :
	Entity(p, PLAYER_PHYSICAL_WIDTH, PLAYER_PHYSICAL_HEIGHT, PLAYER_FRAME_SIZE, PLAYER_FRAME_SIZE)
{
	state = s;
	look = view;
	map = nullptr;
	lives = 3;
	score = 0;
	Dead = false;
	godMode = false;
	gameEnd = false;
	isInvincible = false;
}
Player::~Player()
{
}
AppStatus Player::Initialise()
{
	int i;
	const int n = PLAYER_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_PLAYER, "images/player.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_PLAYER));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for player sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)PlayerAnim::NUM_ANIMATIONS);
	
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_UP, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_UP, { 2*n, 6*n, 2*n, 2*n });
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_DOWN, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_DOWN, { 2 * n, 4*n, 2*n, 2*n });
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_LEFT, { 2 * n, 2 * n, 2 * n, 2 * n });
	sprite->SetAnimationDelay((int)PlayerAnim::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)PlayerAnim::IDLE_RIGHT, { 2 * n, 4 * n, 2 * n, 2 * n });

	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_RIGHT, { (float)(2*i)*n, 4*n, 2*n, 2*n });
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_LEFT, { (float)(2*i)*n, 2*n, 2*n, 2*n });
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_UP, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_UP, { (float)(2*i)*n, 6*n, 2*n, 2*n });
	sprite->SetAnimationDelay((int)PlayerAnim::WALKING_DOWN, ANIM_DELAY);
	for (i = 0; i < 3; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::WALKING_DOWN, { (float)(2*i)*n, 0, 2*n, 2*n });

	


	sprite->SetAnimationDelay((int)PlayerAnim::DEATH, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)PlayerAnim::DEATH, { (float)i * n, 6 * n, n, n });

	

	return AppStatus::OK;
}



////SOUNDS






void Player::InitScore()
{
	score = 0;
}
void Player::IncrScore(int n)
{
	score += n;
}
void Player::GodMode()
{
	godMode = !godMode;
}
int Player::GetScore()
{
	return score;
}
int Player::GetLives() const
{
	return lives;
}
bool Player::GetDead() const
{
	return Dead;
}
void Player::SetDead(bool state)
{
	Dead = state;
}

bool Player::GetGameOver() const
{
	return gameOver;
}
void Player::SetGameOver(bool state)
{
	gameOver = state;
}
bool Player::GetGameEnd() const
{
	return gameEnd;
}
void Player::SetGameEnd(bool state)
{
	gameEnd = state;
}
int Player::GetPosX()
{
	return pos.x;
}
int Player::GetPosY()
{
	return pos.y;
}
bool Player::GetHasEatenEnergizer() const
{
	return Player::hasEatenEnergizer;
}
void Player::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Player::IsLookingRight() const
{
	return look == Look::RIGHT;
}
bool Player::IsLookingLeft() const
{
	return look == Look::LEFT;
}
bool Player::IsLookingUp() const
{
	return look == Look::UP;
}
bool Player::IsLookingDown() const
{
	return look == Look::DOWN;
}
bool Player::GetPlayerInvincible() const
{
	if (isInvincible)
		return true;
	else
		return false;
}

void Player::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
PlayerAnim Player::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (PlayerAnim)sprite->GetAnimation();
}
void Player::Stop()
{
	dir = { 0,0 };
	state = StateP::IDLE;
	if (IsLookingRight())	SetAnimation((int)PlayerAnim::IDLE_RIGHT);
	else if(IsLookingLeft()) SetAnimation((int)PlayerAnim::IDLE_LEFT);
	else if (IsLookingDown()) SetAnimation((int)PlayerAnim::IDLE_DOWN);
	else if (IsLookingUp()) SetAnimation((int)PlayerAnim::IDLE_UP);
}
void Player::StartWalkingLeft()
{
	state = StateP::WALKING;
	look = Look::LEFT;
	SetAnimation((int)PlayerAnim::WALKING_LEFT);
}
void Player::StartWalkingRight()
{
	state = StateP::WALKING;
	look = Look::RIGHT;
	SetAnimation((int)PlayerAnim::WALKING_RIGHT);
}
void Player::StartWalkingUp()
{
	state = StateP::WALKING;
	look = Look::UP;
	SetAnimation((int)PlayerAnim::WALKING_UP);
}
void Player::StartWalkingDown()
{
	state = StateP::WALKING;
	look = Look::DOWN;
	SetAnimation((int)PlayerAnim::WALKING_DOWN);
}
void Player::Death()
{
	Dead = true;
	state = StateP::DEAD;
	lives--;
	
	SetAnimation((int)PlayerAnim::DEATH);
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	//while (sprite->GetIsAnimationFinished() == false)
	//{
	sprite->RepeatOnceMore();

	if (lives <= 0)
	{
		gameOver = true;

	}
	else
	{
		if (IsKeyPressed(KEY_ENTER)) {
			{
				//continuelevel
			}

	}
}
}
void Player::ChangeAnimRight()
{
	look = Look::RIGHT;
	switch (state)
	{
		case StateP::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_RIGHT);    break; 
		case StateP::WALKING: SetAnimation((int)PlayerAnim::WALKING_RIGHT); break;
	}
}
void Player::ChangeAnimLeft()
{
	look = Look::LEFT;
	switch (state)
	{
		case StateP::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_LEFT);    break;
		case StateP::WALKING: SetAnimation((int)PlayerAnim::WALKING_LEFT); break;
	}
}
void Player::ChangeAnimUp()
{
	look = Look::UP;
	switch (state)
	{
	case StateP::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_UP);    break;
	case StateP::WALKING: SetAnimation((int)PlayerAnim::WALKING_UP); break;
	}
}
void Player::ChangeAnimDown()
{
	look = Look::DOWN;
	switch (state)
	{
	case StateP::IDLE:	 SetAnimation((int)PlayerAnim::IDLE_DOWN);    break;
	case StateP::WALKING: SetAnimation((int)PlayerAnim::WALKING_DOWN); break;
	}
}
void Player::Update()
{
	//Player doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.
	MoveX();
	MoveY();

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Player::MoveX()
{
	AABB box;
	int prev_x = pos.x;


	if (IsKeyDown(KEY_LEFT))
	{
		pos.x += -PLAYER_SPEED;
		if (state == StateP::IDLE) StartWalkingLeft();
		else
		{
			if (IsLookingRight()) ChangeAnimLeft();
		}

		box = GetHitbox();
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			if (state == StateP::WALKING) Stop();
		}
	}
	else if (IsKeyDown(KEY_RIGHT))
	{
		pos.x += PLAYER_SPEED;
		if (state == StateP::IDLE) StartWalkingRight();
		else
		{
			if (IsLookingLeft()) ChangeAnimRight();
		}

		box = GetHitbox();
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			if (state == StateP::WALKING) Stop();
		}
	}
	else
	{
		if (state == StateP::WALKING) Stop();
	}
	/*pos.x += PLAYER_SPEED;
	if (state == StateP::IDLE)
		StartWalkingRight();
	else if ((IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) && Dead == false && state != StateP::WALKING)
	{
	pos.x += PLAYER_SPEED;
	if (state == State::IDLE)
		StartWalkingRight();
	else if (state == State::CROUCHING) StartWalkingRight();
	else
	{
		if (IsLookingLeft()) ChangeAnimRight();
	}

	box = GetHitbox();
	if (map->TestCollisionWallRight(box))
	{
		pos.x = prev_x;
		if (state == State::WALKING) Stop();
	}
	}*/
}
void Player::MoveY()
{
	AABB box;
	int prev_y = pos.y;


	if (IsKeyDown(KEY_UP))
	{
		pos.y += -PLAYER_SPEED;
		if (state == StateP::IDLE) StartWalkingUp();
		else
		{
			if (IsLookingDown()) ChangeAnimUp();
		}

		box = GetHitbox();
		if (map->TestCollisionWallUp(box))
		{
			pos.y = prev_y;
			if (state == StateP::WALKING) Stop();
		}
	}
	else if (IsKeyDown(KEY_DOWN))
	{
		pos.y += PLAYER_SPEED;
		if (state == StateP::IDLE) StartWalkingDown();
		else
		{
			if (IsLookingUp()) ChangeAnimDown();
		}

		box = GetHitbox();
		if (map->TestCollisionWallDown(box))
		{
			pos.y = prev_y;
			if (state == StateP::WALKING) Stop();
		}
	}
	else
	{
		if (state == StateP::WALKING) Stop();
	}
		
	}

void Player::DrawDebug(const Color& col) const
{	
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
	
	DrawText(TextFormat("Position: (%d,%d)\nSize: %dx%d\nFrame: %dx%d", pos.x, pos.y, width, height, frame_width, frame_height), 18*16, 0, 8, LIGHTGRAY);
	DrawPixel(pos.x, pos.y, WHITE);
}
void Player::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_PLAYER);

	render->Release();
}