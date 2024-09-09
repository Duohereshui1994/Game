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

	//Affine cameraCenter{ {1.0f,1.0f},0,{0,0} };//原点
	Affine cameraCenter{ {1.0f,1.0f},0,{640,360} };//原点
	Speed cameraSpeed{ 1.0f / 60.0f,1.0f / 60.0f,5 };//拡縮や回転や移動のスピード
	camera_ = new Camera(cameraCenter, cameraSpeed);//インスタンス
}

void GameStage::Update(char keys[], char preKeys[])
{
	camera_->Update(keys);
	player_->Update(keys, preKeys, camera_);
}

void GameStage::Draw()
{
	camera_->Draw();
	player_->Draw();
}
