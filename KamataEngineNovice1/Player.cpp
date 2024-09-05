#include "Player.h"
#include "Novice.h"

Player::Player()
{
	textureHandle_ = Novice::LoadTexture("./RS/1.png");
	textureHandle2_ = Novice::LoadTexture("./RS/2.png");
	mousePosX = 0;
	mousePosY = 0;
	frameNum_ = 0;
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

	bullets_.resize(10);
}

Player::~Player()
{
}

void Player::Initialize()
{
	mousePosX = 0;
	mousePosY = 0;
	frameNum_ = 0;
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
}

void Player::Update()
{
	Novice::GetMousePosition(&mousePosX, &mousePosY);
	Vector2 mousePos;

	if (Novice::IsPressMouse(0))
	{
		mousePos = Vector2((float)mousePosX, (float)mousePosY);
		for (auto& bullet : bullets_)
		{
			if (!bullet.GetIsShot())
			{
				bullet.Shoot(obj_.position, mousePos);  // 发射子弹
				break;  // 只发射一颗子弹
			}
		}
	}

	for (auto& bullet : bullets_) {
		bullet.Update(obj_.position, mousePos);
	}

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
	if (mousePosX < obj_.position.x) {
		Novice::DrawSpriteRect((int)obj_.position.x, (int)obj_.position.y, (int)frameNum_ * 64, 0, (int)obj_.width, (int)obj_.height, textureHandle_, 1.0f / 4.0f, 1.0f, 0.0f, WHITE);
	}
	else if (mousePosX > obj_.position.x) {
		Novice::DrawSpriteRect((int)obj_.position.x, (int)obj_.position.y, (int)frameNum_ * 64, 0, (int)obj_.width, (int)obj_.height, textureHandle2_, 1.0f / 4.0f, 1.0f, 0.0f, WHITE);
	}

}
