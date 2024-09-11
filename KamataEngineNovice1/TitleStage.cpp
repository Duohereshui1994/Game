#include "TitleStage.h"
#include "Novice.h"

TitleStage::TitleStage()
{
	tBG_ = new TitleBackground();
	tBG_->Initialize();
}

TitleStage::~TitleStage()
{
	delete tBG_;
}

void TitleStage::Initialize()
{
	tBG_->Initialize();
}

void TitleStage::Update(char keys[], char preKeys[])
{
	if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
		finished_ = true;
	}
	tBG_->Update();
}

void TitleStage::Draw()
{
	tBG_->Draw();
}
