#define BG_WIDTH 1280
#define BG_HEIGHT 720
#define MAX_BG_FRAME 4
#define BG_TIME_SCALE 1.0f
#include "Background.h"
#include "Novice.h"

Background::Background()
{
	//加载图片
	textureBackground_ = Novice::LoadTexture("./RS/Background/BG.png");
	textureGround_ = Novice::LoadTexture("./RS/Background/doro.png");
	textureKusaDoro_ = Novice::LoadTexture("./RS/Background/kusadoro.png");

	// 初始化 Mono 结构体成员
	obj_.width = BG_WIDTH;
	obj_.height = BG_HEIGHT;
	obj_.scale = Vector2(1.0f, 1.0f);
	obj_.rotate = 0.0f;

	frameNum_ = 0;
	deltaTime_ = 1.0f / 60.0f;

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

Background::~Background()
{
	delete math_;
}

void Background::Initialize()
{
	//以画面中心为原点画图的操作方法
	affine_ = { obj_.scale,obj_.rotate,{0.0f,0.0f} };
	affine_.translate = { 640.0f, 360.0f };
}

void Background::Update(Camera* camera)
{
	//背景帧数变化
	if (frameNum_ > MAX_BG_FRAME - 1) {
		frameNum_ = 0;
	}
	else {
		frameNum_ += deltaTime_ * BG_TIME_SCALE;
	}

	//更新变换矩阵
	worldMatrix_ = math_->MakeAffine(affine_);

	wvpVpMatrix_ = math_->WvpVpMatrix(worldMatrix_, camera->GetVpVpMatrix());

	screen_ = math_->TransformSprite(local_, wvpVpMatrix_);
}

void Background::Draw()
{
	DrawTexture((int)frameNum_ * (int)BG_WIDTH, 0, (int)obj_.width, (int)obj_.height, textureBackground_);
	DrawTexture(0, 0, (int)obj_.width, (int)obj_.height, textureGround_);
	DrawTexture(0, 0, (int)obj_.width, (int)obj_.height, textureKusaDoro_);
}

void Background::DrawTexture(int leftTopX, int leftTopY, int width, int height, int textureHandle)
{
	Novice::DrawQuad(
		(int)screen_.leftTop.x, (int)screen_.leftTop.y,
		(int)screen_.rightTop.x, (int)screen_.rightTop.y,
		(int)screen_.leftBottom.x, (int)screen_.leftBottom.y,
		(int)screen_.rightBottom.x, (int)screen_.rightBottom.y,
		leftTopX, leftTopY, width, height, textureHandle, WHITE);
}
