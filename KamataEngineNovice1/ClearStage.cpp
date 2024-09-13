#include "ClearStage.h"
#include "Novice.h"


ClearStage::ClearStage()
{
	audioClip_ = new AudioClip();
	cBG_ = new ClearBackground();
	buttonType_ = ClearButtonType::kNull;
	_spListNum = Novice::LoadTexture("./RS/UI/number42x42.png");
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
	DrawScore(_scorePos, Score::GetScore(), WHITE);
	DrawScore(_scoreHighPos, Score::GetHighScore(), 0xffeb3bff);
	//DrawScore(_scorePos, 12345, WHITE);
	//DrawScore(_scoreHighPos, 123456789, 0xffeb3bff);
}

void ClearStage::DrawScore(Vector2 pos, int score, int color)
{
	Vector2 listSize = { 462,42 };			//整张数字图集的大小(中间的图都是正方形的)
	Vector2 spriteSize = { 42,42 };
	float scoreNextLength = -12;			//两个数字贴图之间的间隔距离
	int digitMax = 8;						//最多显示多少位数
	bool isMax = false;						//是否超过所有可以显示的位数了
	int sprite = _spListNum;

	//分数绘画
	// 将每一位数字都拆开并存放
	std::vector<int> scoreDigits;
	int temp = score;
	while (temp > 0) {
		int digit = temp % 10;
		scoreDigits.push_back(digit);
		temp /= 10;
	}
	std::reverse(scoreDigits.begin(), scoreDigits.end()); // 翻转要素，让它正确显示数字顺序
	isMax = score >= 99999999 ? true : false;

	// 画图
	if (scoreDigits.size() == 0) {
		Novice::DrawSpriteRect((int)(pos.x - spriteSize.x / 2.f), (int)(pos.y - spriteSize.y / 2.f), 0, 0, (int)spriteSize.x, (int)spriteSize.y, sprite, spriteSize.x / listSize.x, 1, 0, color);
	}
	else if (isMax) {
		for (int i = 0; i < digitMax; i++) {
			Vector2 startPos = { pos.x - (scoreDigits.size() * 0.5f * (spriteSize.x + scoreNextLength)),pos.y };
			Vector2 numPos = { startPos.x + (spriteSize.x + scoreNextLength) * i, pos.y };
			Novice::DrawSpriteRect((int)numPos.x, (int)numPos.y, (int)(listSize.y * 9), 0, (int)listSize.y, (int)listSize.y, sprite, spriteSize.x / listSize.x, 1, 0, RED);
		}
	}
	else {
		int numDigit = 0;   // 当前显示到第几位
		if (!isMax) {
			for (int digit : scoreDigits) {
				Vector2 startPos = { pos.x - (scoreDigits.size() * 0.5f * (spriteSize.x + scoreNextLength)),pos.y };
				Vector2 numPos = { startPos.x + (spriteSize.x + scoreNextLength) * numDigit, pos.y };
				Novice::DrawSpriteRect((int)numPos.x, (int)numPos.y, (int)(listSize.y * digit), 0, (int)listSize.y, (int)listSize.y, sprite, listSize.y / listSize.x, 1, 0, color);
				numDigit++;
			}
		}
	}
}
