#include "Grid.h"

void Grid::DrawBullet(int currentBullet, int maxBullet)
{
	float bgW = 123;							//背景条的长
	float cutLineNextX = bgW / maxBullet;		//下一个分割条的X位置
	unsigned int bulletColor1 = 0xfdd835ff;		//子弹条的单颗子弹颜色1
	unsigned int bulletColor2 = 0xfbc02dff;		//子弹条的单颗子弹颜色2
	//最开始先做一个子弹的判断(防止子弹数超过最大子弹数，UI出问题)
	currentBullet = currentBullet > maxBullet ? maxBullet : currentBullet;
	//画出背景
	Novice::DrawSprite(int(_bulletPos.x), int(_bulletPos.y), _spBullet_bg, 1, 1, 0.0f, WHITE);
	//画出当前子弹
	if (currentBullet != maxBullet) {
		Novice::DrawBox(int(_bulletPos.x + 3), int(_bulletPos.y + 3), int(currentBullet * cutLineNextX), 16, 0, bulletColor1, kFillModeSolid);
		Novice::DrawBox(int(_bulletPos.x + 3), int(_bulletPos.y + 3 + 9), int(currentBullet * cutLineNextX), 16 - 9, 0, bulletColor2, kFillModeSolid);
	}
	else {
		Novice::DrawBox(int(_bulletPos.x + 3), int(_bulletPos.y + 3), 117, 16, 0, bulletColor1, kFillModeSolid);
		Novice::DrawBox(int(_bulletPos.x + 3), int(_bulletPos.y + 3 + 9), 117, 16 - 9, 0, bulletColor2, kFillModeSolid);
	}
	//画出边框
	Novice::DrawSprite(int(_bulletPos.x), int(_bulletPos.y), _spBullet_outline, 1, 1, 0.0f, WHITE);
	//画出分割线
	for (int i = 1; i < maxBullet; i++) {
		Novice::DrawSprite(int(_bulletPos.x + i * cutLineNextX), int(_bulletPos.y + 3), _spBullet_cutline, 1, 1, 0.0f, WHITE);
	}

	//空子弹时候的抖动
	if (Novice::IsTriggerMouse(0) && currentBullet <= 0
		|| Novice::IsTriggerMouse(1) && currentBullet <= 0)
		_isBulletShake = true;
	if (_isBulletShake) {
		if (FrameTimeWatch(30, 0, false))
			_isBulletShake = false;
		BulletShake();
	}
	else
		_bulletShakeOffset = { 0,0 };
}

void Grid::BulletShake()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis_moveX(-_randBulletShake, _randBulletShake);
	std::uniform_real_distribution<float> dis_moveY(-_randBulletShake, _randBulletShake);
	_bulletShakeOffset = Vector2{ dis_moveX(rd),dis_moveY(rd) };
	_bulletPos = _bulletPosOffset + _bulletShakeOffset;
}

void Grid::HelpButtom()
{
	if (_isHelpStart)
	{
		if (FrameTimeWatch(_helpStartTime, 1, false)) {
			_isHelpStart = false;
		}
		if (_currentTimes[1] > _helpStartTime - 60) {
			_helpBottomScale -= Vector2{ 0.05f,0.05f };
			if (_helpBottomScale.x < 0)
				_helpBottomScale = { 0,0 };
		}
		FrameAnimation(0, 10, 5, { 48,60 }, _spHelpMouse, _helpMousePos, 0, _helpBottomScale);
		FrameAnimation(1, 5, 15, { 131,46 }, _spHelpSpace, _helpSpacePos, 0, _helpBottomScale);
	}
}

void Grid::FrameAnimation(int index, int frameTime, int frameSum, Vector2 frameSize, int sprite, Vector2 pos, float rotate, Vector2 scale, int color)
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
	float aditionX = rotatedPos.x * cosf(rotate) - rotatedPos.y * sinf(rotate);
	float aditionY = rotatedPos.y * cosf(rotate) + rotatedPos.x * sinf(rotate);
	rotatedPos = { aditionX ,aditionY };
	rotatedPos = { rotatedPos.x + pos.x ,rotatedPos.y + pos.y };
	Novice::DrawSpriteRect((int)(rotatedPos.x), (int)(rotatedPos.y), listX, listY, int(frameSize.x), int(frameSize.y), sprite, frameSize.x / listW * scale.x, frameSize.y / listH * scale.y, rotate, color);
}

bool Grid::FrameTimeWatch(int frame, int index, bool first)
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

bool Grid::FrameTimeWatch_ani(int frame, int index, bool first)
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

Grid::Grid()
{
	textureGrid_ = Novice::LoadTexture("./RS/UI/UI_Grid.png");
	_spBullet_bg = Novice::LoadTexture("./RS/UI/bullet_ui_bg.png");
	_spBullet_outline = Novice::LoadTexture("./RS/UI/bullet_ui_outline.png");
	_spBullet_cutline = Novice::LoadTexture("./RS/UI/bullet_ui_cutline.png");
	_spHelpMouse = Novice::LoadTexture("./RS/UI/MouseAni.png");
	_spHelpSpace = Novice::LoadTexture("./RS/UI/SpaceKeyAni.png");
}

Grid::~Grid()
{
}

void Grid::Initialize()
{
	_bulletShakeOffset = { 0,0 };
	_bulletPosOffset = _bulletPos;
	_isBulletShake = false;
	_isHelpStart = true;
	_helpBottomScale = { 1,1 };
}

void Grid::Update()
{
}

void Grid::Draw(int currentBullet, int maxBullet)
{
	//外边框
	Novice::DrawSprite(0, 0, textureGrid_, 1, 1, 0.0f, WHITE);
	//子弹
	DrawBullet(currentBullet, maxBullet);
	//最开始的按键提示
	HelpButtom();
}
