#pragma once
//title界面类
#include "TitleBackground.h"
#include "AudioClip.h"

enum class TitleButtonType {
	kNull,
	kHelp,
	kStart,
	kExit
};

class TitleStage
{
private:
	//Scene切换用参数
	bool finished_ = false;

	bool isHelpOn_ = false;

	TitleButtonType buttonType_;

	TitleBackground* tBG_ = nullptr;

	AudioClip* audioClip_ = nullptr;
public:
	TitleStage();

	~TitleStage();

	void Initialize();

	void Update();

	void Draw();

	bool IsFinished() const { return finished_; }

	TitleButtonType GetButtonType() const { return buttonType_; }
};

