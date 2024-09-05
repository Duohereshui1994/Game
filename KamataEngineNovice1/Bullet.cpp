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

	leftTop = { -width_ / 2,  height_ / 2 };
	rightTop = { width_ / 2, height_ / 2 };
	leftBottom = { -width_ / 2, -height_ / 2 };
	rightBottom = { width_ / 2, -height_ / 2 };

	leftTop_Screen = { 0.0f,0.0f };
	rightTop_Screen = { 0.0f,0.0f };
	leftBottom_Screen = { 0.0f,0.0f };
	rightBottom_Screen = { 0.0f,0.0f };

	targetPos_ = { 0.0f,0.0f };
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
		targetPos_ = mousePos;
	}


	// 更新子弹的移动
	if (isShot_) {
		Vector2 dir = math_->Subtract(targetPos_, playerPos);
		pos_ = math_->Add(pos_, math_->Multiply(speed_, math_->Normalize(dir)));

		// 可以加一个判断，确保子弹超出屏幕或者达到目标后停止
		if (pos_.x > 1280 || pos_.x < 0 || pos_.y > 720 || pos_.y < 0) {
			Initialize();
		}
	}


	if (rotate > 2.0f * (float)M_PI) {
		rotate = 0;
	}
	else {
		rotate += 1.0f / 15.0f * (float)M_PI;
	}



	leftTop_Screen = math_->Transform(leftTop, math_->MakeAffineMatrix({ 1.0f,1.0f }, rotate, pos_));
	rightTop_Screen = math_->Transform(rightTop, math_->MakeAffineMatrix({ 1.0f,1.0f }, rotate, pos_));
	leftBottom_Screen = math_->Transform(leftBottom, math_->MakeAffineMatrix({ 1.0f,1.0f }, rotate, pos_));
	rightBottom_Screen = math_->Transform(rightBottom, math_->MakeAffineMatrix({ 1.0f,1.0f }, rotate, pos_));
}


void Bullet::Draw()
{
	if (isShot_)
	{
		//Novice::DrawSprite((int)pos_.x, (int)pos_.y, textureBullet_, 1, 1, 0.0f, WHITE);
		Novice::DrawQuad((int)leftTop_Screen.x, (int)leftTop_Screen.y, (int)rightTop_Screen.x, (int)rightTop_Screen.y, (int)leftBottom_Screen.x, (int)leftBottom_Screen.y, (int)rightBottom_Screen.x, (int)rightBottom_Screen.y, 0, 0, 32, 32, textureBullet_, WHITE);
	}
}
