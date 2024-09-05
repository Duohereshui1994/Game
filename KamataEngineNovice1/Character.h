#pragma once
#include "MathFunc.h"
class Character
{
protected:
	//draw
	int textureHandle_;
	float frameNum_;
	float deltaTime_;

	//property
	int hpMax_;
	int hp_;
	int attack_;

	//transform
	MathFunc* math_ = nullptr;

	MathFunc::Object obj_;

public:
	virtual void Initialize();
	virtual void Update(char keys[], char preKeys[]) = 0;
	virtual void Draw();

	Vector2 GetPosition() { return obj_.position; }
};

