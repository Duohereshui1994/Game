#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Camera.h"
#include "Score.h"
#include "Particle.h"
class GameStage
{
public:
	GameStage();

	~GameStage();

	void Initialize();

	void Update(char keys[], char preKeys[]);

	void Draw();

	void IsCollision();//碰撞判定
private:
	Player* player_ = nullptr;
	Camera* camera_ = nullptr;


};

