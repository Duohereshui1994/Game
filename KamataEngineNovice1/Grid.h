#pragma once
#include <struct.h>
//UI外框类
class Grid
{
private:
	//贴图
	int textureGrid_;
	int _spBullet_outline = 0;
	int _spBullet_bg = 0;
	int _spBullet_cutline = 0;
	//位置
	Vector2 _bulletPos = { 1280.f / 2.f - 123.f / 2.f,560 };
	//方法
	void DrawBullet(int currentBullet, int maxBullet);
public:
	Grid();
	~Grid();
	void Initialize();
	void Update();
	void Draw(int currentBullet, int maxBullet);
};

