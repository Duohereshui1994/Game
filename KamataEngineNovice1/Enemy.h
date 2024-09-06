#pragma once
#include <Novice.h>
#include <vector>
#include <cmath>
#include <queue>
#include "Character.h"
class EnemyTools;

class Enemy : public Character
{
private:
	//基础属性
	Vector2 _pos = { -100,-100 };		//中心坐标
	float _rotate = 0;					//旋转(弧度)
	int _sprite = 0;					//贴图（一般都是图集，所有本地贴图都存在EnemyManager）
	Vector2 _size = { 96,96 };			//物体的大小（需要碰撞就调用这个）
	Vector2 _spriteSize = { 96,96 };	//贴图的大小
	int _frameSum = 1;					//动画帧数总数
	Vector2 _scale = { 1,1 };			//尺寸
	int _color = WHITE;					//颜色

	//战斗属性
	//战斗相关的属性每个敌人类型都不同，就写到了初始化方法中了，这里只补充基类缺少的变量
	float _speed = 0;

	//状态
	bool _isDead = false;
	bool _isCollison = false;

	//工具
	EnemyTools* tools;

public:
	inline static enum Type {
		tSnake,		//蛇
		tEagles,	//老鹰
		tSpider,	//蜘蛛
		tBee,		//蜜蜂
	} enemyType;
	Type _type = tSnake;

	Enemy();
	~Enemy();
	//初始化(坐标，弧度，类型)
	void Initialize(Vector2 pos, float rotate, Type type);
	//更新
	void Update(char keys[], char preKeys[]);
	//描画 重写
	void Draw() override;
	//放入开始的循环中(生成敌人后，要调用这个方法敌人才会进入主循环)
	void PushUpdate();

	const int& GetHp() const { return hp_; };
};

class EnemyManager {
public:
	//本地贴图
	inline static int _spSnake = 0;
	static void LoadRes();//预先载入本地地图，在游戏最开始载入

	//Manager主函数
	static void Update(char keys[], char preKeys[]);
	static void Draw();

	//对象池相关
	inline static std::vector<Enemy*> _updatePool;	//更新对象池
	inline static std::queue<Enemy*> _idlePool;		//闲置对象池
	static Enemy* AcquireEnemy(Vector2 pos, float rotate, Enemy::Type type);// 获取一个对象，并且初始化
	static void ReleaseEnemy(Enemy* enemy);// 回收一个对象
};

class EnemyTools {
private:
	int _currentFrame_ani[3] = { 0 };								// 这个用于计时器的使用
	int _currentFrameIndex[3] = { 0 };								//用以计算播放到第几帧
	bool FrameTimeWatch_ani(int frame, int index, bool first);		// 计时器：帧时间、编号、首次是否输出true
public:
	//帧动画
	void FrameAnimation(int index, int frameTime, int frameSum, Vector2 frameSize, int sprite, Vector2 pos, float rotate = 0, Vector2 scale = { 1,1 }, int color = WHITE);
	//数学
	Vector2	AditionRule(Vector2 pos, float rad);//加法定理
};
