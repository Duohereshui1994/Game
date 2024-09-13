#include "ClearStage.h"
#include "Novice.h"


ClearStage::ClearStage()
{
	audioClip_ = new AudioClip();
	cBG_ = new ClearBackground();
	buttonType_ = ClearButtonType::kNull;
}

ClearStage::~ClearStage()
{
	delete cBG_;
	delete audioClip_;
}

void ClearStage::Initialize()
{
	audioClip_ = new AudioClip();
	cBG_->Initialize();
	buttonType_ = ClearButtonType::kNull;

}

void ClearStage::Update(char keys[], char preKeys[])
{
	if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
		finished_ = true;
	}

	cBG_->Update();

	switch (cBG_->GetParam())
	{
		case 0:
			buttonType_ = ClearButtonType::kNull;
			break;
		case 1:
			buttonType_ = ClearButtonType::kRestart;
			break;
		case 2:
			buttonType_ = ClearButtonType::kMenu;
			break;
	}

	if (buttonType_ == ClearButtonType::kRestart || buttonType_ == ClearButtonType::kMenu) {
		if (Novice::IsTriggerMouse(0)) {
			Novice::PlayAudio(audioClip_->audioEffectButton_, false, 1.5f);
			finished_ = true;
		}
	}
}

void ClearStage::Draw()
{
	cBG_->Draw();
}
