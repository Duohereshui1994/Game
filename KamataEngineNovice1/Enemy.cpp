#include "Enemy.h"

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
	_spriteSize = { 96,96 };
	_frameSum = 1;
	_scale = { 1,1 };
	_color = WHITE;
	hpMax_ = 1;
	attack_ = 1;
	_speed = 3;

	_isDead = false;
	_isCollison = false;

	switch (_type)
	{
	case Enemy::tSnake:
		_sprite = EnemyManager::_spSnake;
		_spriteSize = { 96,96 };
		_frameSum = 8;
		break;
	case Enemy::tEagles:
		break;
	case Enemy::tSpider:
		break;
	case Enemy::tBee:
		hpMax_ = 2;
		break;
	default:
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
}
