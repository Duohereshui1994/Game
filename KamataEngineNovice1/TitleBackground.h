#pragma once
//title界面的背景类
#include "MathFunc.h"

class TitleBackground
{
private:
	int textureTitleBackground_;

	int textureHelpBackground_;
	int param_;				//背景按钮参数

	int mousePosX;
	int mousePosY;

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
	void DrawHelp();

	int GetParam() const { return param_; }
	void SetParam(int param);
};


