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
	_camera = camera; _currentTime = 0;
	_pos = pos; _vel = { 0,0 }; _acc = { 0,0 }; _dir = { 0,0 }; _gravity = 0.7f;
	_type = type; _sprite = 0; _spriteSize = { 6,6 };
	_angle = 0; _scale = { 1,1 }; _color = WHITE; _alphaValue = 255;
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
		std::uniform_int_distribution dis_life(5, 10);
		_lifeTime = dis_life(gen);
		break;
	}
	case bulletHurt: {
		_spriteSize = { 20,20 };
		_sprite = Novice::LoadTexture("./RS/Particle/stone.png");
		_speed = 0.1f;
		std::uniform_real_distribution dis_dirX(-0.7f, 0.7f);
		std::uniform_real_distribution dis_dirY(0.f, 2.f);
		_dir = { dis_dirX(gen), dis_dirY(gen) };
		std::uniform_real_distribution dis_angle(-10.f, 10.f);
		_angle = dis_angle(gen);
		std::uniform_real_distribution dis_scale(0.5f, 0.7f);
		float randScale = dis_scale(gen);
		_scale = { randScale,randScale };
		std::uniform_int_distribution dis_life(40, 60);
		_lifeTime = dis_life(gen);
		break;
	}
	case minusScore: {
		_spriteSize = { 99,44 };
		_sprite = Novice::LoadTexture("./RS/Particle/minus.png");
		_speed = 0.2f;
		_dir = { 0, 1 };
		_scale = { 0.7f,0.7f };
		_lifeTime = 30;
		break;
	}
	case plusScore: {
		_spriteSize = { 46,25 };
		_sprite = Novice::LoadTexture("./RS/Particle/bonus.png");
		_speed = 0.2f;
		_dir = { 0, 1 };
		_scale = { 1.0f,1.0f };
		_lifeTime = 30;
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
	case bulletHurt: {
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
	case minusScore:
	case plusScore: {
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
		{ -_spriteSize.x / 2.0f, +_spriteSize.y / 2.0f},
		{ +_spriteSize.x / 2.0f, +_spriteSize.y / 2.0f},
		{ -_spriteSize.x / 2.0f, -_spriteSize.y / 2.0f},
		{ +_spriteSize.x / 2.0f, -_spriteSize.y / 2.0f},
	};
	Vertex _screen = math_->TransformSprite(local, wvpVpMatrix_);
	Vector2 screenPos = { _screen.leftTop.x - _spriteSize.x / 2.0f ,_screen.leftTop.y + _spriteSize.y / 2.0f };

	switch (_type) {
	case fireWorks:
		Novice::DrawBox(int(screenPos.x), int(screenPos.y), int(_radius * _scale.x), int(_radius * _scale.y), _angle, _color, kFillModeSolid);
		break;
	case bulletHurt:
		Novice::DrawQuad(
			(int)_screen.leftTop.x, (int)_screen.leftTop.y,
			(int)_screen.rightTop.x, (int)_screen.rightTop.y,
			(int)_screen.leftBottom.x, (int)_screen.leftBottom.y,
			(int)_screen.rightBottom.x, (int)_screen.rightBottom.y,
			0, 0, (int)_spriteSize.x, (int)_spriteSize.y,
			_sprite, _color);
		break;
	}
}

void Particle::PreDraw()
{
	//根据摄像机计算屏幕坐标
	Affine _affine = { _scale,_angle,_pos };
	Matrix3x3 worldMatrix_ = math_->MakeAffine(_affine);
	Matrix3x3 wvpVpMatrix_ = math_->WvpVpMatrix(worldMatrix_, _camera->GetVpVpMatrix());
	Vertex local = {
		{ -_spriteSize.x / 2.0f, +_spriteSize.y / 2.0f},
		{ +_spriteSize.x / 2.0f, +_spriteSize.y / 2.0f},
		{ -_spriteSize.x / 2.0f, -_spriteSize.y / 2.0f},
		{ +_spriteSize.x / 2.0f, -_spriteSize.y / 2.0f},
	};
	Vertex _screen = math_->TransformSprite(local, wvpVpMatrix_);
	Vector2 screenPos = { _screen.leftTop.x - _spriteSize.x / 2.0f ,_screen.leftTop.y + _spriteSize.y / 2.0f };

	switch (_type) {
	case minusScore:
	case plusScore:
		Novice::DrawQuad(
			(int)_screen.leftTop.x, (int)_screen.leftTop.y,
			(int)_screen.rightTop.x, (int)_screen.rightTop.y,
			(int)_screen.leftBottom.x, (int)_screen.leftBottom.y,
			(int)_screen.rightBottom.x, (int)_screen.rightBottom.y,
			0, 0, (int)_spriteSize.x, (int)_spriteSize.y,
			_sprite, _color);
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
	_camera = camera; _type = type;
	_pos = pos; _scale = { 1,1 }; _angle = 0; _color = WHITE;
	_currentTime = 0; _lifeTime = 0; _particleCurrentTime = 0;
	_width = 2; _height = 2; _particleSum = 1;
	_isDead = false; _isLoop = false;

	switch (_type) {
	case fireWorks:
		_width = 5;
		_height = 5;
		_particleSum = 12;
		break;
	case bulletHurt:
		_width = 2;
		_height = 10;
		_particleSum = 5;
		break;
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
			Particle::TYPE parType = Particle::TYPE(_type);
			element = ParticleManager::AcquireParticle(_camera, randomPos, parType);
			element->PushUpdate();
			//switch (_type) {
			//case fireWorks:
			//	element = ParticleManager::AcquireParticle(_camera, randomPos, Particle::fireWorks);
			//	element->PushUpdate();
			//	break;
			//case bulletHurt:
			//	element = ParticleManager::AcquireParticle(_camera, randomPos, Particle::bulletHurt);
			//	element->PushUpdate();
			//	break;
			//}
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
		{ -_width / 2.0f, +_height / 2.0f},
		{ +_width / 2.0f, +_height / 2.0f},
		{ -_width / 2.0f, -_height / 2.0f},
		{ +_width / 2.0f, -_height / 2.0f},
	};
	Vertex _screen = math_->TransformSprite(local, wvpVpMatrix_);
	Vector2 screenPos = { _screen.leftTop.x - _width / 2.0f ,_screen.leftTop.y + +_height / 2.0f };
}

void Emitter::PreDraw()
{
	//根据摄像机计算屏幕坐标
	Affine _affine = { _scale,_angle,_pos };
	Matrix3x3 worldMatrix_ = math_->MakeAffine(_affine);
	Matrix3x3 wvpVpMatrix_ = math_->WvpVpMatrix(worldMatrix_, _camera->GetVpVpMatrix());
	Vertex local = {
		{ -_width / 2.0f, +_height / 2.0f},
		{ +_width / 2.0f, +_height / 2.0f},
		{ -_width / 2.0f, -_height / 2.0f},
		{ +_width / 2.0f, -_height / 2.0f},
	};
	Vertex _screen = math_->TransformSprite(local, wvpVpMatrix_);
	Vector2 screenPos = { _screen.leftTop.x - _width / 2.0f ,_screen.leftTop.y + +_height / 2.0f };
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

void ParticleManager::PreDraw()
{
	for (Emitter* emi : _emitterUpadtePool) {
		emi->PreDraw();
	}
	for (Particle* it : _particleUpdatePool) {
		it->PreDraw();
	}
}
