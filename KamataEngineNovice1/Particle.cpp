#include "Particle.h"

Particle::Particle(Camera* camera, Vector2 pos, TYPE type)
{
	Initialize(camera, pos, type);
	math_ = new MathFunc();
}

void Particle::Initialize(Camera* camera, Vector2 pos, TYPE type)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	_camera = camera;

	_currentTime = 0;
	_pos = pos;
	_vel = { 0,0 };
	_acc = { 0,0 };
	_dir = { 0,0 };
	_gravity = 0.7f;
	_type = type;
	_sprite = {};
	_angle = 0;
	_scale = { 1,1 };
	_alphaValue = 255;
	_isAudio = false;

	switch (_type) {
	case fireWorks: {
		_speed = 0.5f;
		std::uniform_real_distribution dis_dirX(-1.f, 1.f);
		std::uniform_real_distribution dis_dirY(-0.5f, 2.f);
		_dir = { dis_dirX(gen), dis_dirY(gen) };
		_radius = 3;
		std::uniform_real_distribution dis_angle(-10.f, 10.f);
		_angle = dis_angle(gen);
		_color = WHITE;
		std::uniform_int_distribution dis_life(5, 10);
		_lifeTime = dis_life(gen);
		break;
	}
	case gunFire: {
		_speed = 0.01f;
		std::uniform_real_distribution dis_dirX(-1.f, 1.f);
		std::uniform_real_distribution dis_dirY(-0.5f, 2.f);
		_dir = { dis_dirX(gen), dis_dirY(gen) };
		_radius = 1;
		_color = 0xfff59dff;
		std::uniform_int_distribution dis_life(10, 20);
		_lifeTime = dis_life(gen);
		break;
	}
	case playerJump: {
		_speed = 0.1f;
		std::uniform_real_distribution dis_dirX(-0.7f, 0.7f);
		std::uniform_real_distribution dis_dirY(0.f, 2.f);
		_dir = { dis_dirX(gen), dis_dirY(gen) };
		_radius = 3;
		_color = WHITE;
		std::uniform_real_distribution dis_scale(2.f, 4.f);
		float randScale = dis_scale(gen);
		_scale = { randScale,randScale };
		std::uniform_int_distribution dis_life(40, 60);
		_lifeTime = dis_life(gen);
		break;
	}
	case enemyHurtL:
	case enemyHurtR: {
		_speed = 0.5f;
		_dir.x = _type == enemyHurtL ? -1.f : 1.f;
		std::uniform_real_distribution dis_dirY(-0.5f, 2.f);
		_dir.y = dis_dirY(gen);
		_radius = 3;
		_color = 0xce93d8ff;
		std::uniform_int_distribution dis_life(100, 120);
		_lifeTime = dis_life(gen);
		break;
	}
	case playerRunL:
	case playerRunR: {
		_speed = 0.05f;
		_dir.x = _type == playerRunL ? 1.f : -1.f;
		std::uniform_real_distribution dis_dirY(0.f, 1.f);
		_dir.y = dis_dirY(gen);
		_radius = 2;
		_color = 0xe0e0e0ff;
		std::uniform_real_distribution dis_scale(1.f, 2.f);
		_scale = { dis_scale(rd),dis_scale(rd) };
		std::uniform_int_distribution dis_life(5, 10);
		_lifeTime = dis_life(gen);
		break;
	}
	case playerHurt: {
		_speed = 0.5f;
		std::uniform_real_distribution dis_dirX(-1.f, 1.f);
		std::uniform_real_distribution dis_dirY(-0.5f, 2.f);
		_dir = { dis_dirX(gen), dis_dirY(gen) };
		_radius = 3;
		_color = 0xe51c23ff;
		std::uniform_int_distribution dis_life(100, 120);
		_lifeTime = dis_life(gen);
		break;
	}
	}
}

void Particle::Move()
{
	std::random_device rd;
	std::mt19937 gen(rd());

	switch (_type) {
	case fireWorks: {
		_acc.x = _dir.x * _speed;
		_acc.y = _dir.y * _speed;
		if (_currentTime < 10) {
			_vel.x += _acc.x;
			_vel.y += _acc.y;
		}
		_vel.y -= 0.5f;
		_pos = { _pos.x + _vel.x,_pos.y + _vel.y };
		break;
	}
	case gunFire: {
		_acc.x = _dir.x * _speed;
		_acc.y = _dir.y * _speed;
		if (_currentTime < 10) {
			_vel.x += _acc.x;
			_vel.y += _acc.y;
		}
		_vel.y += 0.1f;
		_pos = { _pos.x + _vel.x,_pos.y + _vel.y };
		_scale.x += 0.05f;
		_scale.y += 0.05f;
		float steps = float(_currentTime) / float(_lifeTime);
		_color = ColorInterpolation(0xfff59dff, RED, steps);
		break;
	}
	case playerJump: {
		_acc.x = _dir.x * _speed;
		_acc.y = _dir.y * _speed;
		if (_currentTime < 10) {
			_vel.x += _acc.x;
			_vel.y += _acc.y;
		}
		else {
			_vel.y *= 0.9f;
		}
		_pos = { _pos.x + _vel.x,_pos.y + _vel.y };
		if (_alphaValue > 5) {
			_color = 0xFFFFFF00 | _alphaValue << 0;
			_alphaValue -= 5;
		}
		_scale.x += 0.01f;
		_scale.y += 0.01f;
		break;
	}
	case enemyHurtL:
	case enemyHurtR: {
		if (_alphaValue > 5 && _currentTime > _lifeTime - 30) {
			_color = 0xce93d800 | _alphaValue << 0;
			_alphaValue -= 5;
		}

		_acc.x = _dir.x * _speed;
		_acc.y = _dir.y * _speed;
		if (_currentTime < 10) {
			_vel.x += _acc.x;
			_vel.y += _acc.y;
		}
		_vel.y -= 0.5f;
		break;
	}
	case playerRunL:
	case playerRunR: {
		_acc.x = _dir.x * _speed;
		_acc.y = _dir.y * _speed;
		if (_currentTime < 10) {
			_vel.x += _acc.x;
			_vel.y += _acc.y;
		}
		_pos = { _pos.x + _vel.x,_pos.y + _vel.y };
		_scale.x += 0.02f;
		_scale.y += 0.02f;
		break;
	}
	case playerHurt: {
		if (_alphaValue > 5 && _currentTime > _lifeTime - 30) {
			_color = 0xe51c2300 | _alphaValue << 0;
			_alphaValue -= 5;
		}

		_acc.x = _dir.x * _speed;
		_acc.y = _dir.y * _speed;
		if (_currentTime < 10) {
			_vel.x += _acc.x;
			_vel.y += _acc.y;
		}
		_vel.y -= 0.5f;
		break;
	}
	}

	_currentTime++;
}

void Particle::Draw()
{
	//根据摄像机计算屏幕坐标
	Affine _affine = { _scale,_angle,_pos };
	Matrix3x3 worldMatrix_ = math_->MakeAffine(_affine);
	Matrix3x3 wvpVpMatrix_ = math_->WvpVpMatrix(worldMatrix_, _camera->GetVpVpMatrix());
	Vertex local = {
		{ -_scale.x / 2.0f, +_scale.y / 2.0f},
		{ +_scale.x / 2.0f, +_scale.y / 2.0f},
		{ -_scale.x / 2.0f, -_scale.y / 2.0f},
		{ +_scale.x / 2.0f, -_scale.y / 2.0f},
	};
	Vertex _screen = math_->TransformSprite(local, wvpVpMatrix_);
	Vector2 screenPos = { _screen.leftTop.x - _scale.x / 2.0f ,_screen.leftTop.y + _scale.y / 2.0f };

	switch (_type) {
	case fireWorks:
		Novice::DrawBox(int(screenPos.x), int(screenPos.y), int(_radius * _scale.x), int(_radius * _scale.y), _angle, _color, kFillModeSolid);
		break;
	case gunFire:
	case playerJump:
	case enemyHurtL:
	case enemyHurtR:
	case playerRunL:
	case playerRunR:
	case playerHurt:
		Novice::DrawEllipse(int(screenPos.x), int(screenPos.y), int(_radius * _scale.x), int(_radius * _scale.y), _angle, _color, kFillModeSolid);
		break;
	}
}

void Particle::ToDead()
{
	if (_currentTime >= _lifeTime) {
		ParticleManager::ReleaseParticle(this);
	}
}

void Particle::PushUpdate()
{
	ParticleManager::_particleUpdatePool.push_back(this);
}

unsigned int Particle::ColorInterpolation(unsigned int startColor, unsigned int targetColor, float percent)
{

	uint8_t startAlpha = (startColor >> 24) & 0xFF;
	uint8_t startRed = (startColor >> 16) & 0xFF;
	uint8_t startGreen = (startColor >> 8) & 0xFF;
	uint8_t startBlue = startColor & 0xFF;

	uint8_t targetAlpha = (targetColor >> 24) & 0xFF;
	uint8_t targetRed = (targetColor >> 16) & 0xFF;
	uint8_t targetGreen = (targetColor >> 8) & 0xFF;
	uint8_t targetBlue = targetColor & 0xFF;

	uint8_t currentAlpha = uint8_t(startAlpha + percent * (targetAlpha - startAlpha));
	uint8_t currentRed = uint8_t(startRed + percent * (targetRed - startRed));
	uint8_t currentGreen = uint8_t(startGreen + percent * (targetGreen - startGreen));
	uint8_t currentBlue = uint8_t(startBlue + percent * (targetBlue - startBlue));

	unsigned int returnColor = (currentAlpha << 24) | (currentRed << 16) | (currentGreen << 8) | currentBlue;

	return returnColor;
}

Emitter::Emitter(Camera* camera, Vector2 pos, TYPE type)
{
	Initialize(camera, pos, type);
	math_ = new MathFunc();
}

void Emitter::Initialize(Camera* camera, Vector2 pos, TYPE type)
{
	_pos = pos;
	_scale = { 1,1 };
	_type = type;
	_isDead = false;
	_color = WHITE;
	_angle = 0;
	_currentTime = 0;
	_lifeTime = 0;
	_isLoop = false;
	_particleCurrentTime = 0;
	_camera = camera;

	switch (_type) {
	case fireWorks:
		_width = 5;
		_height = 5;
		_particleSum = 12;
		break;
	case gunFire:
		_width = 25;
		_height = 25;
		_particleSum = 1;
		_lifeTime = 2;
		_color = 0xfff59dff;
		break;
	case playerJump:
		_width = 2;
		_height = 10;
		_particleSum = 5;
		break;
	case enemyHurtL:
	case enemyHurtR:
		_width = 5;
		_height = 5;
		_particleSum = 2;
		break;
	case playerRunL:
	case playerRunR:
		_width = 2;
		_height = 10;
		_particleSum = 1;
		break;
	case playerHurt:
		_width = 5;
		_height = 5;
		_particleSum = 5;
	}
}

void Emitter::ParticleStart()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution disX(-_width / 2, _width / 2);
	std::uniform_real_distribution disY(-_height / 2, _height / 2);
	Vector2 randomPos{};
	Particle* element{};

	if (_particleCurrentTime == 0) {
		for (int i = 0; i < _particleSum; i++) {
			randomPos = { disX(gen),disY(gen) };
			randomPos = { randomPos.x + _pos.x,randomPos.y + _pos.y };
			switch (_type) {
			case fireWorks:
				element = ParticleManager::AcquireParticle(_camera, randomPos, Particle::fireWorks);
				element->PushUpdate();
				break;
			case gunFire:
				element = ParticleManager::AcquireParticle(_camera, randomPos, Particle::gunFire);
				element->PushUpdate();
				break;
			case playerJump:
				element = ParticleManager::AcquireParticle(_camera, randomPos, Particle::playerJump);
				element->PushUpdate();
				break;
			case enemyHurtL:
				element = ParticleManager::AcquireParticle(_camera, randomPos, Particle::enemyHurtL);
				element->PushUpdate();
				break;
			case enemyHurtR:
				element = ParticleManager::AcquireParticle(_camera, randomPos, Particle::enemyHurtR);
				element->PushUpdate();
				break;
			case playerRunL:
				element = ParticleManager::AcquireParticle(_camera, randomPos, Particle::playerRunL);
				element->PushUpdate();
				break;
			case playerRunR:
				element = ParticleManager::AcquireParticle(_camera, randomPos, Particle::playerRunR);
				element->PushUpdate();
				break;
			case playerHurt:
				element = ParticleManager::AcquireParticle(_camera, randomPos, Particle::playerHurt);
				element->PushUpdate();
				break;
			}
		}
	}

	//Emitter生きる時間
	_currentTime++;
	if (_currentTime >= _lifeTime && !_isLoop) {
		_isDead = true;
	}
	//パーティクルを生まれる時間
	_particleCurrentTime++;
	if (_particleCurrentTime > _particleGetTime) {
		_particleCurrentTime = 0;
	}
}

void Emitter::Draw()
{
	//根据摄像机计算屏幕坐标
	Affine _affine = { _scale,_angle,_pos };
	Matrix3x3 worldMatrix_ = math_->MakeAffine(_affine);
	Matrix3x3 wvpVpMatrix_ = math_->WvpVpMatrix(worldMatrix_, _camera->GetVpVpMatrix());
	Vertex local = {
		{ -_scale.x / 2.0f, +_scale.y / 2.0f},
		{ +_scale.x / 2.0f, +_scale.y / 2.0f},
		{ -_scale.x / 2.0f, -_scale.y / 2.0f},
		{ +_scale.x / 2.0f, -_scale.y / 2.0f},
	};
	Vertex _screen = math_->TransformSprite(local, wvpVpMatrix_);
	Vector2 screenPos = { _screen.leftTop.x - _scale.x / 2.0f ,_screen.leftTop.y + _scale.y / 2.0f };

	switch (_type) {
	case gunFire:
		Novice::DrawEllipse(int(screenPos.x), int(screenPos.y), int(_width / 2 * _scale.x), int(_height / 2 * _scale.y), _angle, _color, kFillModeSolid);
		break;
	}
}

void Emitter::ToDead()
{
	if (_isDead) {
		ParticleManager::ReleaseEmitter(this);
	}
}

void Emitter::PushUpdate()
{
	ParticleManager::_emitterUpadtePool.push_back(this);
}

void ParticleManager::ParticleUpdate()
{
	for (Particle* it : _particleUpdatePool) {
		it->Move();
		it->ToDead();
	}
}

void ParticleManager::ParticleUpdateShow()
{
	for (Particle* it : _particleUpdatePool) {
		it->Draw();
	}
}

Particle* ParticleManager::AcquireParticle(Camera* camera, Vector2 startPos, Particle::TYPE type)
{
	if (_particleIdiePool.empty()) {
		Particle* par = new Particle(camera, startPos, type);
		return par;
	}
	else {
		Particle* par = _particleIdiePool.front();
		_particleIdiePool.pop();
		par->Initialize(camera, startPos, type);
		return par;
	}
	//Particle* par = new Particle(startPos, type);
	//return par;
}

void ParticleManager::ReleaseParticle(Particle* particle)
{
	auto it = find(_particleUpdatePool.begin(), _particleUpdatePool.end(), particle);
	if (it != _particleUpdatePool.end()) {
		std::swap(*it, _particleUpdatePool.back());
		_particleUpdatePool.pop_back();
		_particleIdiePool.push(particle);
	}
}


void ParticleManager::EmitterUpdate()
{
	for (Emitter* it : _emitterUpadtePool) {
		it->ParticleStart();
		it->ToDead();
	}
}

void ParticleManager::EmitterUpdateShow()
{
	for (Emitter* emi : _emitterUpadtePool) {
		emi->Draw();
	}
}

Emitter* ParticleManager::AcquireEmitter(Camera* camera, Vector2 pos, Emitter::TYPE type)
{
	if (_emitterIdiePool.empty()) {
		return new Emitter(camera, pos, type);
	}
	else {
		Emitter* emi = _emitterIdiePool.front();
		_emitterIdiePool.pop();
		emi->Initialize(camera, pos, type);
		return emi;
	}
	//Emitter* emi = new Emitter(pos, type);
	//return emi;
}

void ParticleManager::ReleaseEmitter(Emitter* emitter)
{
	auto it = find(_emitterUpadtePool.begin(), _emitterUpadtePool.end(), emitter);
	if (it != _emitterUpadtePool.end()) {
		std::swap(*it, _emitterUpadtePool.back());
		_emitterUpadtePool.pop_back();
		_emitterIdiePool.push(emitter);
	}
}

void ParticleManager::ADD_Particle(Camera* camera, Vector2 pos, Emitter::TYPE type)
{
	Emitter* emi = AcquireEmitter(camera, pos, type);
	emi->PushUpdate();
}

void ParticleManager::Update()
{
	ParticleUpdate();
	EmitterUpdate();
}

void ParticleManager::Draw()
{
	ParticleUpdateShow();
	EmitterUpdateShow();
}
