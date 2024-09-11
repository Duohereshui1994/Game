#define FILTER_WIDTH 1940
#define FILTER_HEIGHT 1100
#define MAX_UPFRAME 4.0f			//从土里钻出来的动画的最大帧数
#define MAX_DOWNFRAME 7.0f			//钻进土里去的动画的最大帧数
#include "Novice.h"
#include "Filter.h"
#include "Player.h"

Filter::Filter()
{
	//加载图片
	textureFilter_ = Novice::LoadTexture("./RS/UI/UI_filter.png");

	upViewScale = { 1.0f,1.0f };
	downViewScale = { 2.0f,2.0f };

	// 初始化 Mono 结构体成员
	obj_.width = FILTER_WIDTH;
	obj_.height = FILTER_HEIGHT;
	obj_.scale = upViewScale;
	obj_.rotate = 0.0f;

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

Filter::~Filter()
{
	delete math_;
}

void Filter::Initialize()
{
	//以画面中心为原点画图的操作方法
	affine_ = { obj_.scale,obj_.rotate,{0.0f,0.0f} };
	affine_.translate = { 640.0f, 360.0f };
}

void Filter::Update(Player* player, Camera* camera)
{
	switch (player->GetState())
	{
		case PlayerState::OnGround:
			obj_.scale = upViewScale;
			break;
		case PlayerState::UnderGround:
			obj_.scale = downViewScale;
			break;
		case PlayerState::Up:
			obj_.scale = math_->Lerp(downViewScale, upViewScale, player->GetUpFrame() / (float)(MAX_UPFRAME - 1));
			break;
		case PlayerState::Down:
			obj_.scale = math_->Lerp(upViewScale, downViewScale, player->GetDownFrame() / (float)(MAX_DOWNFRAME - 1));
			break;
		default:
			break;
	}
	affine_.scale = obj_.scale;
	//更新变换矩阵
	worldMatrix_ = math_->MakeAffine(affine_);

	wvpVpMatrix_ = math_->WvpVpMatrix(worldMatrix_, camera->GetVpVpMatrix());

	screen_ = math_->TransformSprite(local_, wvpVpMatrix_);
}

void Filter::Draw()
{
	DrawTexture(0, 0, (int)obj_.width, (int)obj_.height, textureFilter_);
}

void Filter::DrawTexture(int leftTopX, int leftTopY, int width, int height, int textureHandle)
{
	Novice::DrawQuad(
		(int)screen_.leftTop.x, (int)screen_.leftTop.y,
		(int)screen_.rightTop.x, (int)screen_.rightTop.y,
		(int)screen_.leftBottom.x, (int)screen_.leftBottom.y,
		(int)screen_.rightBottom.x, (int)screen_.rightBottom.y,
		leftTopX, leftTopY, width, height, textureHandle, WHITE);
}
