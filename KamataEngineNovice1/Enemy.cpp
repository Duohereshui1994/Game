#include "Enemy.h"

void Enemy::Move()
{
	Vector2 dir = (_targetPos - _pos).Normalize();
	//左右翻转
	if (dir.x > 0 && _scale.y > 0)
		_scale.y *= -1;
	//移动
	float length = (_targetPos - _pos).Length();
	if (length > 5)
		_pos += dir * _speed;
	else
		EnemyManager::ReleaseEnemy(this);
	//旋转
	_rotate = atan2(dir.y, dir.x) + acosf(-1);
}

void Enemy::Control(char keys[], char preKeys[])
{
	keys, preKeys;
	//旋转
	if (keys[DIK_U]) {
		_rotate -= 0.1f;
	}
	else if (keys[DIK_O]) {
		_rotate += 0.1f;
	}
	//尺寸
	if (keys[DIK_J]) {
		_scale.x += 0.1f;
	}
	else if (keys[DIK_L]) {
		_scale.x -= 0.1f;
	}
	if (keys[DIK_I]) {
		_scale.y += 0.1f;
	}
	else if (keys[DIK_K]) {
		_scale.y -= 0.1f;
	}
	//移动
	if (keys[DIK_W]) {
		_pos.y += _speed;
	}
	else if (keys[DIK_S]) {
		_pos.y -= _speed;
	}
	if (keys[DIK_A]) {
		_pos.x -= _speed;
	}
	else if (keys[DIK_D]) {
		_pos.x += _speed;
	}
}

void Enemy::PlayerSadWait()
{
	_rotate = 0;
	_scale = { 1,1 };
	_pos = { _pos.x,260 };
	_sprite = EnemyManager::_spPlayer_sad;
	_frameSum = 2;
	_spriteSize = { 192.f / _frameSum,96 };
	//在等待途中碰到别的敌人，就会被吃掉
	for (Enemy* it : EnemyManager::_updatePool) {
		if (it->_type != tPlayer) {
			float length = (it->GetTranslate() - _pos).Length();
			if (length < it->GetRadian() + _size.x) {
				ParticleManager::ADD_Particle(_camera, _pos, Emitter::friendDead);
				_isDead = true;
			}
		}
	}
}

void Enemy::ToDead()
{
	int aniTime = 30;
	float t = tools->_currentTimes[0] / (float)aniTime;
	//颜色
	alphaValue = int((tools->EaseOutQuint(t)) * 255);
	if (alphaValue < 0)
		alphaValue = 0;
	_color = 0xFF000000 | alphaValue << 0;//透明度を制御する
	//旋转
	if (_scale.y > 0)
		_rotate -= (1 - tools->EaseOutQuint(t)) * 0.3f;
	else
		_rotate += (1 - tools->EaseOutQuint(t)) * 0.3f;
	//尺寸
	_scale.x = std::lerp(_scale.x, 0.f, tools->EaseInCirc(t));
	_scale.y = std::lerp(_scale.y, 0.f, tools->EaseInCirc(t));
	//位移
	Vector2 dir = (_targetPos - _pos).Normalize();
	_pos += dir * -1 * 2 * (1 - tools->EaseOutQuint(t));
	if (tools->FrameTimeWatch(aniTime, 0, false))
		EnemyManager::ReleaseEnemy(this);
	//图片
	switch (_type)
	{
	case Enemy::tSnake:
		_sprite = EnemyManager::_spSnake_hurt;
		break;
	case Enemy::tEagles:
		_sprite = EnemyManager::_spEagles_hurt;
		break;
	case Enemy::tSpider:
		_sprite = EnemyManager::_spSpider_hurt;
		break;
	case Enemy::tBee:
		break;
	case Enemy::tPlayer:
		break;
	}

	//传入数据
	_affine = { _scale,_rotate,_pos };
	Matrix3x3 worldMatrix_ = math_->MakeAffine(_affine);
	Matrix3x3 wvpVpMatrix_ = math_->WvpVpMatrix(worldMatrix_, _camera->GetVpVpMatrix());
	Vertex local = {
		{ -_spriteSize.x / 2.0f, +_spriteSize.y / 2.0f},
		{ +_spriteSize.x / 2.0f, +_spriteSize.y / 2.0f},
		{ -_spriteSize.x / 2.0f, -_spriteSize.y / 2.0f},
		{ +_spriteSize.x / 2.0f, -_spriteSize.y / 2.0f},
	};
	_screen = math_->TransformSprite(local, wvpVpMatrix_);
}

void Enemy::ToGetPlayer()
{
	int aniTime = 30;
	float t = tools->_currentTimes[0] / (float)aniTime;
	//颜色
	alphaValue = int((tools->EaseOutQuint(1 - t)) * 255);
	if (alphaValue < 0)
		alphaValue = 0;
	_color = 0xFFFFFF00 | alphaValue << 0;//透明度を制御する
	//尺寸
	_scale.x = std::lerp(1.f, 0.f, tools->EaseInCirc(t));
	_scale.y = std::lerp(_scale.y > 0 ? 1.f : -1.f, 0.f, tools->EaseInCirc(t));
	//位移
	Vector2 dir = (_targetPos + Vector2{ 0,-100 } - _pos).Normalize();
	float length = (_targetPos + Vector2{ 0,-100 } - _pos).Length();
	_pos += dir * _speed * 2;
	//旋转
	if (length > 5)
		_rotate = atan2(dir.y, dir.x) + acosf(-1);
	if (tools->FrameTimeWatch(aniTime, 0, false))
		EnemyManager::ReleaseEnemy(this);

	//传入数据
	_affine = { _scale,_rotate,_pos };
	Matrix3x3 worldMatrix_ = math_->MakeAffine(_affine);
	Matrix3x3 wvpVpMatrix_ = math_->WvpVpMatrix(worldMatrix_, _camera->GetVpVpMatrix());
	Vertex local = {
		{ -_spriteSize.x / 2.0f, +_spriteSize.y / 2.0f},
		{ +_spriteSize.x / 2.0f, +_spriteSize.y / 2.0f},
		{ -_spriteSize.x / 2.0f, -_spriteSize.y / 2.0f},
		{ +_spriteSize.x / 2.0f, -_spriteSize.y / 2.0f},
	};
	_screen = math_->TransformSprite(local, wvpVpMatrix_);
}

Enemy::Enemy()
{
	frameNum_ = 0;
	deltaTime_ = 1.0f / 60.0f;
	_affine = { _scale,_rotate,_pos };
	_screen = {};
	math_ = new MathFunc();			// 初始化 MathFunc 对象
	tools = new EnemyTools();		//初始化工具合集
}

Enemy::~Enemy()
{
	delete math_;
	delete tools;
	delete _camera;
}

void Enemy::Initialize(Camera* camera, Vector2 pos, Type type, Vector2 targetPos)
{
	_camera = camera; _pos = pos; _rotate = 0; _scale = { 1,1 };
	_size = { 96,96 }; _spriteSize = { 128,128 }; _frameSum = 1;
	_type = type; _color = WHITE; alphaValue = 255;
	hpMax_ = 1; attack_ = 1; _speed = 3;
	_targetPos = targetPos;

	_isDead = false; _isGetPlayer = false; _isFriendWiat = false;

	switch (_type)
	{
	case Enemy::tSnake:
		_sprite = EnemyManager::_spSnake;
		_frameSum = 8;
		_spriteSize = { 1024.f / _frameSum,128 };
		break;
	case Enemy::tEagles:
		_sprite = EnemyManager::_spEagles;
		_frameSum = 6;
		_spriteSize = { 1152.f / _frameSum,128 };
		break;
	case Enemy::tSpider:
		_sprite = EnemyManager::_spSpider;
		_frameSum = 6;
		_spriteSize = { 1152.f / _frameSum,128 };
		break;
	case Enemy::tBee:
		_sprite = EnemyManager::_spBee;
		_frameSum = 4;
		_spriteSize = { 512.f / _frameSum,128 };
		break;
	case Enemy::tPlayer:
		_sprite = EnemyManager::_spPlayer_walk;
		_frameSum = 4;
		_spriteSize = { 384.f / _frameSum,96 };
		_size = { 74,74 };
		break;
	}

	hp_ = hpMax_;
	_affine = { _scale,_rotate,_pos };
}

void Enemy::Update(char keys[], char preKeys[])
{
	keys; preKeys;
	//Control(keys, preKeys);

	if (!_isFriendWiat)
		Move();//移动
	else
		PlayerSadWait();

	//传入数据
	_affine = { _scale,_rotate,_pos };
	Matrix3x3 worldMatrix_ = math_->MakeAffine(_affine);
	Matrix3x3 wvpVpMatrix_ = math_->WvpVpMatrix(worldMatrix_, _camera->GetVpVpMatrix());
	Vertex local = {
		{ -_spriteSize.x / 2.0f, +_spriteSize.y / 2.0f},
		{ +_spriteSize.x / 2.0f, +_spriteSize.y / 2.0f},
		{ -_spriteSize.x / 2.0f, -_spriteSize.y / 2.0f},
		{ +_spriteSize.x / 2.0f, -_spriteSize.y / 2.0f},
	};
	_screen = math_->TransformSprite(local, wvpVpMatrix_);
}

void Enemy::Draw()
{
	//tools->FrameAnimation(0, 10, _frameSum, _spriteSize, _sprite, _pos, _rotate, _scale, _color);
	tools->FrameAnimation(0, 10, _frameSum, _spriteSize, _sprite, _screen, _color);
}

void Enemy::PushUpdate()
{
	EnemyManager::_updatePool.push_back(this);
}

void EnemyTools::FrameAnimation(int index, int frameTime, int frameSum, Vector2 frameSize, int sprite, Vertex screen, int color)
{
	//帧动画的帧计算
	if (FrameTimeWatch_ani(frameTime, index, false))
		_currentFrameIndex[index]++;
	if (_currentFrameIndex[index] > frameSum - 1 || _currentFrameIndex[index] < 0)
		_currentFrameIndex[index] = 0;
	//绘图(通过Camera计算的坐标绘图
	Novice::DrawQuad(
		(int)screen.leftTop.x, (int)screen.leftTop.y,
		(int)screen.rightTop.x, (int)screen.rightTop.y,
		(int)screen.leftBottom.x, (int)screen.leftBottom.y,
		(int)screen.rightBottom.x, (int)screen.rightBottom.y,
		(int)(_currentFrameIndex[index] * frameSize.x), 0,
		(int)frameSize.x, (int)frameSize.y, sprite, color);
}

void EnemyTools::FrameAnimation(int index, int frameTime, int frameSum, Vector2 frameSize, int sprite, Vector2 pos, float rotate, Vector2 scale, int color)
{
	//帧动画的帧计算
	if (FrameTimeWatch_ani(frameTime, index, true))
		_currentFrameIndex[index]++;
	if (_currentFrameIndex[index] > frameSum - 1 || _currentFrameIndex[index] < 0)
		_currentFrameIndex[index] = 0;
	//绘图(直接在屏幕上)
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
		if (it->Get_isDead())
			it->ToDead();
		else if (it->Get_isGetPlayer())
			it->ToGetPlayer();
		else
			it->Update(keys, preKeys);
	}
}

Enemy* EnemyManager::AcquireEnemy(Camera* camera, Vector2 pos, Enemy::Type type, Vector2 targetPos)
{
	if (_idlePool.empty()) {
		Enemy* enemy = new Enemy();
		enemy->Initialize(camera, pos, type, targetPos);
		return enemy;
	}
	else {
		Enemy* enemy = _idlePool.front();
		_idlePool.pop();
		enemy->Initialize(camera, pos, type, targetPos);
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

void EnemyManager::BornEnemy(Camera* camera, int score, int friendSum)
{
	std::random_device rd;
	std::mt19937 gen(rd());

	_bornEnemyTime = 45;							//路线中生成敌人的时间

	//按照分数调整难度
	if (score > 3000) {
		_linesSum = 6;				//当前多少条线路
		_lineTime = 30;				//进行随机选择路线的时间
		_eachBornMax = 5;			//每回至多生成敌人数量
		_enemyType_fly[3] = { Enemy::tEagles };
	}
	else if (score > 2500) {
		_linesSum = 6;
		_lineTime = 30;
		_eachBornMax = 5;
	}
	else if (score > 2000) {
		_linesSum = 6;
		_lineTime = 40;
		_eachBornMax = 5;
		_enemyType_fly[2] = { Enemy::tEagles };
	}
	else if (score > 1700) {
		_linesSum = 6;
		_lineTime = 50;
		_eachBornMax = 4;
	}
	else if (score > 1300) {
		_linesSum = 6;
		_lineTime = 60;
		_eachBornMax = 4;
		_enemyType_fly[1] = { Enemy::tEagles };
	}
	else if (score > 1000) {
		_linesSum = 5;
		_lineTime = 60;
		_eachBornMax = 4;
		_enemyType_walk[2] = { Enemy::tSpider };
	}
	else if (score > 700) {
		_linesSum = 5;
		_lineTime = 50;
		_eachBornMax = 3;
	}
	else if (score > 400) {
		_linesSum = 4;
		_lineTime = 20;
		_eachBornMax = 3;
		_enemyType_walk[1] = { Enemy::tSpider };
	}
	else if (score > 200) {
		_linesSum = 4;
		_lineTime = 20;
		_eachBornMax = 2;
	}
	else if (score > 100) {
		_linesSum = 3;
		_lineTime = 15;
		_eachBornMax = 2;
	}
	else {
		_linesSum = 2;
		_lineTime = 10;
		_eachBornMax = 2;
	}

	//按照小伙伴人数调整生成新的小伙伴的几率
	if (friendSum < 5)
		_bornFriendRandom = 1;
	else if (friendSum < 10)
		_bornFriendRandom = 2;
	else
		_bornFriendRandom = 3;

	//随机选择路线并填入敌人
	if (FrameTimeWatch(_lineTime, 0, true)) {
		//往哪个路线填入敌人
		std::uniform_int_distribution<int> dis_line(0, _linesSum - 1);
		int lineNum = dis_line(gen);
		//根据路线(天空和地面)，随机出本次要出现的敌人
		Enemy::Type enemyType = Enemy::tSnake;
		if (lineNum == 0 || lineNum == 1) {
			//地面路线
			std::uniform_int_distribution<int> dis_enemyIndex(0, _enemyTypeSum - 1);
			enemyType = _enemyType_walk[dis_enemyIndex(gen)];
		}
		else {
			//天空路线
			std::uniform_int_distribution<int> dis_enemyIndex(0, _enemyTypeSum - 1);
			enemyType = _enemyType_fly[dis_enemyIndex(gen)];
		}
		//往路线中填入多少个敌人
		std::uniform_int_distribution<int> dis_enemySum(1, _eachBornMax);
		int enemySum = dis_enemySum(gen);
		for (int i = 0; i < enemySum; i++) {
			Vector2 bornPos = _targetPos + _bornPosOffset[lineNum];
			Enemy* it = AcquireEnemy(camera, bornPos, enemyType, _targetPos);
			_enemyLines[lineNum].push(it);
		}
		for (int i = 0; i < _bornEnemySpace; i++)//后占位，隔开敌人
			_enemyLines[lineNum].push(nullptr);

	}

	//随机选择路线并填入小伙伴
	if (FrameTimeWatch(_bornFriendTime, 2, false)) {
		//是否生成小伙伴
		std::uniform_int_distribution<int> dis_playerRandom(0, friendSum);
		int playerRandom = dis_playerRandom(gen);
		if (playerRandom <= _bornFriendRandom) {
			//往哪个路线填入
			std::uniform_int_distribution<int> dis_line(0, _linesSum - 1);
			int lineNum = dis_line(gen);
			//根据路线判断是天空还是地面
			int sprite = 0;
			Vector2 spriteSize{};
			if (lineNum == 0 || lineNum == 1)
			{
				sprite = _spPlayer_walk;
				spriteSize = { 384 / 4.f,96 };
			}
			else
			{
				sprite = _spPlayer_fly;
				spriteSize = { 432 / 4.f,177 };
			}
			//生成小伙伴
			Vector2 bornPos = _targetPos + _bornPosOffset[lineNum];
			for (int i = 0; i < _bornFriendSpace; i++)//前占位，避免敌人挤压小伙伴
				_enemyLines[lineNum].push(nullptr);
			Enemy* it = AcquireEnemy(camera, bornPos, Enemy::tPlayer, _targetPos);
			it->Set_sprite(sprite);
			it->Set_spriteSize(spriteSize);
			_enemyLines[lineNum].push(it);
			for (int i = 0; i < _bornFriendSpace; i++)//后占位
				_enemyLines[lineNum].push(nullptr);
		}
	}

	//将路线中储存的敌人和小伙伴释放出来
	if (FrameTimeWatch(_bornEnemyTime, 1, true)) {
		for (int i = 0; i < _linesSum; i++) {
			if (!_enemyLines[i].empty()) {
				Enemy* it = _enemyLines[i].front();
				if (it != nullptr)
					it->PushUpdate();
				_enemyLines[i].pop();
			}
		}
	}
}

void EnemyManager::ClearAllEnemy()
{
	//将待生成队列中的敌人全部回收并清除
	for (int i = 0; i < _linesSum; i++) {
		while (!_enemyLines[i].empty())
		{
			Enemy* it = _enemyLines[i].front();
			if (it != nullptr)
				ReleaseEnemy(it);
			_enemyLines[i].pop();
		}
	}
	//将已激活的敌人全部回收并清除
	for (Enemy* it : _updatePool) {
		ReleaseEnemy(it);
	}
	_updatePool.clear();
}

void EnemyManager::RestartData()
{
	//重置能生成的敌人类型队列
	for (int i = 0; i < _enemyTypeSum; i++) {
		_enemyType_walk[i] = { Enemy::tSnake };
		_enemyType_fly[i] = { Enemy::tBee };
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
	_spSnake_hurt = Novice::LoadTexture("./RS/Enemy/snake_hurt.png");
	_spEagles = Novice::LoadTexture("./RS/Enemy/hawk_walk.png");
	_spEagles_hurt = Novice::LoadTexture("./RS/Enemy/hawk_hurt.png");
	_spSpider = Novice::LoadTexture("./RS/Enemy/spider_walk.png");
	_spSpider_hurt = Novice::LoadTexture("./RS/Enemy/spider_hurt.png");
	_spBee = Novice::LoadTexture("./RS/Enemy/bee_walk.png");
	_spPlayer_walk = Novice::LoadTexture("./RS/Enemy/friend_walk.png");
	_spPlayer_fly = Novice::LoadTexture("./RS/Enemy/friend_fly.png");
	_spPlayer_sad = Novice::LoadTexture("./RS/Enemy/player_sad.png");
}

