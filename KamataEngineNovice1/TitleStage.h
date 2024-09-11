#pragma once
#include "TitleBackground.h"

class TitleStage
{
private:
	TitleBackground* tBG_ = nullptr;
public:
	TitleStage();

	~TitleStage();

	void Initialize();

	void Update();

	void Draw();
};

