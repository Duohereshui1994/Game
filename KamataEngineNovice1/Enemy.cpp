#include "Enemy.h"

void Enemy::Move()
{
	Vector2 dir = (_targetPos - _pos).Normalize();
	float length = (_targetPos - _pos).Length();
	if (length > 5)
		_pos += dir * _speed;
	else
		EnemyManager::ReleaseEnemy(this);
}

Enemy::Enemy()
{
	frameNum_ = 0;
	deltaTime_ = 1.0f / 60.0f;
	// 初始化 MathFunc 对象
	math_ = new MathFunc();
	// 初始化 Mono 结构体成员
	obj_.position = _pos;
	obj_.width = _size.x;
	obj_.height = _size.y;
	obj_.center = Vector2(obj_.position.x + obj_.width / 2.0f, obj_.position.y + obj_.height / 2.0f);
	obj_.scale = _scale;
	obj_.rotate = _rotate;

	tools = new EnemyTools();
}

Enemy::~Enemy()
{
	delete math_;
	delete tools;
}

void Enemy::Initialize(Vector2 pos, float rotate, Type type)
{
	_pos = pos;
	_rotate = rotate;
	_type = type;
	_size = { 96,96 };
	_spriteSize = { 128,128 };
	_frameSum = 1;
	_scale = { 1,1 };
	_color = WHITE;
	hpMax_ = 1;
	attack_ = 1;
	_speed = 3.f;

	_isDead = false;
	_isCollison = false;

	switch (_type)
	{
	case Enemy::tSnake:
		_sprite = EnemyManager::_spSnake;
		_spriteSize = { 128,128 };
		_frameSum = 8;
		break;
	case Enemy::tEagles:
		_sprite = EnemyManager::_spEagles;
		_spriteSize = { 1152 / 6.f,128 };
		_frameSum = 6;
		break;
	case Enemy::tSpider:
		_sprite = EnemyManager::_spPlayer;
		_spriteSize = { 96,96 };
		_frameSum = 4;
		break;
	case Enemy::tBee:
		_sprite = EnemyManager::_spPlayer;
		_spriteSize = { 96,96 };
		_frameSum = 4;
		hpMax_ = 2;
		break;
	case Enemy::tPlayer:
		_sprite = EnemyManager::_spPlayer;
		_spriteSize = { 96,96 };
		_frameSum = 4;
		break;
	}

	hp_ = hpMax_;
}

void Enemy::Update(char keys[], char preKeys[])
{
	keys; preKeys;
	if (keys[DIK_Q]) {
		_rotate -= 0.1f;
	}
	if (keys[DIK_E]) {
		_rotate += 0.1f;
	}
	if (keys[DIK_W]) {
		_pos.y -= _speed;
	}
	if (keys[DIK_S]) {
		_pos.y += _speed;
	}
	if (keys[DIK_A]) {
		_pos.x -= _speed;
	}
	if (keys[DIK_D]) {
		_pos.x += _speed;
	}

	Move();//移动

	//传入数据
	obj_.position = _pos;
	obj_.width = _size.x;
	obj_.height = _size.y;
	obj_.center = Vector2(obj_.position.x + obj_.width / 2.0f, obj_.position.y + obj_.height / 2.0f);
	obj_.scale = _scale;
	obj_.rotate = _rotate;
}

void Enemy::Draw()
{
	tools->FrameAnimation(0, 10, _frameSum, _spriteSize, _sprite, _pos, _rotate, _scale, _color);
}

void Enemy::PushUpdate()
{
	EnemyManager::_updatePool.push_back(this);
}

void EnemyTools::FrameAnimation(int index, int frameTime, int frameSum, Vector2 frameSize, int sprite, Vector2 pos, float rotate, Vector2 scale, int color)
{
	if (FrameTimeWatch_ani(frameTime, index, true))
		_currentFrameIndex[index]++;
	if (_currentFrameIndex[index] > frameSum - 1 || _currentFrameIndex[index] < 0)
		_currentFrameIndex[index] = 0;
	int listX = int(_currentFrameIndex[index] * frameSize.x);
	int listY = 0;
	int listW = int(frameSum * frameSize.x);
	int listH = int(frameSize.y);
	Vector2 zoom = { frameSize.x / 2.f - frameSize.x / 2.f * scale.x,frameSize.y / 2.f + frameSize.y / 2.f * scale.y };
	Vector2 rotatedPos = { -frameSize.x / 2.f + zoom.x ,frameSize.y / 2.f - zoom.y };
	rotatedPos = AditionRule(rotatedPos, rotate);
	rotatedPos = { rotatedPos.x + pos.x ,rotatedPos.y + pos.y };
	Novice::DrawSpriteRect((int)(rotatedPos.x), (int)(rotatedPos.y), listX, listY, int(frameSize.x), int(frameSize.y), sprite, frameSize.x / listW * scale.x, frameSize.y / listH * scale.y, rotate, color);
}

bool EnemyTools::FrameTimeWatch_ani(int frame, int index, bool first)
{
	if (!first) {
		if (_currentFrame_ani[index] > frame) {
			_currentFrame_ani[index] = 0;
			return true;
		}
		_currentFrame_ani[index]++;
	}
	else {
		if (_currentFrame_ani[index] <= 0) {
			_currentFrame_ani[index] = frame;
			return true;
		}
		_currentFrame_ani[index]--;
	}
	return false;
}

Vector2 EnemyTools::AditionRule(Vector2 pos, float rad)
{
	float aditionX = pos.x * cosf(rad) - pos.y * sinf(rad);
	float aditionY = pos.y * cosf(rad) + pos.x * sinf(rad);
	return Vector2{ aditionX,aditionY };
}

bool EnemyTools::FrameTimeWatch(int frame, int index, bool first)
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

void EnemyManager::Update(char keys[], char preKeys[])
{
	for (Enemy* it : _updatePool) {
		it->Update(keys, preKeys);
	}
}

Enemy* EnemyManager::AcquireEnemy(Vector2 pos, float rotate, Enemy::Type type)
{
	if (_idlePool.empty()) {
		Enemy* enemy = new Enemy();
		enemy->Initialize(pos, rotate, type);
		return enemy;
	}
	else {
		Enemy* enemy = _idlePool.front();
		_idlePool.pop();
		enemy->Initialize(pos, rotate, type);
		return enemy;
	}
}

void EnemyManager::Draw()
{
	for (Enemy* it : _updatePool) {
		it->Draw();
	}
}

bool EnemyManager::FrameTimeWatch(int frame, int index, bool first)
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

void EnemyManager::BornEnemy(int score, int friendSum)
{
	score, friendSum;
	std::random_device rd;
	std::mt19937 gen(rd());

	//随机选择路线并填入敌人
	if (FrameTimeWatch(_lineTime, 0, true)) {
		assert(!_idlePool.empty());//请不要让敌人的闲置对象池为空
		//往哪个路线填入敌人
		int lineNum = 0;
		std::uniform_int_distribution<int> dis_line(0, _linesSum - 1);
		lineNum = dis_line(gen);
		//根据路线，随机出本次要出现的敌人
		Enemy::Type enemyType = Enemy::tSnake;
		if (lineNum == 0 || lineNum == _linesSum - 1) {
			std::uniform_int_distribution<int> dis_enemyIndex(0, 1);
			switch (dis_enemyIndex(gen)) {
			case 0:enemyType = Enemy::tSnake; break;
			case 1:enemyType = Enemy::tSpider; break;
			}
		}
		else {
			std::uniform_int_distribution<int> dis_enemyIndex(0, 1);
			switch (dis_enemyIndex(gen)) {
			case 0:enemyType = Enemy::tEagles; break;
			case 1:enemyType = Enemy::tBee; break;
			}
		}
		//往路线中填入多少个敌人
		std::uniform_int_distribution<int> dis_enemySum(0, _eachBornMax);
		int enemySum = dis_enemySum(gen);
		for (int i = 0; i < enemySum; i++) {
			Vector2 targetPos = _idlePool.front()->Get_targetPos();
			Vector2 bornPos = targetPos + _bornPosOffset[lineNum];
			Enemy* it = AcquireEnemy(bornPos, 0, enemyType);
			_enemyLines[lineNum].push(it);
		}
	}

	//将路线中储存的敌人释放出来
	if (FrameTimeWatch(_bornEnemyTime, 1, true)) {
		for (int i = 0; i < _linesSum; i++) {
			if (!_enemyLines[i].empty()) {
				Enemy* it = _enemyLines[i].top();
				it->PushUpdate();
				_enemyLines[i].pop();
			}
		}
	}
}

void EnemyManager::ClearEnemyLines()
{
	for (int i = 0; i < _linesSum; i++) {
		while (!_enemyLines[i].empty())
		{
			Enemy* it = _enemyLines[i].top();
			ReleaseEnemy(it);
			_enemyLines[i].pop();
		}
	}
}

void EnemyManager::ReleaseEnemy(Enemy* enemy)
{
	auto it = std::find(_updatePool.begin(), _updatePool.end(), enemy);
	if (it != _updatePool.end()) {
		std::swap(*it, _updatePool.back());
		_updatePool.pop_back();
		_idlePool.push(enemy);
	}
}

void EnemyManager::LoadRes()
{
	_spSnake = Novice::LoadTexture("./RS/Enemy/snake_walk.png");
	_spEagles = Novice::LoadTexture("./RS/Enemy/hawk_walk.png");
	_spPlayer = Novice::LoadTexture("./RS/Enemy/player_Up_Idle_Left.png");
}

