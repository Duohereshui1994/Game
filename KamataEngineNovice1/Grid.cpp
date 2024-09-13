#include "Grid.h"
#include "Novice.h"

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
		if (FrameTimeWatch_ani(30, 0, false))
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
}

Grid::~Grid()
{
}

void Grid::Initialize()
{
	_bulletShakeOffset = { 0,0 };
	_bulletPosOffset = _bulletPos;
	_isBulletShake = false;
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
}
