#include "GameStage.h"

GameStage::GameStage()
{
}

GameStage::~GameStage()
{
	delete player_;
}

void GameStage::Initialize()
{
	player_ = new Player();
	player_->Initialize();
}

void GameStage::Updadte(char keys[], char preKeys[])
{
	player_->Update(keys,preKeys);
}

void GameStage::Draw()
{
	player_->Draw();
}
