#pragma once

//结算界面stage
class ClearStage
{
private:
	//Scene切换用参数
	bool finished_ = false;

public:
	ClearStage();

	~ClearStage();

	void Initialize();

	void Update(char keys[], char preKeys[]);

	void Draw();

	bool IsFinished() const { return finished_; }
};

