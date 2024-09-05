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

void GameStage::Updadte()
{
	player_->Update();
}

void GameStage::Draw()
{
	player_->Draw();
}
