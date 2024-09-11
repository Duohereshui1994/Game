#define ATTACK_COOLDOWN 0.2f		// 攻击冷却时间（秒）如果要改变攻速 就改这里
#define MAX_UPFRAME 4.0f			//从土里钻出来的动画的最大帧数
#define MAX_DOWNFRAME 7.0f			//钻进土里去的动画的最大帧数
#define MAX_IDLEFRAME 4.0f			//在地面或者在地下待机的动画最大帧数（共用一个）
#define UP_DOWN_TIME_SCALE 15.0f		//钻进钻出的动画速度
#define IDLE_TIME_SCALE 2.0f		//待机的动画速度
#define RECOVER_SPEED 120			//恢复速度
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

	emotionValue_ = 50;
	emotionRecover_ = RECOVER_SPEED;


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

	for (int i = 0; i < 14; i++) {
		affineFriends_[i] = { obj_.scale,obj_.rotate,{0.0f,0.0f} };
	}


	//スクリーン座標系に変化に使う
	screen_ = {};

	for (int i = 0; i < 14; i++) {

		screenFriends_[i] = {};
		worldMatrixFriends_[i] = {};
		wvpVpMatrixFriends_[i] = {};
	}

	//ワールドマトリックス
	worldMatrix_ = {};
	//wvpVp
	wvpVpMatrix_ = {};

	//=========================================================

	//伙伴保存数列 位置及状态
	//最下层 4
	friends_[10] = { {UpPos.x + PLAYER_WIDTH / 2.0f * 1.5f * 2.0f,UpPos.y},0xFFFFFFFF,false };
	friends_[11] = { {UpPos.x - PLAYER_WIDTH / 2.0f * 1.5f * 2.0f,UpPos.y},0xFFFFFFFF,false };
	friends_[12] = { {UpPos.x + PLAYER_WIDTH / 2.0f * 1.5f * 1.0f,UpPos.y},0xFFFFFFFF,false };
	friends_[13] = { {UpPos.x - PLAYER_WIDTH / 2.0f * 1.5f * 1.0f,UpPos.y},0xFFFFFFFF,false };

	//第二层 4
	friends_[6] = { {UpPos.x + PLAYER_WIDTH / 3.0f + PLAYER_WIDTH / 2.0f * 1.5f,UpPos.y + 48.0f},0xFFFFFFFF,false };
	friends_[7] = { {UpPos.x - PLAYER_WIDTH / 3.0f - PLAYER_WIDTH / 2.0f * 1.5f,UpPos.y + PLAYER_HEIGHT / 2.0f},0xFFFFFFFF,false };
	friends_[8] = { {UpPos.x + PLAYER_WIDTH / 3.0f,UpPos.y + PLAYER_HEIGHT / 2.0f},0xFFFFFFFF,false };
	friends_[9] = { {UpPos.x - PLAYER_WIDTH / 3.0f,UpPos.y + PLAYER_HEIGHT / 2.0f},0xFFFFFFFF,false };

	//第三层 3
	friends_[3] = { {UpPos.x ,UpPos.y + PLAYER_HEIGHT},0xFFFFFFFF,false };
	friends_[4] = { {UpPos.x - PLAYER_WIDTH / 2.0f * 1.5f,UpPos.y + PLAYER_HEIGHT},0xFFFFFFFF,false };
	friends_[5] = { {UpPos.x + PLAYER_WIDTH / 2.0f * 1.5f,UpPos.y + PLAYER_HEIGHT},0xFFFFFFFF,false };

	//第四层 2
	friends_[1] = { {UpPos.x - PLAYER_WIDTH / 3.0f,UpPos.y + PLAYER_HEIGHT * 1.5f},0xFFFFFFFF,false };
	friends_[2] = { {UpPos.x + PLAYER_WIDTH / 3.0f,UpPos.y + PLAYER_HEIGHT * 1.5f},0xFFFFFFFF,false };

	//第五层 1
	friends_[0] = { {UpPos.x,UpPos.y + PLAYER_HEIGHT * 2.0f},0xFFFFFFFF,false };

	for (int i = 0; i < 14; i++) {
		localFriends_[i] = {
			{ -obj_.width / 2.0f, +obj_.height / 2.0f},
			{ +obj_.width / 2.0f, +obj_.height / 2.0f},
			{ -obj_.width / 2.0f, -obj_.height / 2.0f},
			{ +obj_.width / 2.0f, -obj_.height / 2.0f},
		};

		affineFriends_[i] = { obj_.scale,obj_.rotate,{0.0f,0.0f} };
	}


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

	emotionValue_ = 50;

	affine_ = { obj_.scale,obj_.rotate,{0.0f,0.0f} };
	affine_.translate = UpPos;


	for (int i = 0; i < 14; i++) {
		affineFriends_[i].translate = friends_[i].pos_;
	}

	for (auto& bullet : bullets_)
	{
		bullet.Initialize();
	}

}

void Player::Update(char keys[], char preKeys[])
{
	if (keys[DIK_J] && !preKeys[DIK_J]) {
		// 如果当前索引对应的 friend 不活跃
		if (!friends_[currentFriendIndex].isAlive_) {
			friends_[currentFriendIndex].isAlive_ = true;
		}
		// 增加索引以便下次操作下一个 friend
		currentFriendIndex--;

		// 如果索引超过范围，重置为 0
		if (currentFriendIndex < 0) {
			currentFriendIndex = 0;
		}
	}
	if (keys[DIK_K] && !preKeys[DIK_K]) {
		// 如果当前索引对应的 friend 不活跃
		if (friends_[currentFriendIndex+1].isAlive_) {
			friends_[currentFriendIndex+1].isAlive_ = false;
		}
		// 增加索引以便下次操作下一个 friend
		currentFriendIndex++;

		// 如果索引超过范围，重置为 
		if (currentFriendIndex > 13) {
			currentFriendIndex = 14;
		}
	}
	if (keys[DIK_U] && !preKeys[DIK_U]) {
		emotionValue_ += 10;
	}
	if (keys[DIK_I] && !preKeys[DIK_I]) {
		emotionValue_ -= 10;
	}
}

void Player::Update(char keys[], char preKeys[], Camera* camera)
{
	EmotionUpdate();

	SwithGround(keys, preKeys, camera);

	Attack(camera);

	Novice::GetMousePosition(&mousePosX, &mousePosY);
	mousePos = Vector2((float)mousePosX, (float)mousePosY);
	mousePosWorld = { mousePos.x,720 - mousePos.y };

	//==================friends================
	for (int i = 0; i < 14; i++) {
		affineFriends_[i].translate = friends_[i].pos_;
	}

	worldMatrix_ = math_->MakeAffine(affine_);

	wvpVpMatrix_ = math_->WvpVpMatrix(worldMatrix_, camera->GetVpVpMatrix());

	screen_ = math_->TransformSprite(local_, wvpVpMatrix_);

	for (int i = 0; i < 14; i++) {
		worldMatrixFriends_[i] = math_->MakeAffine(affineFriends_[i]);
		wvpVpMatrixFriends_[i] = math_->WvpVpMatrix(worldMatrixFriends_[i], camera->GetVpVpMatrix());
		screenFriends_[i] = math_->TransformSprite(localFriends_[i], wvpVpMatrixFriends_[i]);
	}

}



void Player::Draw()
{
	switch (state_) {
		case PlayerState::OnGround:

			if (mousePosX < affine_.translate.x) {
				//friends
				for (int i = 0; i < 14; i++) {
					if (friends_[i].isAlive_) {
						Novice::DrawQuad(
							(int)screenFriends_[i].leftTop.x, (int)screenFriends_[i].leftTop.y,
							(int)screenFriends_[i].rightTop.x, (int)screenFriends_[i].rightTop.y,
							(int)screenFriends_[i].leftBottom.x, (int)screenFriends_[i].leftBottom.y,
							(int)screenFriends_[i].rightBottom.x, (int)screenFriends_[i].rightBottom.y,
							(int)frameNum_ * (int)PLAYER_WIDTH, 0, (int)obj_.width, (int)obj_.height, textureHandleLeft_, WHITE);
					}
				}

				//BULLET
				for (auto& bullet : bullets_)
				{
					bullet.Draw();
				}

				//本体
				DrawTexture((int)frameNum_ * (int)PLAYER_WIDTH, 0, (int)obj_.width, (int)obj_.height, textureHandleLeft_);
			}
			else if (mousePosX > affine_.translate.x) {
				//friends
				for (int i = 0; i < 14; i++) {
					if (friends_[i].isAlive_) {
						Novice::DrawQuad(
							(int)screenFriends_[i].leftTop.x, (int)screenFriends_[i].leftTop.y,
							(int)screenFriends_[i].rightTop.x, (int)screenFriends_[i].rightTop.y,
							(int)screenFriends_[i].leftBottom.x, (int)screenFriends_[i].leftBottom.y,
							(int)screenFriends_[i].rightBottom.x, (int)screenFriends_[i].rightBottom.y,
							(int)frameNum_ * (int)PLAYER_WIDTH, 0, (int)obj_.width, (int)obj_.height, textureHandleRight_, WHITE);
					}
				}

				//BULLET
				for (auto& bullet : bullets_)
				{
					bullet.Draw();
				}

				//本体
				DrawTexture((int)frameNum_ * (int)PLAYER_WIDTH, 0, (int)obj_.width, (int)obj_.height, textureHandleRight_);

			}
			break;

		case PlayerState::UnderGround:
			//BULLET
			for (auto& bullet : bullets_)
			{
				bullet.Draw();
			}

			DrawTexture((int)frameNum_ * (int)PLAYER_WIDTH, 0, (int)obj_.width, (int)obj_.height, textureHandleUnder_);
			break;

		case PlayerState::Up:
			//BULLET
			for (auto& bullet : bullets_)
			{
				bullet.Draw();
			}

			DrawTexture((int)upFrame_ * (int)PLAYER_WIDTH, 0, (int)obj_.width, (int)obj_.height, textureHandleUp_);
			break;

		case PlayerState::Down:
			//BULLET
			for (auto& bullet : bullets_)
			{
				bullet.Draw();
			}

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


			//回复心情
			emotionValue_ += (int)(deltaTime_ * RECOVER_SPEED);
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

void Player::EmotionUpdate()
{
	if (emotionValue_ > 100) {
		emotionValue_ = 100;
	}
	if (emotionValue_ < 0) {
		emotionValue_ = 0;
	}

	if (emotionValue_ > 60) {
		emotionState_ = EmotionState::Happy;
	}
	else if (emotionValue_ > 30 && emotionValue_ <= 60) {
		emotionState_ = EmotionState::General;
	}
	else {
		emotionState_ = EmotionState::Sad;
	}

}


