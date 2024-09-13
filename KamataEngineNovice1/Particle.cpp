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
		_sprite = ParticleManager::_spBulletHurt;
		_speed = 0.2f;
		std::uniform_real_distribution dis_dirX(-0.7f, 0.7f);
		std::uniform_real_distribution dis_dirY(0.f, 2.f);
		_dir = { dis_dirX(gen), dis_dirY(gen) };
		std::uniform_real_distribution dis_angle(-10.f, 10.f);
		_angle = dis_angle(gen);
		std::uniform_real_distribution dis_scale(0.5f, 0.7f);
		float randScale = dis_scale(gen);
		_scale = { randScale,randScale };
		std::uniform_int_distribution dis_life(60, 80);
		_lifeTime = dis_life(gen);
		break;
	}
	case minusScore: {
		_spriteSize = { 99,44 };
		_sprite = ParticleManager::_spMinusScore;
		_speed = 0.2f;
		_dir = { 0, 1 };
		_scale = { 0.6f,0.6f };
		_lifeTime = 30;
		break;
	}
	case plusScore: {
		_spriteSize = { 46,25 };
		_sprite = ParticleManager::_spPlusScore;
		_speed = 0.2f;
		_dir = { 0, 1 };
		_scale = { 1.0f,1.0f };
		_lifeTime = 30;
		break;
	}
	case plusScore_long: {
		_spriteSize = { 57,52 };
		_sprite = ParticleManager::_spPlusScore_long;
		_speed = 0.2f;
		_dir = { 0, 1 };
		_scale = { 1.0f,1.0f };
		_lifeTime = 30;
		break;
	}
	case emotion_happy:
	case emotion_normal:
	case emotion_sad: {
		switch (_type) {
		case Particle::emotion_happy:_sprite = ParticleManager::_spEmotion_happy; break;
		case Particle::emotion_normal:_sprite = ParticleManager::_spEmotion_normal; break;
		case Particle::emotion_sad:_sprite = ParticleManager::_spEmotion_sad; break;
		}
		_spriteSize = { 128,128 };
		_speed = 1.f;
		_dir = { 1.f, 1.f };
		_lifeTime = 100;
		_scale = { 0,0 };
		Vector2 posOffset = { 40,60 };//偏移
		_pos += posOffset;
		break;
	}
	case friendDead: {
		_spriteSize = { 29,4 };
		_sprite = ParticleManager::_spFriendDead;
		_scale = { 1.f,2.f };
		_speed = 0.5f;
		std::uniform_real_distribution dis_dirX(-1.f, 1.f);
		_dir.x = dis_dirX(gen);
		std::uniform_real_distribution dis_dirY(2.f, 2.5f);
		_dir.y = dis_dirY(gen) * -1;
		std::uniform_real_distribution dis_angle(-10.f, 10.f);
		_angle = dis_angle(gen);
		_lifeTime = 240;
		break;
	}
	case friendAdd: {
		_spriteSize = { 44,44 };
		_sprite = ParticleManager::_spFriendAdd;
		_scale = { 0.5f,0.5f };
		_speed = 0.5f;
		std::uniform_real_distribution dis_dirX(-0.5f, 0.5f);
		_dir.x = dis_dirX(gen);
		std::uniform_real_distribution dis_dirY(2.f, 2.5f);
		_dir.y = dis_dirY(gen) * -1;
		std::uniform_real_distribution dis_angle(-10.f, 10.f);
		_angle = dis_angle(gen);
		_lifeTime = 240;
		break;
	}
	case happy: {
		_spriteSize = { 22,22 };
		_sprite = ParticleManager::_spHappy;
		_speed = 0.2f;
		std::uniform_real_distribution dis_dirX(-0.7f, 0.7f);
		std::uniform_real_distribution dis_dirY(0.f, 2.f);
		_dir = { dis_dirX(gen), dis_dirY(gen) };
		std::uniform_real_distribution dis_angle(-10.f, 10.f);
		_angle = dis_angle(gen);
		std::uniform_real_distribution dis_scale(0.5f, 0.7f);
		float randScale = dis_scale(gen);
		_scale = { randScale,randScale };
		std::uniform_int_distribution dis_life(60, 80);
		_lifeTime = dis_life(gen);
		break;
	}
	case unHappy: {
		_spriteSize = { 22,22 };
		_sprite = ParticleManager::_spUnHappy;
		_speed = 0.2f;
		std::uniform_real_distribution dis_dirX(-0.5f, 0.5f);
		_dir.x = dis_dirX(gen);
		std::uniform_real_distribution dis_dirY(2.f, 2.5f);
		_dir.y = dis_dirY(gen) * -1;
		std::uniform_real_distribution dis_angle(-10.f, 10.f);
		_angle = dis_angle(gen);
		std::uniform_int_distribution dis_life(60, 80);
		_lifeTime = dis_life(gen);
		break;
	}
	case unHappy_screen: {
		_spriteSize = { 22,22 };
		_sprite = ParticleManager::_spUnHappy;
		_speed = 0.2f;
		std::uniform_real_distribution dis_dirX(-0.5f, 0.5f);
		_dir.x = dis_dirX(gen);
		std::uniform_real_distribution dis_dirY(2.f, 2.5f);
		_dir.y = dis_dirY(gen);
		std::uniform_real_distribution dis_angle(-10.f, 10.f);
		_angle = dis_angle(gen);
		std::uniform_int_distribution dis_life(60, 80);
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
	case plusScore:
	case plusScore_long: {
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
	case emotion_happy:
	case emotion_normal:
	case emotion_sad: {
		int aniTime_in = 30;
		int aniTime_out = 20;
		if (_currentTime < aniTime_in) {
			float t = _currentTime / float(aniTime_in);
			_pos += _dir * _speed * EaseOutQuint(1 - t);
			_scale = { 1 * EaseOutQuint(t),1 * EaseOutQuint(t) };
		}
		else if (_currentTime > _lifeTime - aniTime_out) {
			float t = (_currentTime - (_lifeTime - aniTime_out)) / (float)(_lifeTime - aniTime_out);
			_alphaValue = int(255 * EaseInCirc(1 - t));
			_color = 0xFFFFFF00 | _alphaValue << 0;
		}
		break;
	}
	case friendDead:
	case friendAdd: {
		if (_alphaValue > 5 && _currentTime > _lifeTime - 30) {
			_color = 0xFFFFFF00 | _alphaValue << 0;
			_alphaValue -= 5;
		}
		float bounce = 0.6f;
		float gravity = -0.5f;
		Vector2 backupPos = { _pos.x,_pos.y };
		_angle += 0.1f;
		_acc.y = _dir.y * _speed;
		if (_currentTime < 10) {
			_vel.y += _acc.y;
		}
		_vel.y -= gravity;
		_pos.y -= _vel.y;
		_acc.x = _dir.x * _speed;
		if (_currentTime < 10) {
			_vel.x += _acc.x;
		}
		_pos.x += _vel.x;
		if (_pos.y < 200) {
			_pos.y = backupPos.y;
			_vel.x *= bounce;
			_vel.y *= -bounce;
			_angle *= -bounce;
		}
		break;
	}
	case happy: {
		_acc.x = _dir.x * _speed * 0.5f;
		_acc.y = _dir.y * _speed;
		if (_currentTime < 10) {
			_vel.x += _acc.x;
			_vel.y += _acc.y;
		}
		else {
			_vel.y *= 0.95f;
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
	case unHappy: {
		if (_alphaValue > 5 && _currentTime > _lifeTime - 30) {
			_color = 0xFFFFFF00 | _alphaValue << 0;
			_alphaValue -= 5;
		}
		float gravity = -0.1f;
		Vector2 backupPos = { _pos.x,_pos.y };
		_angle += 0.1f;
		_acc.y = _dir.y * _speed;
		if (_currentTime < 10) {
			_vel.y += _acc.y;
		}
		_vel.y -= gravity;
		_pos.y -= _vel.y;
		_acc.x = _dir.x * _speed;
		if (_currentTime < 10) {
			_vel.x += _acc.x;
		}
		_pos.x += _vel.x;
		break;
	}
	case unHappy_screen: {
		if (_alphaValue > 5 && _currentTime > _lifeTime - 30) {
			_color = 0xFFFFFF00 | _alphaValue << 0;
			_alphaValue -= 5;
		}
		float gravity = 0.1f;
		Vector2 backupPos = { _pos.x,_pos.y };
		_angle += 0.1f;
		_acc.y = _dir.y * _speed;
		if (_currentTime < 10) {
			_vel.y += _acc.y;
		}
		_vel.y -= gravity;
		_pos.y -= _vel.y;
		_acc.x = _dir.x * _speed;
		if (_currentTime < 10) {
			_vel.x += _acc.x;
		}
		_pos.x += _vel.x;
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
	case friendDead:
	case friendAdd:
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
	case plusScore_long:
	case emotion_happy:
	case emotion_normal:
	case emotion_sad:
	case happy:
	case unHappy:
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

void Particle::ScreenDraw()
{
	switch (_type) {
	case unHappy_screen:
		int listX = 0;
		int listY = 0;
		int listW = int(_spriteSize.x);
		int listH = int(_spriteSize.y);
		Vector2 zoom = { _spriteSize.x / 2.f - _spriteSize.x / 2.f * _scale.x,_spriteSize.y / 2.f + _spriteSize.y / 2.f * _scale.y };
		Vector2 rotatedPos = { -_spriteSize.x / 2.f + zoom.x ,_spriteSize.y / 2.f - zoom.y };
		float aditionX = rotatedPos.x * cosf(_angle) - rotatedPos.y * sinf(_angle);
		float aditionY = rotatedPos.y * cosf(_angle) + rotatedPos.x * sinf(_angle);
		rotatedPos = { aditionX,aditionY };
		rotatedPos = { rotatedPos.x + _pos.x ,rotatedPos.y + _pos.y };
		Novice::DrawSpriteRect((int)(rotatedPos.x), (int)(rotatedPos.y), listX, listY, int(_spriteSize.x), int(_spriteSize.y), _sprite, _spriteSize.x / listW * _scale.x, _spriteSize.y / listH * _scale.y, _angle, _color);
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
	_isDead = false; _isLoop = false; _isFirst = true;

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
	case friendAdd:
		_width = 5;
		_height = 5;
		_particleSum = 5;
		break;
	case friendDead:
		_width = 5;
		_height = 5;
		_particleSum = 5;
		_lifeTime = 20;
		_particleGetTime = 10;
		_isFirst = false;
		_scale = { 20,20 };
		break;
	case happy:
	case unHappy:
	case unHappy_screen:
		_width = 20;
		_height = 10;
		_particleSum = 8;
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

	if (_particleCurrentTime == 0 && _isFirst) {
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
	_isFirst = true;
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
	switch (_type)
	{
	case Emitter::friendDead:
		FrameAnimation(0, _lifeTime / 4, 4, { 64,64 }, ParticleManager::_spListBite, _screen, _color);
		break;
	}
}

void Emitter::PreDraw()
{
}

void Emitter::ScreenDraw()
{
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

bool Emitter::FrameTimeWatch_ani(int frame, int index, bool first)
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

void Emitter::FrameAnimation(int index, int frameTime, int frameSum, Vector2 frameSize, int sprite, Vertex screen, int color)
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

void ParticleManager::LoadRes()
{
	_spPlusScore = Novice::LoadTexture("./RS/Particle/bonus.png");
	_spPlusScore_long = Novice::LoadTexture("./RS/Particle/bonus_long.png");
	_spMinusScore = Novice::LoadTexture("./RS/Particle/minus.png");
	_spBulletHurt = Novice::LoadTexture("./RS/Particle/stone.png");
	_spEmotion_happy = Novice::LoadTexture("./RS/Particle/emotion_1.png");
	_spEmotion_normal = Novice::LoadTexture("./RS/Particle/emotion_2.png");
	_spEmotion_sad = Novice::LoadTexture("./RS/Particle/emotion_3.png");
	_spFriendAdd = Novice::LoadTexture("./RS/Particle/friendAdd.png");
	_spFriendDead = Novice::LoadTexture("./RS/Particle/friendDead.png");
	_spHappy = Novice::LoadTexture("./RS/Particle/happy.png");
	_spUnHappy = Novice::LoadTexture("./RS/Particle/unHappy.png");

	_spListBite = Novice::LoadTexture("./RS/Particle/bite.png");
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

void ParticleManager::ScreenDraw()
{
	for (Emitter* emi : _emitterUpadtePool) {
		emi->ScreenDraw();
	}
	for (Particle* it : _particleUpdatePool) {
		it->ScreenDraw();
	}
}
