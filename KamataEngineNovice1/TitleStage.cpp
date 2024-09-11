#include "TitleStage.h"

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

void TitleStage::Update()
{
	tBG_->Update();
}

void TitleStage::Draw()
{
	tBG_->Draw();
}
