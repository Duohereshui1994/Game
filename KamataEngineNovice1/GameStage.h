#pragma once
//game界面的类
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Camera.h"
#include "Background.h"
#include "Filter.h"
#include "Emotion.h"
#include "Grid.h"
#include "Score.h"
#include "Particle.h"
#include "AudioClip.h"

class GameStage
{
public:
	GameStage();

	~GameStage();

	void Initialize();

	void Update(char keys[], char preKeys[]);

	void Draw();

	void IsCollision();//碰撞判定

	bool IsFinished() const { return finished_; }
private:
	Player* player_ = nullptr;
	Camera* camera_ = nullptr;
	Background* bg_ = nullptr;
	Filter* filter_ = nullptr;
	Emotion* emotion_ = nullptr;
	Grid* grid_ = nullptr;
	AudioClip* audioClip_ = nullptr;

	//Scene切换
	bool finished_ = false;
};

