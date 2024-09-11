#pragma once
//敌人类
#include <Novice.h>
#include <vector>
#include <cmath>
#include <queue>
#include <stack>
#include <random>
#include "Character.h"
#include "Camera.h"
class EnemyTools;

class Enemy : public Character
{
private:
	//相机映射计算
	Camera* _camera = nullptr;			//相机
	Affine _affine = {};				//Affine合集(尺寸，旋转，位移)
	Vertex _screen = {};				//屏幕坐标合集

	//基础属性
	Vector2 _pos = { -100,-100 };		//中心坐标
	float _rotate = 0;					//旋转(弧度)
	int _sprite = 0;					//贴图（一般都是图集，所有本地贴图都存在EnemyManager）
	Vector2 _size{};					//物体的大小（需要碰撞就调用这个）
	Vector2 _spriteSize{};				//贴图的大小
	int _frameSum = 1;					//动画帧数总数
	Vector2 _scale = { 1,1 };			//尺寸
	unsigned int _color = WHITE;		//颜色
	int alphaValue = 255;				//颜色的alpha值

	//战斗属性
	//战斗相关的属性每个敌人类型都不同，就写到了初始化方法中了，这里只补充基类缺少的变量
	float _speed = 0;
	Vector2 _targetPos{};//用于移动的目标位置

	//状态
	bool _isDead = false;
	bool _isGetPlayer = false;

	//工具
	EnemyTools* tools;

	void Move();
	void Control(char keys[], char preKeys[]);//这个函数会操控敌人，只用作测试使用

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
	void Initialize(Camera* camera, Vector2 pos, Type type, Vector2 targetPos);//初始化(坐标，弧度，类型,目标位置)
	void Update(char keys[], char preKeys[]);//更新
	void Draw() override;//描画 重写
	void PushUpdate();//放入开始的循环中(生成敌人后，要调用这个方法敌人才会进入主循环)
	void ToDead();

	//Get&Set
	const int& Get_hp() const { return hp_; };
	const Vector2& Get_pos() const { return _pos; };
	const Vector2& Get_targetPos() const { return _targetPos; };
	void Set_sprite(const int& sprite) { _sprite = sprite; };
	const Vector2& Get_scale() const { return _scale; };
	void Set_scale(const Vector2& scale) { _scale = scale; };
	const bool& Get_isDead() const { return _isDead; };
	void Set_isDead(const bool& flag) { _isDead = flag; };
	const Vector2& Get_size() const { return _size; };
	const Type& Get_type() const { return _type; };
	void Set_spriteSize(const Vector2& size) { _spriteSize = size; };
	const Vector2& Get_spriteSize() const { return _spriteSize; };

	const Vector2& GetTranslate() const { return _affine.translate; }				//获取中心点
	void SetTranslate(const Vector2& translate) { _affine.translate = translate; }	//设置中心点
	const float GetRadian() const { return _size.x / 2.f; };						//获取半径
};

class EnemyManager {
private:
	//敌人生成路线
	inline static int _linesSum = 2;									//当前有多少条线路
	inline static const int _linesSumMax = 6;							//至多多少条路线
	inline static Vector2 _targetPos = { 640.0f, 220.0f + 30.f };		//移动的目标位置
	inline static Vector2 _bornPosOffset[_linesSumMax] = {
		{ -1000, 0 },{ 1000, 0 },{ -1000, 300 } ,{ 1000, 300 },{ -500, 800 } ,{ 500, 800 }
	};																	//和目标点之前的偏移
	inline static std::stack<Enemy*> _enemyLines[_linesSumMax];			//包含每条路线中生成的敌人
	//敌人生成随机数
	inline static int _lineTime = 0;				//进行随机选择路线的时间(具体调整写到了BornEnemy中)
	inline static int _bornEnemyTime = 0;			//路线中生成敌人的时间
	inline static int _eachBornMax = 0;				//每回至多生成敌人数量
	//生成敌人种类(最初能生成的种类，随着难度，会调整这个数组中的敌人类型)
	inline static Enemy::Type _enemyType_walk[2] = { Enemy::tSnake,Enemy::tSnake };
	inline static Enemy::Type _enemyType_fly[2] = { Enemy::tEagles,Enemy::tEagles };
	//生成小伙伴
	inline static int _bornFriendTime = 60;			//生成小伙伴的判断时间
	inline static int _bornFriendSpace = 6;			//小伙伴的前后要空多少个位置
	inline static int _bornFriendRandom = 1;		//生成的几率

	//工具
	inline static int _currentTimes[10] = { 0 };					// 这个用于计时器的使用
	static bool FrameTimeWatch(int frame, int index, bool first);	// 计时器：帧时间、编号、首次是否输出true

public:
	//本地贴图
	inline static int _spSnake = 0;
	inline static int _spSnake_hurt = 0;
	inline static int _spEagles = 0;
	inline static int _spEagles_hurt = 0;
	inline static int _spSpider = 0;
	inline static int _spBee = 0;
	inline static int _spPlayer_walk = 0;
	inline static int _spPlayer_fly = 0;
	static void LoadRes();//预先载入本地地图，在游戏最开始载入

	//Manager主函数
	static void Update(char keys[], char preKeys[]);
	static void Draw();

	//敌人生成相关函数
	static void BornEnemy(Camera* camera, int score, int friendSum);	//生成敌人(相机,分数,小伙伴数量)
	static void ClearAllEnemy();										//清除并回收所有激活的敌人

	//对象池相关
	inline static std::vector<Enemy*> _updatePool;	//更新对象池
	inline static std::queue<Enemy*> _idlePool;		//闲置对象池
	static Enemy* AcquireEnemy(Camera* camera, Vector2 pos, Enemy::Type type, Vector2 targetPos);// 获取一个对象，并且初始化
	static void ReleaseEnemy(Enemy* enemy);// 回收一个对象
};

class EnemyTools {
private:
	int _currentFrame_ani[3] = { 0 };								// 这个用于计时器的使用
	int _currentFrameIndex[3] = { 0 };								//用以计算播放到第几帧
	bool FrameTimeWatch_ani(int frame, int index, bool first);		// 计时器：帧时间、编号、首次是否输出true
public:
	//帧动画
	void FrameAnimation(int index, int frameTime, int frameSum, Vector2 frameSize, int sprite, Vertex screen, int color = WHITE);
	void FrameAnimation(int index, int frameTime, int frameSum, Vector2 frameSize, int sprite, Vector2 pos, float rotate = 0, Vector2 scale = { 1,1 }, int color = WHITE);
	//数学
	Vector2	AditionRule(Vector2 pos, float rad);//加法定理
	//Ease
	float EaseOutQuint(float t) { return 1.f - powf(1.f - t, 5.f); };
	float EaseInCirc(float t) { return 1 - sqrtf(1 - powf(t, 2)); };
	//工具
	int _currentTimes[10] = { 0 };								// 这个用于计时器的使用
	bool FrameTimeWatch(int frame, int index, bool first);		// 计时器：帧时间、编号、首次是否输出true
};

