#pragma once
#include "MathFunc.h"

//结束界面背景
class ClearBackground
{
private:
	int textureClearBackground_;
	int param_;				//背景按钮
	MathFunc* math_ = nullptr;	//数学函数类的指针
public:
	ClearBackground();
	~ClearBackground();
	//初始化
	void Initialize();
	//更新
	void Update(char keys[], char preKeys[]);
	//描画
	void Draw();
};

