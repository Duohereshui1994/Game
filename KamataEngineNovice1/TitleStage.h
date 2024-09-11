#pragma once
//title界面类
#include "TitleBackground.h"

class TitleStage
{
private:
	//Scene切换用参数
	bool finished_ = false;

	TitleBackground* tBG_ = nullptr;
public:
	TitleStage();

	~TitleStage();

	void Initialize();

	void Update(char keys[], char preKeys[]);

	void Draw();

	bool IsFinished() const { return finished_; }
};

