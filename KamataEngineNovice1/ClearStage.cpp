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
	DrawScore(_scorePos, Score::GetScore());
	DrawScore(_scoreHighPos, Score::GetHighScore());
	//DrawScore(_scorePos, 99999);
	//DrawScore(_scoreHighPos, 99999);
}

void ClearStage::DrawScore(Vector2 pos, int score)
{
	Vector2 listSize = { 462,42 };			//整张数字图集的大小(中间的图都是正方形的)
	float scoreNextLength = -12;			//两个数字贴图之间的间隔距离
	int digitMax = 7;						//最多显示多少位数
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
	isMax = digitMax <= scoreDigits.size() ? true : false;

	// 画图
	if (scoreDigits.size() == 0) {
		Novice::DrawSpriteRect((int)pos.x, (int)pos.y, (int)listSize.y * 0, 0, (int)listSize.y, (int)listSize.y, sprite, listSize.y / listSize.x, 1, 0, WHITE);
	}
	else if (isMax) {
		for (int i = 0; i < digitMax + 1; i++) {
			Vector2 numPos = { pos.x + i * (listSize.y + scoreNextLength), pos.y };
			Novice::DrawSpriteRect((int)numPos.x, (int)numPos.y, (int)(listSize.y * 9), 0, (int)listSize.y, (int)listSize.y, sprite, listSize.y / listSize.x, 1, 0, RED);
		}
	}
	else {
		int numDigit = 0;   // 当前显示到第几位
		if (!isMax) {
			for (int digit : scoreDigits) {
				Vector2 numPos = { pos.x + numDigit * (listSize.y + scoreNextLength), pos.y };
				Novice::DrawSpriteRect((int)numPos.x, (int)numPos.y, (int)(listSize.y * digit), 0, (int)listSize.y, (int)listSize.y, sprite, listSize.y / listSize.x, 1, 0, WHITE);
				numDigit++;
			}
		}
	}
}
