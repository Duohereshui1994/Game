#include "ClearStage.h"
#include "Novice.h"


ClearStage::ClearStage()
{
	cBG_ = new ClearBackground();
}

ClearStage::~ClearStage()
{
	delete cBG_;
}

void ClearStage::Initialize()
{
	cBG_->Initialize();
}

void ClearStage::Update(char keys[], char preKeys[])
{
	if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
		finished_ = true;
	}
	cBG_->Update(keys, preKeys);
}

void ClearStage::Draw()
{
	cBG_->Draw();
}
