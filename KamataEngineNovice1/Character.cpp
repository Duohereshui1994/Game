#include "Character.h"
#include "Novice.h"

void Character::Initialize()
{
	textureHandle_ = 0;
	frameNum_ = 0;
	deltaTime_ = 1.0f / 60.0f;

	// 初始化 MathFunc 对象
	math_ = new MathFunc();

	// 初始化 Mono 结构体成员
	obj_.position = Vector2(0.0f, 0.0f);
	obj_.width = 64.0f;
	obj_.height = 64.0f;
	obj_.center = Vector2(obj_.position.x + obj_.width / 2.0f, obj_.position.y + obj_.height / 2.0f);
	obj_.scale = Vector2(1.0f, 1.0f);
	obj_.rotate = 0.0f;

}



void Character::Draw()
{
	if (frameNum_ > 4) {
		frameNum_ = 0;
	}
	else {
		frameNum_ += deltaTime_ * 2;
	}
	Novice::DrawSpriteRect((int)obj_.position.x, (int)obj_.position.y, (int)frameNum_ * 64, 0, (int)obj_.width, (int)obj_.height, textureHandle_, 1.0f / 4.0f, 1.0f, 0.0f, WHITE);

}
