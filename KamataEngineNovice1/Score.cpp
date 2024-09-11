#include "Score.h"

void Score::Initialize()
{
	_score = 0;
	_magnification = 0;
	_friendSum = 0;
	_sprite = Novice::LoadTexture("./RS/UI/number42x42.png");
}

void Score::Update(int friendSum)
{
	friendSum = _friendSum;
	if (friendSum <= 5)
		_magnification = 1;
	else if (friendSum <= 10)
		_magnification = 2;
	else if (friendSum <= 15)
		_magnification = 3;
	else if (friendSum <= 20)
		_magnification = 5;
	else
		_magnification = 10;
}

void Score::Draw()
{
	ScoreDraw(scorePos);
	//#ifdef _DEBUG
	//	Novice::ScreenPrintf(10, 10, "Score = %d", _score);
	//	Novice::ScreenPrintf(10, 30, "High Score= %d", _highScore);
	//#endif // _DEBUG
}

void Score::AddScore(Enemy* enemy)
{
	Enemy::Type type = enemy->Get_type();
	int addScore = 0;
	switch (type)
	{
	case Enemy::tSnake:
		addScore = 10;
		break;
	case Enemy::tEagles:
		addScore = 10;
		break;
	case Enemy::tSpider:
		addScore = 10;
		break;
	case Enemy::tBee:
		addScore = 10;
		break;
	}
	if (_friendSum >= _friendMax)
		addScore = 100;

	addScore *= _magnification;
	_score += addScore;
}

void Score::GameOverScore()
{
	_highScore = _score;
}

void Score::ScoreDraw(Vector2 pos)
{
	Vector2 listSize = { 462,42 };			//整张数字图集的大小(中间的图都是正方形的)
	float scoreNextLength = -12;			//两个数字贴图之间的间隔距离
	int digitMax = 7;						//最多显示多少位数
	bool isMax = false;						//是否超过所有可以显示的位数了
	int sprite = _sprite;
	Vector2 mPos = { pos.x + 290,pos.y };	//倍率数字的开始
	float mNextLength = -22;				//倍率数字的间隔距离

	//分数绘画
	// 将每一位数字都拆开并存放
	std::vector<int> scoreDigits;
	int temp = _score;
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

	//倍率绘画
	Novice::DrawSpriteRect((int)(pos.x + 262), (int)pos.y, (int)(listSize.y * 10), 0, (int)listSize.y, (int)listSize.y, sprite, listSize.y / listSize.x, 1, 0, 0xffbd76ff);//画出x的图标
	//拆分数字并储存
	std::vector<int> mDigits;
	temp = _magnification;
	while (temp > 0) {
		int digit = temp % 10;
		mDigits.push_back(digit);
		temp /= 10;
	}
	std::reverse(mDigits.begin(), mDigits.end());
	// 画图
	if (mDigits.size() == 0) {
		Novice::DrawSpriteRect((int)(mPos.x - listSize.y / 2.f), (int)mPos.y, (int)listSize.y * 0, 0, (int)listSize.y, (int)listSize.y, sprite, listSize.y / listSize.x, 1, 0, WHITE);
	}
	else {
		int numDigit = 0;   // 当前显示到第几位
		for (int digit : mDigits) {
			Vector2 startPos = { mPos.x - (mDigits.size() * 0.5f * mNextLength) ,mPos.y };
			Vector2 numPos = { startPos.x + mNextLength * numDigit, mPos.y };
			Novice::DrawSpriteRect((int)numPos.x, (int)numPos.y, (int)(listSize.y * digit), 0, (int)listSize.y, (int)listSize.y, sprite, listSize.y / listSize.x, 1, 0, 0xffbd76ff);
			numDigit++;
		}
	}
}

