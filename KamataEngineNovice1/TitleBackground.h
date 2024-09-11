#pragma once
#include "MathFunc.h"

class TitleBackground
{
private:
	int textureTitleBackground_;
	float frameNum_;			//背景动画帧数
	float deltaTime_;			//时间间隔
	MathFunc* math_ = nullptr;	//数学函数类的指针
public:
	TitleBackground();
	~TitleBackground();
	//初始化
	void Initialize();
	//更新
	void Update();
	//描画
	void Draw();
};


