#define MAX_TBG_FRAME 4
#define TBG_TIME_SCALE 1.0f
#include "TitleBackground.h"
#include "Novice.h"

TitleBackground::TitleBackground()
{
	textureTitleBackground_ = Novice::LoadTexture("./RS/Title/title.png");

	frameNum_ = 0;
	deltaTime_ = 1.0f / 60.0f;
}

TitleBackground::~TitleBackground()
{
	delete math_;
}

void TitleBackground::Initialize()
{
	frameNum_ = 0;
	deltaTime_ = 1.0f / 60.0f;
}

void TitleBackground::Update()
{
	//背景帧数变化
	if (frameNum_ > MAX_TBG_FRAME - 1) {
		frameNum_ = 0;
	}
	else {
		frameNum_ += deltaTime_ * TBG_TIME_SCALE;
	}
}

void TitleBackground::Draw()
{
	Novice::DrawSpriteRect(0, 0, (int)frameNum_ * 1280, 0, 1280, 720, textureTitleBackground_, 1 / 4.0f, 1, 0.0, WHITE);
}
