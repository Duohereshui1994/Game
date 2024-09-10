#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Camera.h"
class GameStage
{
public:
	GameStage();

	~GameStage();

	void Initialize();

	void Update(char keys[], char preKeys[]);

	void Draw();
private:
	Player* player_ = nullptr;
	Camera* camera_ = nullptr;


};

