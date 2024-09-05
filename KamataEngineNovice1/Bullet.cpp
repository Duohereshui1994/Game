#include "Bullet.h"
#include "Novice.h"

Bullet::Bullet()
{
	pos_.x = -100.0f;
	pos_.y = -100.0f;
	isShot_ = false;
	width_ = 32.0f;
	height_ = 32.0f;
	speed_ = 30.0f;
	textureBullet_ = Novice::LoadTexture("./RS/3.png");

}

Bullet::~Bullet()
{
	delete math_;
}

void Bullet::Initialize()
{
	pos_.x = -100.0f;
	pos_.y = -100.0f;
	isShot_ = false;
}

void Bullet::Update(const Vector2& playerPos, const Vector2& mousePos) {

	// 射击
	Shoot(playerPos, mousePos);
}


void Bullet::Shoot(const Vector2& playerPos, const Vector2& mousePos)
{

	if (!isShot_) {
		pos_ = playerPos;
		isShot_ = true;
	}


	// 更新子弹的移动
	if (isShot_) {
		Vector2 dir = math_->Subtract(mousePos, playerPos);
		pos_ = math_->Add(pos_, math_->Multiply(speed_, math_->Normalize(dir)));

		// 可以加一个判断，确保子弹超出屏幕或者达到目标后停止
		if (pos_.x > 1280 || pos_.x < 0 || pos_.y > 720 || pos_.y < 0) {
			Initialize();
		}
	}
}


void Bullet::Draw()
{
	if (isShot_)
	{
		Novice::DrawSprite((int)pos_.x, (int)pos_.y, textureBullet_, 1, 1, 0.0f, WHITE);
	}
}
