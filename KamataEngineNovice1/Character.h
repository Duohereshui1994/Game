#pragma once
#include "MathFunc.h"
/// <summary>
/// 基类 角色
/// </summary>
class Character
{
protected:
	//draw
	int textureHandle_;		//贴图保存位置，只有一张。如果需要的图多，就在派生类里额外定义，这个无视了就行。
	float frameNum_;		//待机动画帧数的序号 如果动画只有一个 用这个变量就行，多的话需要在派生类额外定义
	float deltaTime_;		//时间间隔 因为这个引擎的帧数是60，所以这个值是1/60

	//property
	int hpMax_;				//最大血量
	int hp_;				//当前血量
	int attack_;			//攻击力

	//transform
	MathFunc* math_ = nullptr;	//数学函数类的指针

	MathFunc::Object obj_;		//数学函数类里 对象结构体的实例

public:
	//虚函数 初始化 可以重写
	virtual void Initialize();

	//纯虚函数 派生类里必须实现
	virtual void Update(char keys[], char preKeys[]) = 0;

	//虚函数 描画 可以重写
	virtual void Draw();

	//获取位置
	Vector2 GetPosition() { return obj_.position; }

	//设定位置
	void SetPositon(Vector2 pos);
};

