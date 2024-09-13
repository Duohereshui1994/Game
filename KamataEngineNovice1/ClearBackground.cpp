#include "ClearBackground.h"
#include "Novice.h"

ClearBackground::ClearBackground()
{
	textureClearBackground_ = Novice::LoadTexture("./RS/ClearStage/result.png");

	mousePosX = 0;
	mousePosY = 0;

	param_ = 0;
}

ClearBackground::~ClearBackground()
{
	delete math_;
}

void ClearBackground::Initialize()
{
	param_ = 0;
}

void ClearBackground::Update()
{
	Novice::GetMousePosition(&mousePosX, &mousePosY);

	if (mousePosX > 981 && mousePosX < 1187 && mousePosY>519 && mousePosY < 568) {
		param_ = 1;
	}
	else if (mousePosX > 981 && mousePosX < 1187 && mousePosY>580 && mousePosY < 629) {
		param_ = 2;
	}
	else {
		param_ = 0;
	}
}

void ClearBackground::Draw()
{
	Novice::DrawSpriteRect(0, 0, (int)param_ * 1280, 0, 1280, 720, textureClearBackground_, 1 / 3.0f, 1, 0.0, WHITE);
}
