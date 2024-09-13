#define MAX_TBG_FRAME 4
#define TBG_TIME_SCALE 1.0f
#include "TitleBackground.h"
#include "Novice.h"

TitleBackground::TitleBackground()
{
	textureTitleBackground_ = Novice::LoadTexture("./RS/Title/title.png");
	textureHelpBackground_ = Novice::LoadTexture("./RS/Title/help.png");

	mousePosX = 0;
	mousePosY = 0;

	param_ = 0;

}

TitleBackground::~TitleBackground()
{
	delete math_;
}

void TitleBackground::Initialize()
{
	param_ = 0;
}

void TitleBackground::Update()
{
	Novice::GetMousePosition(&mousePosX, &mousePosY);


	if (mousePosX > 143 && mousePosX < 427 && mousePosY>580 && mousePosY < 648) {
		param_ = 1;
	}
	else if (mousePosX > 497 && mousePosX < 781 && mousePosY>580 && mousePosY < 648) {
		param_ = 2;
	}
	else if (mousePosX > 845 && mousePosX < 1126 && mousePosY>580 && mousePosY < 648) {
		param_ = 3;
	}
	else {
		param_ = 0;
	}


}

void TitleBackground::Draw()
{
	Novice::DrawSpriteRect(0, 0, (int)param_ * 1280, 0, 1280, 720, textureTitleBackground_, 1 / 4.0f, 1, 0.0, WHITE);
}

void TitleBackground::DrawHelp()
{
	Novice::DrawSprite(0, 0, textureHelpBackground_, 1, 1, 0.0f, WHITE);
}

void TitleBackground::SetParam(int param)
{
	param_ = param;
}
