#pragma once
#include "ClearBackground.h"
#include "AudioClip.h"
#include "Score.h"

enum class ClearButtonType {
	kNull,
	kRestart,
	kMenu,
};


//结算界面stage
class ClearStage
{
private:
	//Scene切换用参数
	bool finished_ = false;

	ClearBackground* cBG_ = nullptr;

	ClearButtonType buttonType_;

	AudioClip* audioClip_ = nullptr;

	int _spListNum = 0;
	Vector2 _scorePos = { 530,240 };
	Vector2 _scoreHighPos = { 530,350 };

public:
	ClearStage();

	~ClearStage();

	void Initialize();

	void Update(char keys[], char preKeys[]);

	void Draw();

	void DrawScore(Vector2 pos, int score);

	bool IsFinished() const { return finished_; }

	ClearButtonType GetButtonType() const { return buttonType_; }
};

