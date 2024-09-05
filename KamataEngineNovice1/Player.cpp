#include "Player.h"
#include "Novice.h"

Player::Player()
{
	textureHandleLeft_ = Novice::LoadTexture("./RS/1.png");
	textureHandleRight_ = Novice::LoadTexture("./RS/2.png");
	textureHandleUnder_ = Novice::LoadTexture("./RS/4.png");
	textureHandleDown_ = Novice::LoadTexture("./RS/5.png");
	textureHandleUp_ = Novice::LoadTexture("./RS/6.png");
	mousePosX = 0;
	mousePosY = 0;
	mousePos = Vector2(0, 0);
	frameNum_ = 0;
	downFrame_ = 0;
	upFrame_ = 0;
	deltaTime_ = 1.0f / 60.0f;

	UnderPos = Vector2(640, 500);
	UpPos = Vector2(640, 360.0f);


	// 初始化 MathFunc 对象
	math_ = new MathFunc();

	// 初始化 Mono 结构体成员
	obj_.position = UpPos;
	obj_.width = 64.0f;
	obj_.height = 64.0f;
	obj_.center = Vector2(obj_.position.x + obj_.width / 2.0f, obj_.position.y + obj_.height / 2.0f);
	obj_.scale = Vector2(1.0f, 1.0f);
	obj_.rotate = 0.0f;

	bullets_.resize(10);

	isOnGround = true;
}

Player::~Player()
{
}

void Player::Initialize()
{
	mousePosX = 0;
	mousePosY = 0;
	frameNum_ = 0;
	downFrame_ = 0;
	upFrame_ = 0;
	deltaTime_ = 1.0f / 60.0f;

	// 初始化 MathFunc 对象
	math_ = new MathFunc();

	// 初始化 Mono 结构体成员
	obj_.position = Vector2(640.0f, 360.0f);
	obj_.width = 64.0f;
	obj_.height = 64.0f;
	obj_.center = Vector2(obj_.position.x + obj_.width / 2.0f, obj_.position.y + obj_.height / 2.0f);
	obj_.scale = Vector2(1.0f, 1.0f);
	obj_.rotate = 0.0f;

	for (auto& bullet : bullets_)
	{
		bullet.Initialize();
	}

	isOnGround = true;
}

void Player::Update(char keys[], char preKeys[])
{
	SwithGround(keys, preKeys);

	Attack();


}



void Player::Draw()
{
	for (auto& bullet : bullets_)
	{
		bullet.Draw();
	}


	if (frameNum_ > 4) {
		frameNum_ = 0;
	}
	else {
		frameNum_ += deltaTime_ * 2;
	}

	switch (state_) {
		case PlayerState::OnGround:
			if (mousePosX < obj_.position.x) {
				Novice::DrawSpriteRect((int)obj_.position.x, (int)obj_.position.y, (int)frameNum_ * 64, 0, (int)obj_.width, (int)obj_.height, textureHandleLeft_, 1.0f / 4.0f, 1.0f, 0.0f, WHITE);
			}
			else if (mousePosX > obj_.position.x) {
				Novice::DrawSpriteRect((int)obj_.position.x, (int)obj_.position.y, (int)frameNum_ * 64, 0, (int)obj_.width, (int)obj_.height, textureHandleRight_, 1.0f / 4.0f, 1.0f, 0.0f, WHITE);
			}
			break;
		case PlayerState::UnderGround:
			Novice::DrawSpriteRect((int)obj_.position.x, (int)obj_.position.y, (int)frameNum_ * 64, 0, (int)obj_.width, (int)obj_.height, textureHandleUnder_, 1.0f / 4.0f, 1.0f, 0.0f, WHITE);
			break;
		case PlayerState::Up:
			if (upFrame_ >= 4) {
				upFrame_ = 0;
			}
			else {
				upFrame_ += deltaTime_ * 2;
			}
			Novice::DrawSpriteRect((int)obj_.position.x, (int)obj_.position.y, (int)frameNum_ * 64, 0, (int)obj_.width, (int)obj_.height, textureHandleUp_, 1.0f / 4.0f, 1.0f, 0.0f, WHITE);
			break;
		case PlayerState::Down:
			if (downFrame_ >= 4) {
				downFrame_ = 0;
			}
			else {
				downFrame_ += deltaTime_ * 2;
			}
			Novice::DrawSpriteRect((int)obj_.position.x, (int)obj_.position.y, (int)frameNum_ * 64, 0, (int)obj_.width, (int)obj_.height, textureHandleDown_, 1.0f / 4.0f, 1.0f, 0.0f, WHITE);
			break;
	}

}

void Player::Attack()
{
	Novice::GetMousePosition(&mousePosX, &mousePosY);
	mousePos = Vector2((float)mousePosX, (float)mousePosY);

	if (state_ == PlayerState::OnGround) {
		for (auto& bullet : bullets_)
		{
			if (Novice::IsPressMouse(0))
			{

				if (!bullet.GetIsShot())
				{
					bullet.SetIsShot(true);
					bullet.SetTargetPos(mousePos);
					bullet.Shoot(obj_.position, mousePos);  // 发射子弹
					break;  // 只发射一颗子弹
				}
			}
		}
	}

	for (auto& bullet : bullets_) {
		bullet.Update(obj_.position, mousePos);
	}
}

void Player::SwithGround(char keys[], char preKeys[])
{

	switch (state_) {
		case PlayerState::OnGround:
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				state_ = PlayerState::Down;
			}
			break;
		case PlayerState::UnderGround:
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				state_ = PlayerState::Up;
			}
			break;
		case PlayerState::Up:
			if (upFrame_ >= 4) {
				upFrame_ = 4;
				isOnGround = false;
			}
			else {
				upFrame_ += deltaTime_;
			}
			obj_.position = math_->Lerp(UnderPos, UpPos, upFrame_ / 4.0f);
			if (upFrame_ >= 4) {
				state_ = PlayerState::OnGround;
				upFrame_ = 0;
			}
			break;
		case PlayerState::Down:
			if (downFrame_ >= 4) {
				downFrame_ = 4;
				isOnGround = false;
			}
			else {
				downFrame_ += deltaTime_;
			}
			obj_.position = math_->Lerp(UpPos, UnderPos, downFrame_ / 4.0f);
			if (downFrame_ >= 4) {
				state_ = PlayerState::UnderGround;
				downFrame_ = 0;
			}
			break;
	}
}
