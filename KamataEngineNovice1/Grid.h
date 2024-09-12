#pragma once
#include <struct.h>
#include <random>
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
	//变量
	float _randBulletShake = 3;				//抖动幅度(想要改变效果修改这个)
	Vector2 _bulletShakeOffset{};			//抖动的偏差值
	Vector2 _bulletPosOffset = _bulletPos;	//原始坐标值
	bool _isBulletShake = false;
	//方法
	void DrawBullet(int currentBullet, int maxBullet);
	void BulletShake();
	//工具
	int _currentFrame_ani[3] = { 0 };								// 这个用于计时器的使用
	int _currentFrameIndex[3] = { 0 };								//用以计算播放到第几帧
	bool FrameTimeWatch_ani(int frame, int index, bool first);		// 计时器：帧时间、编号、首次是否输出true
public:
	Grid();
	~Grid();
	void Initialize();
	void Update();
	void Draw(int currentBullet, int maxBullet);
};

