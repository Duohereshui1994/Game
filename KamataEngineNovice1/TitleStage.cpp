#include "TitleStage.h"
#include "Novice.h"


TitleStage::TitleStage()
{
	buttonType_ = TitleButtonType::kNull;
	tBG_ = new TitleBackground();
	tBG_->Initialize();
}

TitleStage::~TitleStage()
{
	delete tBG_;
	delete audioClip_;
}

void TitleStage::Initialize()
{
	finished_ = false;
	isHelpOn_ = false;
	buttonType_ = TitleButtonType::kNull;
	tBG_->Initialize();
	audioClip_ = new AudioClip();

}

void TitleStage::Update()
{
	tBG_->Update();

	if (isHelpOn_ == false) {

		switch (tBG_->GetParam())
		{
			case 0:
				buttonType_ = TitleButtonType::kNull;
				break;
			case 1:
				buttonType_ = TitleButtonType::kHelp;
				break;
			case 2:
				buttonType_ = TitleButtonType::kStart;
				break;
			case 3:
				buttonType_ = TitleButtonType::kExit;
				break;
		}

		if (Novice::IsTriggerMouse(0)) {
			if (buttonType_ == TitleButtonType::kStart || buttonType_ == TitleButtonType::kExit) {
				Novice::PlayAudio(audioClip_->audioEffectButton_, false, 1.5f);
				finished_ = true;
			}
			else if (buttonType_ == TitleButtonType::kHelp) {
				Novice::PlayAudio(audioClip_->audioEffectButton_, false, 1.5f);
				isHelpOn_ = true;
			}
		}
	}
	else {
		if (Novice::IsTriggerMouse(1)) {
			Novice::PlayAudio(audioClip_->audioEffectButton_, false, 1.5f);
			isHelpOn_ = false;
			tBG_->SetParam(0);
		}
	}






}

void TitleStage::Draw()
{
	tBG_->Draw();

	if (isHelpOn_ == true) {
		tBG_->DrawHelp();
	}

}

