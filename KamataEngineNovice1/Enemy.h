#pragma once
#include <Novice.h>
#include <vector>
#include <cmath>
#include <queue>
#include <stack>
#include <random>
#include <cassert>
#include "Character.h"
class EnemyTools;

class Enemy : public Character
{
private:
	//基础属性
	Vector2 _pos = { -100,-100 };		//中心坐标
	float _rotate = 0;					//旋转(弧度)
	int _sprite = 0;					//贴图（一般都是图集，所有本地贴图都存在EnemyManager）
	Vector2 _size{};					//物体的大小（需要碰撞就调用这个）
	Vector2 _spriteSize{};				//贴图的大小
	int _frameSum = 1;					//动画帧数总数
	Vector2 _scale = { 1,1 };			//尺寸
	int _color = WHITE;					//颜色

	//战斗属性
	//战斗相关的属性每个敌人类型都不同，就写到了初始化方法中了，这里只补充基类缺少的变量
	float _speed = 0;
	Vector2 _targetPos = { 1280 / 2.f,720 / 2.f + 150 };//用于移动的目标位置

	//状态
	bool _isDead = false;
	bool _isCollison = false;

	//工具
	EnemyTools* tools;

	void Move();

public:
	//类型
	inline static enum Type {
		tSnake,		//蛇
		tEagles,	//老鹰
		tSpider,	//蜘蛛
		tBee,		//蜜蜂
		tPlayer,	//小伙伴
	} enemyType;
	Type _type = tSnake;

	//基础方法
	Enemy();
	~Enemy();
	void Initialize(Vector2 pos, float rotate, Type type);//初始化(坐标，弧度，类型)
	void Update(char keys[], char preKeys[]);//更新
	void Draw() override;//描画 重写
	void PushUpdate();//放入开始的循环中(生成敌人后，要调用这个方法敌人才会进入主循环)

	//功能方法

	//Get&Set
	const int& GetHp() const { return hp_; };
	const Vector2& Get_targetPos() const { return _targetPos; };
};

class EnemyManager {
private:
	//敌人生成路线
	inline static const int _linesSum = 6;						//一共多少条路线
	inline static Vector2 _bornPosOffset[_linesSum] = {
		{ -800, 0 },{ -700, -300 } ,{ -400, -600 } ,{ 400, -600 } ,{ 700, -300 } ,{ 800, 0 }
	};															//和目标点之前的偏移
	inline static std::stack<Enemy*> _enemyLines[_linesSum];	//包含每条路线中生成的敌人
	//敌人生成随机数
	inline static int _lineTime = 30;				//进行随机选择路线的时间
	inline static int _bornEnemyTime = 30;			//路线中生成敌人的时间
	inline static int _eachBornMax = 5;				//每回至多生成敌人数量

	//工具
	inline static int _currentTimes[10] = { 0 };					// 这个用于计时器的使用
	static bool FrameTimeWatch(int frame, int index, bool first);	// 计时器：帧时间、编号、首次是否输出true

public:
	//本地贴图
	inline static int _spSnake = 0;
	inline static int _spEagles = 0;
	inline static int _spPlayer = 0;
	static void LoadRes();//预先载入本地地图，在游戏最开始载入

	//Manager主函数
	static void Update(char keys[], char preKeys[]);
	static void Draw();

	//敌人生成相关函数
	static void BornEnemy(int score, int friendSum);
	static void ClearEnemyLines();

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
	//工具
	int _currentTimes[10] = { 0 };								// 这个用于计时器的使用
	bool FrameTimeWatch(int frame, int index, bool first);		// 计时器：帧时间、编号、首次是否输出true
};

