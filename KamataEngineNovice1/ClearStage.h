#pragma once
#include "ClearBackground.h"

//结算界面stage
class ClearStage
{
private:
	//Scene切换用参数
	bool finished_ = false;

	ClearBackground* cBG_ = nullptr;

public:
	ClearStage();

	~ClearStage();

	void Initialize();

	void Update(char keys[], char preKeys[]);

	void Draw();

	bool IsFinished() const { return finished_; }
};

