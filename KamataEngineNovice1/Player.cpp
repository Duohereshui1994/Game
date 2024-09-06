#define ATTACK_COOLDOWN 1.0f	// 攻击冷却时间（秒）如果要改变攻速 就改这里
#define MAX_UPFRAME 4			//从土里钻出来的动画的最大帧数
#define MAX_DOWNFRAME 4			//钻进土里去的动画的最大帧数
#define MAX_IDLEFRAME 4			//在地面或者在地下待机的动画最大帧数（共用一个）
#define UP_DOWN_TIME_SCALE 4	//钻进钻出的动画速度
#define IDLE_TIME_SCALE 2		//待机的动画速度
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

	attackCD_ = 0.0f;


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

}

Player::~Player()
{
	delete math_;
}

void Player::Initialize()
{
	mousePosX = 0;
	mousePosY = 0;
	frameNum_ = 0;
	downFrame_ = 0;
	upFrame_ = 0;
	deltaTime_ = 1.0f / 60.0f;

	attackCD_ = 0.0f;


	obj_.position = Vector2(640.0f, 360.0f);


	for (auto& bullet : bullets_)
	{
		bullet.Initialize();
	}

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
			//if (upFrame_ >= MAX_UPFRAME) {
			//	upFrame_ = 0;
			//}
			//else {
			//	upFrame_ += deltaTime_ * 2;
			//}
			Novice::DrawSpriteRect((int)obj_.position.x, (int)obj_.position.y, (int)upFrame_ * 64, 0, (int)obj_.width, (int)obj_.height, textureHandleUp_, 1.0f / 4.0f, 1.0f, 0.0f, WHITE);
			break;
		case PlayerState::Down:
			//if (downFrame_ >= MAX_DOWNFRAME) {
			//	downFrame_ = 0;
			//}
			//else {
			//	downFrame_ += deltaTime_ * 2;
			//}
			Novice::DrawSpriteRect((int)obj_.position.x, (int)obj_.position.y, (int)downFrame_ * 64, 0, (int)obj_.width, (int)obj_.height, textureHandleDown_, 1.0f / 4.0f, 1.0f, 0.0f, WHITE);
			break;
	}

}

void Player::Attack()
{
	Novice::GetMousePosition(&mousePosX, &mousePosY);
	mousePos = Vector2((float)mousePosX, (float)mousePosY);


	if (attackCD_ <= 0) {
		attackCD_ = 0.0f;
	}
	else {
		attackCD_ -= deltaTime_;
	}

	if (state_ == PlayerState::OnGround) {
		for (auto& bullet : bullets_)
		{
			if (Novice::IsPressMouse(0) && attackCD_ <= 0)
			{
				if (!bullet.GetIsShot())
				{
					bullet.SetIsShot(true);
					bullet.SetTargetPos(mousePos);
					bullet.Shoot(obj_.position, mousePos);  // 发射子弹
					attackCD_ = ATTACK_COOLDOWN;
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

			if (frameNum_ > MAX_IDLEFRAME) {
				frameNum_ = 0;
			}
			else {
				frameNum_ += deltaTime_ * IDLE_TIME_SCALE;
			}

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				downFrame_ = 0;
				state_ = PlayerState::Down;
			}
			break;
		case PlayerState::UnderGround:

			if (frameNum_ > MAX_IDLEFRAME) {
				frameNum_ = 0;
			}
			else {
				frameNum_ += deltaTime_ * IDLE_TIME_SCALE;
			}

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				upFrame_ = 0;
				state_ = PlayerState::Up;
			}
			break;
		case PlayerState::Up:
			if (upFrame_ >= MAX_UPFRAME) {
				upFrame_ = MAX_UPFRAME;
			}
			else {
				upFrame_ += deltaTime_ * UP_DOWN_TIME_SCALE;
			}
			obj_.position = math_->Lerp(UnderPos, UpPos, upFrame_ / (float)MAX_UPFRAME);
			if (upFrame_ >= MAX_UPFRAME) {
				upFrame_ = 0;
				state_ = PlayerState::OnGround;
				frameNum_ = 0;
			}
			break;
		case PlayerState::Down:
			if (downFrame_ >= MAX_DOWNFRAME) {
				downFrame_ = MAX_DOWNFRAME;
			}
			else {
				downFrame_ += deltaTime_ * UP_DOWN_TIME_SCALE;
			}
			obj_.position = math_->Lerp(UpPos, UnderPos, downFrame_ / (float)MAX_DOWNFRAME);
			if (downFrame_ >= MAX_DOWNFRAME) {
				downFrame_ = 0;
				state_ = PlayerState::UnderGround;
				frameNum_ = 0;
			}
			break;
	}
}


