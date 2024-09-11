#define Emotion_WIDTH 128
#define Emotion_HEIGHT 128
#include "Emotion.h"
#include "Novice.h"
#include "Player.h"

Emotion::Emotion()
{
	//加载图片
	textureEmotion_ = Novice::LoadTexture("./RS/Emotion/emotion.png");

	// 初始化 Mono 结构体成员
	obj_.width = Emotion_WIDTH;
	obj_.height = Emotion_HEIGHT;
	obj_.scale = Vector2(1.0f, 1.0f);
	obj_.rotate = 0.0f;

	stateTextureRange_ = 0;

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
}

Emotion::~Emotion()
{
	delete math_;
}

void Emotion::Initialize()
{
	//以画面中心为原点画图的操作方法
	affine_ = { obj_.scale,obj_.rotate,{0.0f,0.0f} };
	affine_.translate = { 768.0f, 360.0f };
}

void Emotion::Update(Player* player, Camera* camera)
{
	player->GetEmotion();

	switch (player->GetEmotion())
	{
		case EmotionState::Happy:
			stateTextureRange_ = 0;
			break;

		case EmotionState::General:
			stateTextureRange_ = 1;
			break;

		case EmotionState::Sad:
			stateTextureRange_ = 2;
			break;
	}


	//更新变换矩阵
	worldMatrix_ = math_->MakeAffine(affine_);

	wvpVpMatrix_ = math_->WvpVpMatrix(worldMatrix_, camera->GetVpVpMatrix());

	screen_ = math_->TransformSprite(local_, wvpVpMatrix_);
}

void Emotion::Draw()
{
	DrawTexture((int)stateTextureRange_ * Emotion_WIDTH, 0, (int)obj_.width, (int)obj_.height, textureEmotion_);
}

void Emotion::DrawTexture(int leftTopX, int leftTopY, int width, int height, int textureHandle)
{
	Novice::DrawQuad(
		(int)screen_.leftTop.x, (int)screen_.leftTop.y,
		(int)screen_.rightTop.x, (int)screen_.rightTop.y,
		(int)screen_.leftBottom.x, (int)screen_.leftBottom.y,
		(int)screen_.rightBottom.x, (int)screen_.rightBottom.y,
		leftTopX, leftTopY, width, height, textureHandle, WHITE);
}
