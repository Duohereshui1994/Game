#define ATTACK_COOLDOWN 0.2f		// 攻击冷却时间（秒）如果要改变攻速 就改这里
#define MAX_UPFRAME 4.0f			//从土里钻出来的动画的最大帧数
#define MAX_DOWNFRAME 7.0f			//钻进土里去的动画的最大帧数
#define MAX_IDLEFRAME 4.0f			//在地面或者在地下待机的动画最大帧数（共用一个）
#define UP_DOWN_TIME_SCALE 4.0f		//钻进钻出的动画速度
#define IDLE_TIME_SCALE 2.0f		//待机的动画速度
#define PLAYER_WIDTH 96.0f
#define PLAYER_HEIGHT 96.0f
#include "Player.h"
#include "Novice.h"

Player::Player()
{
	textureHandleLeft_ = Novice::LoadTexture("./RS/Player/player_Up_Idle_Left.png");
	textureHandleRight_ = Novice::LoadTexture("./RS/Player/player_Up_Idle_Right.png");
	textureHandleUnder_ = Novice::LoadTexture("./RS/Player/player_Under_Idle.png");
	textureHandleDown_ = Novice::LoadTexture("./RS/Player/player_Down.png");
	textureHandleUp_ = Novice::LoadTexture("./RS/Player/player_UP.png");
	mousePosX = 0;
	mousePosY = 0;
	mousePos = Vector2(0, 0);
	frameNum_ = 0;
	downFrame_ = 0;
	upFrame_ = 0;
	deltaTime_ = 1.0f / 60.0f;

	UpPos = Vector2(640.0f, 220.0f);

	UnderPos = Vector2(640.0f, 120.0f);

	attackCD_ = 0.0f;


	// 初始化 MathFunc 对象
	math_ = new MathFunc();

	// 初始化 Mono 结构体成员
	obj_.position = { obj_.center.x - obj_.width,obj_.center.y + obj_.height };
	obj_.width = PLAYER_WIDTH;
	obj_.height = PLAYER_HEIGHT;
	obj_.center = UpPos;
	obj_.scale = Vector2(1.0f, 1.0f);
	obj_.rotate = 0.0f;
	//子弹
	bullets_.resize(10);

	//==================camera=================================
	//プレイヤーのローカル座標
	local_ = {
		{ -obj_.width / 2.0f, +obj_.height / 2.0f},
		{ +obj_.width / 2.0f, +obj_.height / 2.0f},
		{ -obj_.width / 2.0f, -obj_.height / 2.0f},
		{ +obj_.width / 2.0f, -obj_.height / 2.0f},
	};
	//プレイヤー 拡縮・回転・移動
	affine_ = { obj_.scale,obj_.rotate,{0.0f,0.0f} };

	//スクリーン座標系に変化に使う
	screen_ = {};

	//ワールドマトリックス
	worldMatrix_ = {};
	//wvpVp
	wvpVpMatrix_ = {};

	//=========================================================

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

	affine_ = { obj_.scale,obj_.rotate,{0.0f,0.0f} };
	affine_.translate = UpPos;

	for (auto& bullet : bullets_)
	{
		bullet.Initialize();
	}

}

void Player::Update(char keys[], char preKeys[])
{
	if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {

	}
}

void Player::Update(char keys[], char preKeys[], Camera* camera)
{
	SwithGround(keys, preKeys, camera);

	Attack(camera);

	Novice::GetMousePosition(&mousePosX, &mousePosY);
	mousePos = Vector2((float)mousePosX, (float)mousePosY);
	mousePosWorld = { mousePos.x,720 - mousePos.y };

	worldMatrix_ = math_->MakeAffine(affine_);

	wvpVpMatrix_ = math_->WvpVpMatrix(worldMatrix_, camera->GetVpVpMatrix());

	screen_ = math_->TransformSprite(local_, wvpVpMatrix_);

}



void Player::Draw()
{

	for (auto& bullet : bullets_)
	{
		bullet.Draw();
	}

	switch (state_) {
		case PlayerState::OnGround:

			if (mousePosX < affine_.translate.x) {
				DrawTexture((int)frameNum_ * (int)PLAYER_WIDTH, 0, (int)obj_.width, (int)obj_.height, textureHandleLeft_);
			}
			else if (mousePosX > affine_.translate.x) {
				DrawTexture((int)frameNum_ * (int)PLAYER_WIDTH, 0, (int)obj_.width, (int)obj_.height, textureHandleRight_);
			}
			break;

		case PlayerState::UnderGround:
			DrawTexture((int)frameNum_ * (int)PLAYER_WIDTH, 0, (int)obj_.width, (int)obj_.height, textureHandleUnder_);
			break;

		case PlayerState::Up:

			DrawTexture((int)upFrame_ * (int)PLAYER_WIDTH, 0, (int)obj_.width, (int)obj_.height, textureHandleUp_);
			break;

		case PlayerState::Down:
			DrawTexture((int)downFrame_ * (int)PLAYER_WIDTH, 0, (int)obj_.width, (int)obj_.height, textureHandleDown_);
			break;
	}

}

void Player::DrawTexture(int leftTopX, int leftTopY, int width, int height, int textureHandle)
{
	Novice::DrawQuad(
		(int)screen_.leftTop.x, (int)screen_.leftTop.y,
		(int)screen_.rightTop.x, (int)screen_.rightTop.y,
		(int)screen_.leftBottom.x, (int)screen_.leftBottom.y,
		(int)screen_.rightBottom.x, (int)screen_.rightBottom.y,
		leftTopX, leftTopY, width, height, textureHandle, WHITE);
}

void Player::Attack(Camera* camera)
{

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
					bullet.SetPos(affine_.translate);
					bullet.SetTargetPos(mousePosWorld);
					bullet.Shoot(bullet.GetTargetPos(), affine_.translate);  // 发射子弹
					bullet.bulletSwitch_ = !bullet.bulletSwitch_;
					bullet.SetIsShot(true);
					attackCD_ = ATTACK_COOLDOWN;
					break;  // 只发射一颗子弹
				}
			}
		}
	}

	for (auto& bullet : bullets_) {
		bullet.Update(bullet.GetTargetPos(), affine_.translate, camera);
	}
}

void Player::SwithGround(char keys[], char preKeys[], Camera* camera)
{

	switch (state_) {
		case PlayerState::OnGround:

			if (frameNum_ > MAX_IDLEFRAME - 1) {
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

			if (frameNum_ > MAX_IDLEFRAME - 1) {
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
			if (upFrame_ > MAX_UPFRAME) {
				upFrame_ = MAX_UPFRAME;
			}
			else {
				upFrame_ += deltaTime_ * UP_DOWN_TIME_SCALE;
			}
			//player本体移动
			affine_.translate = math_->Lerp(UnderPos, UpPos, upFrame_ / (float)(MAX_UPFRAME - 1));
			//相机倍率缩放
			Vector2 cameraUpScale = math_->Lerp(DownCameraScale, UpCameraScale, upFrame_ / (float)(MAX_UPFRAME - 1));
			camera->SetScale(cameraUpScale);

			if (upFrame_ > MAX_UPFRAME - 1) {
				upFrame_ = 0;
				state_ = PlayerState::OnGround;
				frameNum_ = 0;
			}
			break;

		case PlayerState::Down:
			if (downFrame_ > MAX_DOWNFRAME) {
				downFrame_ = MAX_DOWNFRAME;
			}
			else {
				downFrame_ += deltaTime_ * UP_DOWN_TIME_SCALE;
			}
			//player本体移动
			affine_.translate = math_->Lerp(UpPos, UnderPos, downFrame_ / (float)(MAX_DOWNFRAME - 1));
			//相机倍率缩放
			Vector2 cameraDownScale = math_->Lerp(UpCameraScale, DownCameraScale, downFrame_ / (float)(MAX_DOWNFRAME - 1));
			camera->SetScale(cameraDownScale);
			if (downFrame_ > MAX_DOWNFRAME - 1) {
				downFrame_ = 0;
				state_ = PlayerState::UnderGround;
				frameNum_ = 0;
			}
			break;
	}
}

void Player::OnEnenyCollide()
{
}

void Player::OnFriendCollide()
{
}


