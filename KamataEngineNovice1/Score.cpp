#include "Score.h"

void Score::Initialize()
{
	_score = 0;
	_magnification = 1;
	_combo = 0;
	_sprite = Novice::LoadTexture("./RS/UI/number42x42.png");
	_spCombo = Novice::LoadTexture("./RS/UI/combo.png");
	_spComboNums = Novice::LoadTexture("./RS/UI/comboNums.png");

	_comboShakeOffset = { 0,0 };
	_comboPosOffset = _comboPos;
	_isComboShake = false;
	_comboColor = WHITE;
}

void Score::Update()
{
	//10个Combo等于1倍率
	_magnification = int(_combo / 10);
	if (_magnification < 1)
		_magnification = 1;
}

void Score::Draw()
{
	DrawScore(scorePos);
	DrawCombo();
	ComboShake();
}

void Score::AddScore(Enemy* enemy, bool longKill)
{
	Enemy::Type type = enemy->Get_type();
	int addScore = 0;
	switch (type)
	{
	case Enemy::tSnake:
	case Enemy::tEagles:
	case Enemy::tSpider:
	case Enemy::tBee:
		addScore = 5;
		break;
	case Enemy::tPlayer:
		addScore -= 100;
		break;
	}
	if (longKill && type != Enemy::tPlayer)
		addScore = 10;

	if (type != Enemy::tPlayer)
		addScore *= _magnification;
	_score += addScore;
	if (_score < 0)
		_score = 0;
}

void Score::GameOverScore()
{
	if (_score > _highScore)
		_highScore = _score;
}

void Score::RefreshMagnification()
{
	_magnification = _combo;
	if (_magnification < 1)
		_magnification = 1;
	else if (_magnification > 99)
		_magnification = 99;
	_combo = 0;
}

void Score::AddCombo(Enemy* enemy)
{
	switch (enemy->Get_type())
	{
	case Enemy::tPlayer:
		_combo += 3;
		break;
	default:
		_combo += 1;
		break;
	}
}

void Score::DrawScore(Vector2 pos)
{
	Vector2 listSize = { 462,42 };			//整张数字图集的大小(中间的图都是正方形的)
	float scoreNextLength = -12;			//两个数字贴图之间的间隔距离
	int digitMax = 8;						//最多显示多少位数
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
	isMax = _score >= 99999999 ? true : false;

	// 画图
	if (scoreDigits.size() == 0) {
		Novice::DrawSpriteRect((int)pos.x, (int)pos.y, (int)listSize.y * 0, 0, (int)listSize.y, (int)listSize.y, sprite, listSize.y / listSize.x, 1, 0, WHITE);
	}
	else if (isMax) {
		for (int i = 0; i < digitMax; i++) {
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
	//std::reverse(mDigits.begin(), mDigits.end());
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

void Score::DrawCombo()
{
	Vector2 listSize = { 360,50 };								//整张数字图集的大小
	Vector2 spriteSize = { 36,50 };								//数字图的大小
	float scoreNextLength = 0;									//两个数字贴图之间的间隔距离
	Vector2 numPos = { _comboPos.x + 110,_comboPos.y - 14 };	//数字的开始位置

	//画出标记
	Novice::DrawSprite((int)_comboPos.x, (int)_comboPos.y, _spCombo, 1, 1, 0, _comboColor);
	//拆分数字并储存
	std::vector<int> digits;
	int temp = _combo;
	while (temp > 0) {
		int digit = temp % 10;
		digits.push_back(digit);
		temp /= 10;
	}
	std::reverse(digits.begin(), digits.end());
	// 画图
	if (digits.size() == 0) {
		Novice::DrawSpriteRect((int)numPos.x, (int)numPos.y, 0, 0, (int)(spriteSize.x), (int)spriteSize.y, _spComboNums, spriteSize.x / listSize.x, 1, 0, _comboColor);
	}
	else {
		int numDigit = 0;   // 当前显示到第几位
		for (int digit : digits) {
			Vector2 drawPos = { numPos.x + numDigit * (spriteSize.x + scoreNextLength), numPos.y };
			Novice::DrawSpriteRect((int)drawPos.x, (int)drawPos.y, (int)(spriteSize.x * digit), 0, (int)spriteSize.x, (int)spriteSize.y, _spComboNums, spriteSize.x / listSize.x, 1, 0, _comboColor);
			numDigit++;
		}
	}
}

void Score::ComboShake()
{
	if (_isComboShake) {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dis_moveX(-_randComboShake, _randComboShake);
		std::uniform_real_distribution<float> dis_moveY(-_randComboShake, _randComboShake);
		_comboShakeOffset = Vector2{ dis_moveX(rd),dis_moveY(rd) };
		_comboPos = _comboPosOffset + _comboShakeOffset;
		_comboColor = RED;
		if (FrameTimeWatch(30, 0, false))
		{
			_comboShakeOffset = { 0,0 };
			_comboColor = WHITE;
			_isComboShake = false;
		}
	}
}

bool Score::FrameTimeWatch(int frame, int index, bool first)
{
	if (!first) {
		if (_currentTimes[index] > frame) {
			_currentTimes[index] = 0;
			return true;
		}
		_currentTimes[index]++;
	}
	else {
		if (_currentTimes[index] <= 0) {
			_currentTimes[index] = frame;
			return true;
		}
		_currentTimes[index]--;
	}
	return false;
}

