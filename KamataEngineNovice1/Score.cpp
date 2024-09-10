#include "Score.h"

void Score::Initialize()
{
	_score = 0;
	_combo = 0;
	_friendSum = 0;
}

void Score::Update(int friendSum)
{
	//_combo = friendSum;
	friendSum = _friendSum;
	if (friendSum <= 5)
		_combo = 1;
	else if (friendSum <= 10)
		_combo = 2;
	else if (friendSum <= 15)
		_combo = 3;
	else if (friendSum <= 20)
		_combo = 5;
}

void Score::Draw()
{
#ifdef _DEBUG
	Novice::ScreenPrintf(10, 10, "Score = %d", _score);
	Novice::ScreenPrintf(10, 30, "High Score= %d", _highScore);
#endif // _DEBUG
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

	addScore *= _combo;
	_score += addScore;
}

void Score::GameOverScore()
{
	_highScore = _score;
}
