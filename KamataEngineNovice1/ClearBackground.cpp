#include "ClearBackground.h"
#include "Novice.h"

ClearBackground::ClearBackground()
{
	textureClearBackground_ = Novice::LoadTexture("./RS/ClearStage/result.png");

	param_ = 1;
}

ClearBackground::~ClearBackground()
{
	delete math_;
}

void ClearBackground::Initialize()
{
	param_ = 1;
}

void ClearBackground::Update(char keys[], char preKeys[])
{
	if (keys[DIK_UP] && !preKeys[DIK_UP]) {
		if (param_ <= 1) {
			param_ = 1;
		}
		else {
			param_--;
		}
	}
	else if (keys[DIK_DOWN] && !preKeys[DIK_DOWN]) {
		if (param_ >= 2) {
			param_ = 2;
		}
		else {
			param_++;
		}
	}
}

void ClearBackground::Draw()
{
	Novice::DrawSpriteRect(0, 0, (int)param_ * 1280, 0, 1280, 720, textureClearBackground_, 1 / 3.0f, 1, 0.0, WHITE);
}
