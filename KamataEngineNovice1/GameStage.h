#pragma once
#include "Player.h"
class GameStage
{
public:
	GameStage();

	~GameStage();

	void Initialize();

	void Updadte();

	void Draw();
private:
	Player* player_ = nullptr;
};

