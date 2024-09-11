#pragma once
#include "Player.h"
#include "Camera.h"
#include "Background.h"
#include "Filter.h"
#include "Emotion.h"
#include "Grid.h"

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
	Background* bg_ = nullptr;
	Filter* filter_ = nullptr;
	Emotion* emotion_ = nullptr;
	Grid* grid_ = nullptr;

};

